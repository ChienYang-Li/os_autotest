CODENAME         = factory-pi

TIMESTAMP        ?= $(shell TZ=UTC-8 date "+%Y%m%d-%H%M%S")
CPU_CORES        := $(shell grep -c ^processor /proc/cpuinfo)

DIR_HOME         := $(shell pwd)

DIR_WORK         := $(DIR_HOME)/.work
DIR_INTERMEDIATE := $(DIR_WORK)/intermediate
DIR_INSTALL      := $(DIR_WORK)/install/$(THE_BOARD)
DIR_NFS          := $(DIR_WORK)/nfs

DIR_APP          := $(DIR_HOME)/app
DIR_BOARD        := $(DIR_HOME)/board
DIR_BUILD        := $(DIR_HOME)/build
DIR_CONF         := $(DIR_HOME)/conf

LIST_BOARD       := $(shell cd $(DIR_BOARD) && ls -1 *.mk | awk -F. '{print $$1}')

LANG             := C

-:
	@ :

-include $(DIR_BOARD)/$(THE_BOARD).mk

export

$(DIR_WORK)/%:
	mkdir -p $@

include $(DIR_BUILD)/docker.mk
include $(DIR_BUILD)/sys.mk
include $(DIR_BUILD)/module.mk
include $(DIR_BUILD)/app.mk
include $(DIR_BUILD)/image.mk
include $(DIR_BUILD)/server.mk
