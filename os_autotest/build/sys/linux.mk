MODULE_NAME     := linux
MODULE_VER      := 6.0.3
MODULE_FULLNAME := $(MODULE_NAME)-$(MODULE_VER)
MODULE_TARBALL  := $(MODULE_FULLNAME).tar.xz
MODULE_INSTALL  := $(DIR_INSTALL)

LINUX_CONF      := $(DIR_CONF)/linux.$(THE_BOARD).config

export INSTALL_PATH     := $(DIR_INSTALL)/$(MODULE_NAME)
export INSTALL_MOD_PATH := $(DIR_INSTALL)/$(MODULE_NAME)

.PHONY: build

all: clean $(MODULE_TARBALL) unzip build/$(THE_BOARD) install/$(THE_BOARD)

$(MODULE_TARBALL):
	wget -c https://cdn.kernel.org/pub/linux/kernel/v6.x/$(MODULE_TARBALL)

unzip:
	tar xf $(MODULE_TARBALL)

build/orange-pi-pc: config
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES) uImage LOADADDR=42000000
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES) dtbs
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES) modules
build/raspberry-pi-zero-w: config
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES) zImage
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES) modules

install/orange-pi-pc:
	cp $(MODULE_FULLNAME)/arch/arm/boot/uImage                       $(MODULE_INSTALL)
	cp $(MODULE_FULLNAME)/arch/arm/boot/dts/sun8i-h3-orangepi-pc.dtb $(MODULE_INSTALL)
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- modules_install install
install/raspberry-pi-zero-w:
	cp $(MODULE_FULLNAME)/arch/arm/boot/zImage $(MODULE_INSTALL)
	make -C $(MODULE_FULLNAME) ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- modules_install install

clean: clean-src
	rm -f $(MODULE_INSTALL)/uImage
	rm -f $(MODULE_INSTALL)/zImage
	rm -rf $(INSTALL_PATH)
	rm -rf $(INSTALL_MOD_PATH)

clean-src:
	rm -rf $(MODULE_FULLNAME)

config:
	cd $(MODULE_FULLNAME) && [ ! -f $(LINUX_CONF) ] && \
		make ARCH=$(CPU_ARCH) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- $(DEF_LINUX_CONF) || \
		cp $(LINUX_CONF) .config

menuconfig: clean-src $(MODULE_TARBALL) unzip config
	make -C $(MODULE_FULLNAME) ARCH=$(CPU_ARCH) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- menuconfig
	cd $(MODULE_FULLNAME) && cp .config $(LINUX_CONF)
