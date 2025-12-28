obj-m += mmap_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc mmap_app.c -o mmap_app

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f mmap_app
