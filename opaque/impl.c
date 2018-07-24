#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "interface.h"


struct polygon
{
	int l;
	int b;
	int h;
	char color[10];
};

int get_size() {
	return sizeof(struct polygon);
}

int get_area(struct polygon *poly) {
	return (poly->l)*(poly->b);
}

void set_params(struct polygon *poly, int l,int b,int h) {
    poly->l = l;
    poly->b = b;
    poly->h = h;
}

void set_color(struct polygon *poly, char *color) {
    int len = strlen(color);
    memcpy(&(poly->color),color,len);
    poly->color[len] = '\0';
}

char *get_color(struct polygon *poly) {
	return poly->color;
}