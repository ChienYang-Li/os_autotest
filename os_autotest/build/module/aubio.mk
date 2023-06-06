MODULE_NAME     := aubio
MODULE_VER      := 0.4.9
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://github.com/aubio/aubio

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

	cd $(MODULE_FULLNAME) && ./scripts/get_waf.sh && \
	./waf configure --prefix=PREFIX=$(MODULE_INSTALL)
	./waf build


install:
#	make -C $(MODULE_FULLNAME) install
	./waf install

clean:
	rm -rf $(MODULE_INSTALL)
