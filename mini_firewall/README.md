Overview
--------
The package is a mini firewall which is rebuilt according [this artical](http://www.roman10.net/2011/07/23/a-linux-firewall-using-netfilter-part-1overview/) 

It is a practice for me to
* learn how to write a linux kernel module
* learn how to bridge user space and kernel space
* enhance my network perspectives

The mini firewall can BLOCK or LOG packets according to a set of rules.The rules
are set by a user space configuraiton utility program.
User can specify three conditions of ip to BLOCK and LOG:
* ip with specific port
* specific ip
* all of ip in same net id


Environment
--------
Linux kernel for version 4.4.0 .


Build and run
--------
```bash
cd miniFirewall
make
cd module
make
sudo insmod mf_km.ko
cd ..
sudo ./mf --out -t 31.13.87.36 -a BLOCK
```

close the mini firewall
```bash
sudo rmmod mf_km
```



minifirewall: a packet filtering firewall for GNU/Linux systems
minifirewall is a simple packet filtering firewall, written in C, for GNU/Linux based systems. It uses Netfilter's hooks to watch the inbound and outbound packets of a computer in a network.

Source files' description
Under the LKM directory:
minifw.c : A Linux Kernel module (LKM) which implements Netfilter's hooks mainly NF_INET_LOCAL_IN and NF_INET_LOCAL_OUT to filter the packets.
minifw.h : Has all the structure details and other macros needed to implement the rules of minifirewall (the details in this header file must be consistent with the details in the user space minifirewall header).

Under the Userspace directory:
minifirewall.c : This tool acts as the user space program for setting minifirewall's packet filtering rules. It uses "getopt.h" header to parse the arguments sent to it. Please get used to its arguments' notation. Will include the "help" details to the code after some clean-up work.
minifirewall.h : Contains macros and structs as in minifw.c. All the fields in the struct my_ipt are initialised in minifirewall.c and are sent to minifw LKM through /proc. </br/>

Brief demo
Follow the steps given below to insert minifw LKM into the kernel.

    cd LKM
    make
    sudo insmod minifw.ko
You must see a module by the name minifw when you do a lsmod | head -3 to list out the modules in your system. Also you must observe that there is an entry by the name minifw in the /proc directory which you can find out by doing a ls /proc | grep "minifw".

Run the user-space minifirewall program after compiling it. Follow the steps given below to test it.

    gcc -o -Wall minifirewall minifirewall.c
    sudo ./minifirewall --in --proto ALL --action BLOCK
    ping www.google.com
What do you observe? You should NOT be able to ping any server since you have written a minifirewall rule to block all the incoming packets bounded to your system. Also you can try opening a webpage in your browser which should be unsuccessful. If not, then there is some problem with passing/registering the rules with the LKM minifw.

Play around with some more rules of minifirewall by going through its source until I update a "help" section which lists out all the parameters for minifirewall's rules.

