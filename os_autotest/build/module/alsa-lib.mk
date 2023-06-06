MODULE_NAME     := alsa-lib
MODULE_VER      := v1.2.8
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

.PHONY: build

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone git://git.alsa-project.org/alsa-lib.git

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	cd $(MODULE_FULLNAME) && ./gitcompile --host=$(TOOLCHAIN_PREFIX) \
	--enable-static=no \
	--enable-shared=yes
#	--with-configdir=/usr/local/share
#	--prefix=$(MODULE_INSTALL)

	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install DESTDIR=$(MODULE_INSTALL)

clean:
	rm -rf $(MODULE_INSTALL)



