MODULE_NAME     := eigen
MODULE_VER      := 3.4.0
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://gitlab.com/libeigen/eigen.git

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	cd $(MODULE_FULLNAME) && cmake \
		-DCMAKE_C_COMPILER=$(TOOLCHAIN_PREFIX)-gcc \
		-DCMAKE_CXX_COMPILER=$(TOOLCHAIN_PREFIX)-g++ \
		-DCMAKE_INSTALL_PREFIX=$(MODULE_INSTALL) \
		.

	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install

clean:
	rm -rf $(MODULE_INSTALL)
