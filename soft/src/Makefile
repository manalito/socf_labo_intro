obj-m := hps_gpio.o

PWD := $(shell pwd)
WARN := -W -Wall -Wstrict-prototypes -Wmissing-prototypes

all: hps_gpio

hps_gpio:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- M=$(PWD) ${WARN}
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers

