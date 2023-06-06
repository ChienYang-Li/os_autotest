LIST_SYS := $(shell cd $(DIR_BUILD)/sys && ls -1 *.mk | awk -F. '{print $$1}')

$(shell for x in $(LIST_BOARD); do for y in $(LIST_SYS); do echo $$y/$$x; done; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make exec CMD="make -C $(DIR_HOME) +/$(@D) THE_BOARD=$(@F)"
$(shell for y in $(LIST_SYS); do echo +/$$y; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make -C $(DIR_INTERMEDIATE) -f $(DIR_BUILD)/sys/$(@F).mk

$(shell for x in $(LIST_BOARD); do for y in $(LIST_SYS); do echo $$y/$$x/menuconfig; done; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make -C $(DIR_HOME) ++/$(@D)
$(shell for x in $(LIST_BOARD); do for y in $(LIST_SYS); do echo ++/$$y/$$x; done; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make exec CMD="make -C $(DIR_HOME) +$(@D) THE_BOARD=$(@F)"
$(shell for y in $(LIST_SYS); do echo +++/$$y; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make -C $(DIR_INTERMEDIATE) -f $(DIR_BUILD)/sys/$(@F).mk menuconfig
