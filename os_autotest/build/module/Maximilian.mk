MODULE_NAME     := Maximilian
MODULE_VER      := MaximilianV1
MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
	git clone https://github.com/micknoise/Maximilian.git

unzip:
	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)

build:
	cd $(MODULE_FULLNAME) && g++ -Wall -D__LINUX_ALSA__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp \
	-lasound -lpthread \
	-I$(DIR_INSTALL)/alsa-lib/usr/include \
	-L$(DIR_INSTALL)/alsa-lib/usr/lib

#	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)


install:
#	make -C $(MODULE_FULLNAME) install
	cd $(MODULE_FULLNAME) && ./maximilian

clean:
	rm -rf $(MODULE_INSTALL)
