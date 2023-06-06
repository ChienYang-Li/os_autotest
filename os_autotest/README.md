sudo apt -y install nfs-kernel-server


mount -t nfs -o nolock -o tcp 192.168.1.109:/home/xxxxxx/factory-pi/.work/nfs /tmp/nfs
