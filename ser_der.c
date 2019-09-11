#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct test
{
	int a;
	char b;
	int c;
};


unsigned char buff[sizeof(struct test)];

void ser_tx(struct test* arg, char * buff) {


   memset(buff, 0, sizeof(struct test));
  size_t offset = 0;

  printf("sizeof(arg->a):%lu offset:%d\n",sizeof(arg->a),offset );
  memcpy(buff + offset, &(arg->a), sizeof(arg->a));
  offset += sizeof(arg->a);


  printf("sizeof(arg->b):%lu offset:%d\n",sizeof(arg->b),offset );
  memcpy(buff + offset, &(arg->b), sizeof(arg->b));
  offset += sizeof(arg->b);

 printf("sizeof(arg->c):%lu offset:%d\n",sizeof(arg->c),offset );
  memcpy(buff + offset, &(arg->c), sizeof(arg->c));
  offset += sizeof(arg->c);
 
 printf("offset:%d\n",offset );
   /* Set all remaining bytes to zero */
  memset(buff + offset, 0, sizeof(struct test) - offset);
 

 for(int i =0; i< sizeof(struct test); i++) {
 	printf("buff[%d]:%x\n",i,buff[i] );
 }


} 


void desr_rx(char *buff)  {
  
  int offset = 0;
  struct test rx_arg;

  memcpy(&(rx_arg.a),buff+offset,sizeof(typeof(rx_arg.a)));
  offset = offset + sizeof(typeof(rx_arg.a));

  memcpy(&(rx_arg.b),buff+offset,sizeof(typeof(rx_arg.b)));
  offset = offset + sizeof(typeof(rx_arg.b));

  memcpy(&(rx_arg.c),buff+offset,sizeof(typeof(rx_arg.c)));
  offset = offset + sizeof(typeof(rx_arg.c));

  printf("rx_arg.a:%x\n",rx_arg.a );
  printf("rx_arg.b:%x\n",rx_arg.b );
  printf("rx_arg.c:%x\n",rx_arg.c );
}

int main() {

struct test arg = {.a = 0x11223344, .b = 0x55, .c = 0x66778899};


  /* May be larger than strictly needed */
  //unsigned char buff[sizeof(arg)];

  ser_tx(&arg, buff);
  desr_rx(buff);

}


  
