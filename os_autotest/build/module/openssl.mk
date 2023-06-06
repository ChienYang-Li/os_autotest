MODULE_NAME     := openssl
MODULE_VER      := OpenSSL_1_1_1r
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://github.com/openssl/openssl

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build/arm:
	cd $(MODULE_FULLNAME) && ./Configure linux-armv4 --prefix=$(MODULE_INSTALL) --cross-compile-prefix=$(TOOLCHAIN_PREFIX)- \
		--with-zlib-include=$(DIR_INSTALL)/zlib/include \
		no-hw-xxx threads shared zlib-dynamic

	perl -pi -e 's:(.*define DEVRANDOM_WAIT).*:$$1 "/dev/urandom":g' $(MODULE_FULLNAME)/e_os.h

	perl -pi -e 's:(.*OPENSSL_armcap_P & ARMV7_TICK.*):return 0;$$1:g' $(MODULE_FULLNAME)/crypto/armcap.c
	perl -pi -e 's:(        _armv7_tick\(\);):/*$$1*/:g'               $(MODULE_FULLNAME)/crypto/armcap.c
	perl -pi -e 's:(.*OPENSSL_armcap_P \|\= ARMV7_TICK;*):/*$$1*/:g'   $(MODULE_FULLNAME)/crypto/armcap.c

build/aarch64:
	cd $(MODULE_FULLNAME) && ./Configure linux-aarch64 --prefix=$(MODULE_INSTALL) --cross-compile-prefix=$(TOOLCHAIN_PREFIX)- \
		--with-zlib-include=$(DIR_INSTALL)/zlib/include \
		no-hw-xxx threads shared zlib-dynamic

#	perl -pi -e 's:(.*define DEVRANDOM_WAIT).*:$$1 "/dev/urandom":g' $(MODULE_FULLNAME)/e_os.h

build: build/$(CPU_ARCH)
	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install

clean:
	rm -rf $(MODULE_INSTALL)
