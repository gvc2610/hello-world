obj-m += locking_demo.o

all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
    gcc user_tester.c -o user_tester

clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
    rm -f user_tester
