$(shell for x in $(LIST_BOARD); do echo image/$$x; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make exec CMD="make -C $(DIR_HOME) +/image THE_BOARD=$(@F)"
	make +image+ THE_BOARD=$(@F)

$(shell for x in $(LIST_BOARD); do echo task/$$x; done): $(DIR_INTERMEDIATE) $(DIR_INSTALL)
	make exec CMD="make -C $(DIR_HOME) +/task THE_BOARD=$(@F)"
