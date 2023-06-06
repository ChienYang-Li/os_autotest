http:
	@ sudo docker run -it --user $$(id -u):$$(id -g) --name $(CODENAME)-http --rm=true \
		-w /web \
		-v $(DIR_WORK)/install:/web \
		-e LANG=en_US.UTF-8 \
		-e TIMESTAMP=$(TIMESTAMP) \
		-p 80:80 \
		-d \
		$(CODENAME) python3 -m http.server 80

nfs: $(DIR_NFS)
	sudo sh -c "echo '$(DIR_NFS) *(rw,sync,no_root_squash,no_subtree_check)' > /etc/exports"
	sudo exportfs -a
	sudo service nfs-kernel-server restart
