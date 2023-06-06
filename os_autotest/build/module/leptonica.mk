MODULE_NAME     := leptonica
MODULE_VER      := 1.82.0
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

export PKG_CONFIG_PATH := $(DIR_INSTALL)/zlib/lib/pkgconfig:$(DIR_INSTALL)/libtiff/lib/pkgconfig:$(DIR_INSTALL)/libpng/lib/pkgconfig

.PHONY: build

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://github.com/danbloomberg/leptonica

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	cd $(MODULE_FULLNAME) && ./autogen.sh
	cd $(MODULE_FULLNAME) && ./configure --prefix=$(MODULE_INSTALL) --host=$(TOOLCHAIN_PREFIX) \
		--disable-programs \
		--without-jpeg \
		--without-giflib \
		--without-libwebp \
		--without-libwebpmux \
		--without-libopenjpeg

	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install

clean:
	rm -rf $(MODULE_INSTALL)
