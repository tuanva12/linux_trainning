# obj-m += timer.o

# PWD := $(shell pwd)
# CROSS=/home/caphuong/linux_trainning/bbb/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
# KER_DIR=/home/caphuong/linux_trainning/bbb/kernelbuildscripts/KERNEL

# all:
# 	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KER_DIR) M=$(PWD) modules

# clean:
# 	make -C $(KER_DIR) M=$(PWD) clean

obj-m += timer.o

all:
	 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

