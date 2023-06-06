LIST_MODULE := $(shell cd $(DIR_BUILD)/module && ls -1 *.mk | awk -F. '{print $$1}')

$(shell for x in $(LIST_BOARD); do for y in $(LIST_MODULE); do echo $$y/$$x; done; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make exec CMD="make -C $(DIR_HOME) +/$(@D) THE_BOARD=$(@F)"
$(shell for y in $(LIST_MODULE); do echo +/$$y; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make -C $(DIR_INTERMEDIATE) -f $(DIR_BUILD)/module/$(@F).mk
