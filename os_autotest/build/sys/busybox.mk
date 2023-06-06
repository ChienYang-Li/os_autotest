MODULE_NAME     := busybox
MODULE_VER      := 1.34.1
MODULE_FULLNAME := $(MODULE_NAME)-$(MODULE_VER)
MODULE_TARBALL  := $(MODULE_FULLNAME).tar.bz2
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

BUSYBOX_CONF    := $(DIR_CONF)/busybox.$(THE_BOARD).config

all: clean $(MODULE_TARBALL) unzip build install

$(MODULE_TARBALL):
	wget -c https://busybox.net/downloads/$(MODULE_TARBALL)

unzip:
	tar xf $(MODULE_TARBALL)

build: config
	make -C $(MODULE_FULLNAME) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- install CONFIG_PREFIX=$(MODULE_INSTALL)

clean: clean-src
	rm -rf $(MODULE_INSTALL)
clean-src:
	rm -rf $(MODULE_FULLNAME)

config:
	cd $(MODULE_FULLNAME) && [ ! -f $(BUSYBOX_CONF) ] && \
		make CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- defconfig || \
		cp $(BUSYBOX_CONF) .config

menuconfig: clean-src $(MODULE_TARBALL) unzip config
	make -C $(MODULE_FULLNAME) CROSS_COMPILE=$(TOOLCHAIN_PREFIX)- menuconfig
	cd $(MODULE_FULLNAME) && cp .config $(BUSYBOX_CONF)
