LIST_APP := $(shell cd $(DIR_APP) && ls -1 -d */ | awk -F/ '{print $$1}')

$(shell for x in $(LIST_BOARD); do for y in $(LIST_APP); do echo $$y/$$x; done; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make exec CMD="make -C $(DIR_HOME) +/$(@D) THE_BOARD=$(@F)"
$(shell for y in $(LIST_APP); do echo +/$$y; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make -C $(DIR_APP)/$(@F)
