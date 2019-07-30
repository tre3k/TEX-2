obj-m := plx9030_detector.o
KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
GCC:=gcc

all: default

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
test:
	rmmod plx9030_detector
	insmod plx9030_detector.ko
	chmod 666 /dev/plxdetector*
	dmesg | tail -n 40
