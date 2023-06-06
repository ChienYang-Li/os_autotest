MODULE_NAME     := opencv
MODULE_VER      := 4.6.0
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

.PHONY: build

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://github.com/opencv/opencv

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	mkdir -p $(MODULE_FULLNAME)/x && cd $(MODULE_FULLNAME)/x && cmake \
		-DCMAKE_C_COMPILER=$(TOOLCHAIN_PREFIX)-gcc \
		-DCMAKE_CXX_COMPILER=$(TOOLCHAIN_PREFIX)-g++ \
		-DCMAKE_INSTALL_PREFIX=$(MODULE_INSTALL) \
		-DCMAKE_INCLUDE_PATH="$(DIR_INSTALL)/zlib/include" \
		-DCMAKE_LIBRARY_PATH="$(DIR_INSTALL)/zlib/lib" \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=ON \
		-DBUILD_TESTS=OFF \
		-DBUILD_PERF_TESTS=OFF \
		-DBUILD_EXAMPLES=OFF \
		-DBUILD_opencv_apps=OFF \
		-DWITH_GTK=OFF \
		-DWITH_IPP=OFF \
		..

	make -C $(MODULE_FULLNAME)/x -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME)/x install

clean:
	rm -rf $(MODULE_INSTALL)
