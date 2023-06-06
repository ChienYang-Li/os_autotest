MODULE_NAME     := fftw
MODULE_VER      := 3.3.10
MODULE_FULLNAME := $(MODULE_NAME)-$(MODULE_VER)
MODULE_TARBALL  := $(MODULE_FULLNAME).tar.gz
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)


all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	wget -c https://www.fftw.org/$(MODULE_TARBALL)

unzip:
#	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)
	tar xf $(MODULE_TARBALL)

build:
	cd $(MODULE_FULLNAME) && ./configure --host=$(TOOLCHAIN_PREFIX) --prefix=$(MODULE_INSTALL)

	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install

clean:
	rm -rf $(MODULE_INSTALL)
