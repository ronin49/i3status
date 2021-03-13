#include <math.h>
#include <string.h>
struct color {
	unsigned char colors[3];
};
#define time int
struct color gradient(time startTime, struct color startColor, time endTime, struct color endColor, time nowTime) {
	double percent = 1.0*(nowTime-startTime)/(endTime-startTime);
	struct color result;
	for(int i = 0; i < 3; i++)
		result.colors[i] = round(1.0*startColor.colors[i] + percent*(endColor.colors[i]-startColor.colors[i]));
	return result;
}
struct color rgb(unsigned char r, unsigned char g, unsigned char b) {
	struct color c;
	c.colors[0] = r;
	c.colors[1] = g;
	c.colors[2] = b;
	return c;
}
#define RED rgb(255,0,0)
#define YELLOW rgb(255,255,0)
#define GREEN rgb(0,255,0)
#define BLACK rgb(0,0,0)
#define WHITE rgb(255,255,255)
char* hex(struct color c) {
	char buf[256];
	int j = 0;
	for(int i = 0; i < 3; i++) {
		if(c.colors[i] < 16) j+=sprintf(buf+j,"0");
		j += sprintf(buf+j,"%x", c.colors[i]);
	}
	buf[j]='\0';
	char * str = (char*)malloc(j+1);
	strcpy(str,buf);
	return str;
}
#undef time
