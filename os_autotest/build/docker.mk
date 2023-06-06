docker:
	rm -rf /tmp/empty-$(CODENAME) && mkdir -p /tmp/empty-$(CODENAME)
	cd /tmp/empty-$(CODENAME) && sudo docker build --rm -t $(CODENAME) -f $(DIR_BUILD)/dockerfile . --no-cache

exec:
	@ sudo docker run -it --user $$(id -u):$$(id -g) --name $(CODENAME)-$(@F)-$$(uuidgen) --rm=true \
		-w $(DIR_HOME) \
		-v $(DIR_HOME):$(DIR_HOME) \
		-e LANG=en_US.UTF-8 \
		-e TIMESTAMP=$(TIMESTAMP) \
		$(CODENAME) $(CMD)
