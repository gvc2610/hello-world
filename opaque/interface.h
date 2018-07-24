struct polygon;

int get_size(void);
int get_area(struct polygon *poly);
void set_params(struct polygon *poly, int l,int b,int h);
void set_color(struct polygon *poly, char *color);
char *get_color(struct polygon *poly);