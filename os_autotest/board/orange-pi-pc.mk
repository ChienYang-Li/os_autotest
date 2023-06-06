THE_BOARD        := orange-pi-pc

CPU_ARCH         := arm
TOOLCHAIN_PREFIX := arm-linux-gnueabihf

DEF_UBOOT_CONF   := orangepi_pc_defconfig
DEF_LINUX_CONF   := sunxi_defconfig

# -----------------------------------------------------------------------------
DIR_IMGWORK      := $(DIR_INTERMEDIATE)/$(THE_BOARD)

BIN_IMAGE        := $(DIR_IMGWORK)/image
BIN_UBOOTENV     := $(DIR_IMGWORK)/uboot.env
DIR_ROOTFS       := $(DIR_IMGWORK)/rootfs
BIN_ROOTFS       := $(DIR_IMGWORK)/rootfs.squashfs

+/image: + +ubootenv +rootfs
	dd if=/dev/zero bs=512 count=262144 | tr "\000" "\377" > $(BIN_IMAGE)

	parted $(BIN_IMAGE) mklabel msdos
	parted $(BIN_IMAGE) mkpart primary 2048s 131071s
#	2048s=1M  32767s=15M

	parted $(BIN_IMAGE) mkpart primary 131072s 262143s
#	131071s=63M 131072s=64M 262143s=127M

	dd of=$(BIN_IMAGE) bs=512 if=$(DIR_INSTALL)/u-boot-sunxi-with-spl.bin conv=notrunc seek=16
#	seek=16,start from block 16

+image+:
	sudo losetup --offset 1048576 --show --find $(BIN_IMAGE) > $(DIR_IMGWORK)/loop
#	losetup create loop device for boot
#	1048576=1M
	sudo mkfs.vfat `cat $(DIR_IMGWORK)/loop`
	mkdir -p $(DIR_IMGWORK)/mount
	sudo mount `cat $(DIR_IMGWORK)/loop` -o uid=`id -u`,gid=`id -g` $(DIR_IMGWORK)/mount
	cp $(BIN_UBOOTENV)                         $(DIR_IMGWORK)/mount
	cp $(DIR_INSTALL)/sun8i-h3-orangepi-pc.dtb $(DIR_IMGWORK)/mount
	cp $(DIR_INSTALL)/uImage                   $(DIR_IMGWORK)/mount
	cp $(BIN_ROOTFS)                           $(DIR_IMGWORK)/mount
	cp $(DIR_ROOTFS)/etc/version               $(DIR_IMGWORK)/mount
	sudo umount $(DIR_IMGWORK)/mount
	sudo losetup -d `cat $(DIR_IMGWORK)/loop`

	truncate -s 64M $(BIN_IMAGE)

	cp $(BIN_IMAGE) $(DIR_INSTALL)/$(THE_BOARD).img

+:
	rm -rf $(DIR_IMGWORK)
	mkdir -p $(DIR_IMGWORK)

+ubootenv:
	echo "bootm_size=0xa000000" >> $(BIN_UBOOTENV).tmp
	echo "bootargs=console=ttyS0,115200 root=/dev/ram0 rootfstype=squashfs" >> $(BIN_UBOOTENV).tmp
	echo "bootcmd=fatload mmc 0 42000000 uImage;fatload mmc 0 42b00000 rootfs.squashfs;fatload mmc 0 42a00000 sun8i-h3-orangepi-pc.dtb;bootm 42000000 42b00000 42a00000" >> $(BIN_UBOOTENV).tmp
#	42000000=1056MB,42a00000=1066MB,42b00000=1067MB
	mkenvimage -s 65536 -o $(BIN_UBOOTENV) $(BIN_UBOOTENV).tmp

+rootfs: ^ ^/busybox ^/libc ^/etc +/modules
	- cd $(DIR_ROOTFS) && find . | grep -v '\.ko$$' | xargs $(TOOLCHAIN_PREFIX)-strip > /dev/null 2>&1

	cd $(DIR_ROOTFS) && mksquashfs . $(BIN_ROOTFS).tmp -b 1m -comp xz
#	make squashfs file system -b(blocksize 1m) -comp(compressed to xz)
	mkimage -A arm -T ramdisk -C none -a 0 -e 0 -n 'initrd' -d $(BIN_ROOTFS).tmp $(BIN_ROOTFS)
#	Generate image for U-Boot -A(arch) -T(image type) -a(load address) -e(entry point) -n(image name) -d(image data file)

^:
	mkdir -p $(DIR_ROOTFS)
	cd $(DIR_ROOTFS) && mkdir -p dev etc lib mnt proc tmp sys zconf home
	cd $(DIR_ROOTFS) && echo $(TIMESTAMP) > etc/version

^^:
	mkdir -p $(DIR_ROOTFS)
	cd $(DIR_ROOTFS) && mkdir -p bin etc lib
	cd $(DIR_ROOTFS) && echo $(TIMESTAMP) > etc/version

^/busybox:
	cd $(DIR_ROOTFS) && cp -P --preserve=links -R "$(DIR_INSTALL)/$(@F)/"* .

^/libc:
	cd $(DIR_ROOTFS) && cp -P --preserve=links -R "/usr/$(TOOLCHAIN_PREFIX)/lib/"*.so   lib
	cd $(DIR_ROOTFS) && cp -P --preserve=links -R "/usr/$(TOOLCHAIN_PREFIX)/lib/"*.so.* lib

^/etc:
	echo '::sysinit:/etc/rcS'                                        >> $(DIR_ROOTFS)/etc/inittab
