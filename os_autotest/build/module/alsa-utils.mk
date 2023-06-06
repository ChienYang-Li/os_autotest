MODULE_NAME     := alsa-utils
MODULE_VER      := 1.2.8
MODULE_FULLNAME := $(MODULE_NAME)-$(MODULE_VER)
MODULE_TARBALL  := $(MODULE_FULLNAME).tar.bz2
#MODULE_FULLNAME := $(MODULE_NAME)
MODULE_INSTALL  := $(DIR_INSTALL)/$(MODULE_NAME)

.PHONY: build

all: clean $(MODULE_FULLNAME) unzip build install

$(MODULE_FULLNAME):
#	git clone https://github.com/alsa-project/alsa-utils
	wget -c https://www.alsa-project.org/files/pub/utils/$(MODULE_TARBALL)

unzip:
#	cd $(MODULE_FULLNAME) && git reset --hard && git clean -fdx && git checkout master && git pull && git checkout $(MODULE_VER)
	tar xf $(MODULE_TARBALL)

build:
#	cd $(MODULE_FULLNAME) && \
	aclocal -I ../alsa-lib/utils && \
	gettextize -c -f --no-changelog && \
	autoheader && \
	automake --foreign --add-missing --copy --subdir-objects && \
	autoconf && \
	./configure --host=$(TOOLCHAIN_PREFIX) \
	--with-alsa=$(DIR_INSTALL)/alsa-lib/usr \
	--disable-alsamixer

#	cd $(MODULE_FULLNAME) && cp /usr/share/aclocal/gettext.m4 .

	cd $(MODULE_FULLNAME) && ./configure --prefix=$(MODULE_INSTALL) --host=$(TOOLCHAIN_PREFIX) \
	--disable-alsamixer \
	--disable-xmlto \
	--enable-shared=yes \
	--with-alsa-inc-prefix=$(DIR_INSTALL)/alsa-lib/include \
	--with-alsa-prefix=$(DIR_INSTALL)/alsa-lib/lib \
	--with-curses=ncurses \
	LDFLAGS="-L$(DIR_INSTALL)/ncurses/lib/"
#	--with-alsa-inc-prefix=$(DIR_INSTALL)/alsa-lib/usr/include \
	--with-alsa-prefix=$(DIR_INSTALL)/alsa-lib/usr/lib \
#	--with-alsa=$(DIR_INSTALL)/alsa-lib/usr \
#	--disable-libpanel \
#	--disable-alsamixer \
#	--enable-static=no \
#	--enable-shared=yes \
#	CFLAGS="-I$(DIR_INSTALL)/alsa-lib/usr/include" \
#	LDFLAGS="−L$(DIR_INSTALL)/alsa-lib/usr/lib" \
#	--prefix=$(MODULE_INSTALL)
#	cd $(MODULE_FULLNAME) && cp $(DIR_BUILD)/module/alsa-compile.sh $(DIR_INTERMEDIATE)/$(MODULE_NAME) && \
	chmod 777 $(DIR_INTERMEDIATE)/$(MODULE_NAME)/alsa-compile.sh && ./alsa-compile.sh --utils --install
	make -C $(MODULE_FULLNAME) -j$(CPU_CORES)

install:
	make -C $(MODULE_FULLNAME) install
#DESTDIR=$(MODULE_INSTALL)

clean:
	rm -rf $(MODULE_INSTALL)



