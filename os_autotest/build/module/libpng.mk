MODULE_NAME     := libpng
MODULE_VER      := v1.6.38
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

.PHONY: build

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://github.com/glennrp/libpng

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	cd $(MODULE_FULLNAME) && ./configure --prefix=$(MODULE_INSTALL) --host=$(TOOLCHAIN_PREFIX) \
		CPPFLAGS="-I$(DIR_INSTALL)/zlib/include" \
		LDFLAGS="-L$(DIR_INSTALL)/zlib/lib"

	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install

clean:
	rm -rf $(MODULE_INSTALL)