#	echo '::respawn:/sbin/getty -L ttyS0 115200 vt100'               >> $(DIR_ROOTFS)/etc/inittab
	echo '::respawn:/sbin/getty -L ttyS0 115200 vt100 -n -l /bin/sh' >> $(DIR_ROOTFS)/etc/inittab
	echo '::restart:/sbin/init'                                      >> $(DIR_ROOTFS)/etc/inittab

	echo 'proc    /proc             proc    defaults 0 0' >> $(DIR_ROOTFS)/etc/fstab
	echo 'sysfs   /sys              sysfs   defaults 0 0' >> $(DIR_ROOTFS)/etc/fstab
	echo 'tmpfs   /dev              tmpfs   defaults 0 0' >> $(DIR_ROOTFS)/etc/fstab
	echo 'tmpfs   /tmp              tmpfs   defaults 0 0' >> $(DIR_ROOTFS)/etc/fstab
	echo 'debugfs /sys/kernel/debug debugfs defaults 0 0' >> $(DIR_ROOTFS)/etc/fstab

	echo >> $(DIR_ROOTFS)/etc/mdev.conf

	cp $(DIR_BOARD)/file/rcS.$(THE_BOARD) $(DIR_ROOTFS)/etc/rcS
	chmod +x $(DIR_ROOTFS)/etc/rcS

	mkdir -p $(DIR_ROOTFS)/usr/share/udhcpc
	cp $(DIR_BOARD)/file/udhcpc.default.script $(DIR_ROOTFS)/usr/share/udhcpc/default.script
	chmod +x $(DIR_ROOTFS)/usr/share/udhcpc/default.script

	cp /usr/share/zoneinfo/Asia/Taipei  $(DIR_ROOTFS)/etc/localtime

+/modules: +m/openssl +m/curl +m/zlib +m/libxml2 +m/alsa-lib +m/opencv +m/autotest

+m/openssl:
	cp -P --preserve=links $(DIR_INSTALL)/openssl/bin/openssl $(DIR_ROOTFS)/bin
	cp -P --preserve=links $(DIR_INSTALL)/openssl/lib/*.so    $(DIR_ROOTFS)/lib
	cp -P --preserve=links $(DIR_INSTALL)/openssl/lib/*.so.*  $(DIR_ROOTFS)/lib

+m/curl:
	cp -P --preserve=links $(DIR_INSTALL)/curl/bin/curl $(DIR_ROOTFS)/bin
	cp -P --preserve=links $(DIR_INSTALL)/curl/lib/*.so $(DIR_ROOTFS)/lib

	mkdir -p $(DIR_ROOTFS)/etc/ssl/certs
	cp -P --preserve=links /etc/ssl/certs/ca-certificates.crt $(DIR_ROOTFS)/etc/ssl/certs

+m/zlib:
	cp -P --preserve=links $(DIR_INSTALL)/zlib/lib/*.so   $(DIR_ROOTFS)/lib
	cp -P --preserve=links $(DIR_INSTALL)/zlib/lib/*.so.* $(DIR_ROOTFS)/lib

+m/libxml2:
	cp -P --preserve=links $(DIR_INSTALL)/libxml2/bin/xmllint $(DIR_ROOTFS)/bin

+m/opencv:
	cp -P --preserve=links $(DIR_INSTALL)/opencv/lib/*.so   $(DIR_ROOTFS)/lib
	cp -P --preserve=links $(DIR_INSTALL)/opencv/lib/*.so.* $(DIR_ROOTFS)/lib
	mkdir -p $(DIR_ROOTFS)/home/javierli/factory-pi/.work/intermediate/opencv
	cp -P -r --preserve=links $(DIR_INTERMEDIATE)/opencv/modules $(DIR_ROOTFS)/home/javierli/factory-pi/.work/intermediate/opencv

+m/alsa-lib:
	cp -P --preserve=links $(DIR_INSTALL)/alsa-lib/usr/lib/*.so   $(DIR_ROOTFS)/lib
	cp -P --preserve=links $(DIR_INSTALL)/alsa-lib/usr/lib/*.so.* $(DIR_ROOTFS)/lib
	cp -P -r --preserve=links $(DIR_INSTALL)/alsa-lib/usr/share/* $(DIR_ROOTFS)/usr/share
#	mkdir -p $(DIR_ROOTFS)/home/javierli/factory-pi/.work/install/orange-pi-pc/
#	cp -P -r --preserve=links $(DIR_INSTALL)/alsa-lib $(DIR_ROOTFS)/home/javierli/factory-pi/.work/install/orange-pi-pc/

+m/fftw:
	cp -P --preserve=links $(DIR_INSTALL)/fftw/lib/*.so   $(DIR_ROOTFS)/lib
	cp -P --preserve=links $(DIR_INSTALL)/fftw/lib/*.so.* $(DIR_ROOTFS)/lib
#	mkdir -p $(DIR_ROOTFS)/home/javierli/factory-pi/.work/install/orange-pi-pc/
#	cp -P -r --preserve=links $(DIR_INSTALL)/alsa-lib $(DIR_ROOTFS)/home/javierli/factory-pi/.work/install/orange-pi-pc/

+m/autotest:
	mkdir -p $(DIR_ROOTFS)/usr/autotest
	cp -P -r --preserve=links $(DIR_NFS)/autotest/*   $(DIR_ROOTFS)/usr/autotest

+/task: + ^^ ^/libc +m/zlib +m/opencv
	cd $(DIR_ROOTFS) && echo $(TIMESTAMP) > etc/version

	- cd $(DIR_ROOTFS) && find . | grep -v '\.ko$$' | xargs $(TOOLCHAIN_PREFIX)-strip > /dev/null 2>&1

	cd $(DIR_ROOTFS) && mksquashfs . $(BIN_ROOTFS) -b 1m -comp xz
	cp $(BIN_ROOTFS) $(DIR_INSTALL)/task.$(THE_BOARD).$(TIMESTAMP).img
