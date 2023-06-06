THE_BOARD        := raspberry-pi-zero-w

CPU_ARCH         := arm
TOOLCHAIN_PREFIX := arm-linux-gnueabihf

DEF_UBOOT_CONF   := rpi_0_w_defconfig
DEF_LINUX_CONF   := bcm2835_defconfig

# -----------------------------------------------------------------------------
IMAGE_BIN := $(DIR_INTERMEDIATE)/$(THE_BOARD).img

+/image:
	dd if=/dev/zero bs=512 count=10240 | tr "\000" "\377" > $(IMAGE_BIN)

	dd of=$(IMAGE_BIN) bs=512 if=$(DIR_INSTALL)/u-boot.bin seek=16   conv=notrunc
	dd of=$(IMAGE_BIN) bs=512 if=$(DIR_INSTALL)/zImage     seek=2048 conv=notrunc

	cp $(IMAGE_BIN) $(DIR_INSTALL)/$(THE_BOARD).img

+/rootfs:
