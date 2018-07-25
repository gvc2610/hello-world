#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "interface.h"

int main() {

printf("check size:%d\n",get_size());
struct polygon *poly = malloc(get_size());
int l =1, b=2, h=3;
set_params(poly,1,2,3);

printf("area:%d\n",get_area(poly));

char color[] = "white";
set_color(poly,color);

printf("color:%s\n",get_color(poly));

free(poly);

return 0;
}