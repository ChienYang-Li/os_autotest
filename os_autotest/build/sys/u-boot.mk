MODULE_NAME     := u-boot
MODULE_VER      := v2022.10
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)

UBOOT_CONF      := $(DIR_CONF)/u-boot.$(THE_BOARD).config

.PHONY: build

all: clean $(MODULE_FULLNAME) unzip build/$(THE_BOARD) install/$(THE_BOARD)

$(MODULE_FULLNAME):
	git clone https://github.com/u-boot/u-boot

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build/orange-pi-pc: config
#	perl -pi -e "s:CONFIG_BOOTCOMMAND=.*:CONFIG_BOOTCOMMAND=\"fatload mmc 0 41000000 boot.scr;source 41000000\":g"  $(MODULE_FULLNAME)/.config
	make -C $(MODULE_FULLNAME) ARCH=$(CPU_ARCH) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES)
build/raspberry-pi-zero-w: config
	make -C $(MODULE_FULLNAME) ARCH=$(CPU_ARCH) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES)

install/orange-pi-pc:
	cd $(MODULE_FULLNAME) && cp u-boot-sunxi-with-spl.bin $(MODULE_INSTALL)
install/raspberry-pi-zero-w:
	cd $(MODULE_FULLNAME) && cp u-boot.bin $(MODULE_INSTALL)

clean:
	rm -f $(MODULE_INSTALL)/u-boot.bin

config:
	cd $(MODULE_FULLNAME) && [ ! -f $(UBOOT_CONF) ] && \
		make CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- $(DEF_UBOOT_CONF) || \
		cp $(UBOOT_CONF) .config

menuconfig: $(MODULE_FULLNAME) unzip config
	make -C $(MODULE_FULLNAME) ARCH=$(CPU_ARCH) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- menuconfig
	cd $(MODULE_FULLNAME) && cp .config $(UBOOT_CONF)
