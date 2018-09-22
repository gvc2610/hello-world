#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char *strdup1(char *src) {
int len = strlen(src);
if(len == 0)
return NULL;
char *ch = (char*)malloc(sizeof(char)*(len+1));
memcpy(ch,src,len);
ch[len] = '\0';

return ch;
}
int main(){
char ch1[] = "abcd"; 
//char *ch1 = “defg”;
char *ch2 = strdup1(ch1);
ch1[0]= 'x';
printf("ch1: %s, ch2: %s\n", ch1, ch2);

}