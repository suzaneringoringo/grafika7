#include <linux/fb.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

uint32_t pixel_color(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo)
{
	return (r<<vinfo->red.offset) | (g<<vinfo->green.offset) | (b<<vinfo->blue.offset);
}

void coloring_pixel(uint8_t *fbp, char* file, int x, int y, uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo){
	FILE* f = fopen(file, "r");
    char* s = (char*) malloc(256);
    while (fscanf(f, "%s", s) != EOF ) {
        int i;
        for (i = 0; i < strlen(s); i++)
            if (s[i] == '1'){
                pixel_color(r, g, b, vinfo);
            	long location = (i+x+vinfo->xoffset) * (vinfo->bits_per_pixel/8) + (y+vinfo->yoffset) * finfo->line_length;
				*((uint32_t*)(fbp + location)) = pixel_color(r,g,b,vinfo);
            }
        y++;    
    }
    free(s);
    fclose(f);
}

int main()
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	int x = 0;
	int y = 0;

	int fb_fd = open("/dev/fb0",O_RDWR);

	//Get variable screen information
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	vinfo.grayscale=0;
	vinfo.bits_per_pixel=32;
	ioctl(fb_fd, FBIOPUT_VSCREENINFO, &vinfo);
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

	ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

	long screensize = vinfo.yres_virtual * finfo.line_length;

	uint8_t *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t) 0);
	uint8_t *bbp = (uint8_t*) malloc(vinfo.yres_virtual * finfo.line_length);

	char text[62];
	gets(text);

	int cy = 0;
	while(1){
		for (x=0;x<vinfo.xres;x++)
			for (y=0;y<vinfo.yres;y++){
				long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
				*((uint32_t*)(bbp + location)) = pixel_color(0x00,0x00,0x00, &vinfo);
			}

		for(int i = 0; i < strlen(text); i++){
			char* file = (char*) malloc(256);
            sprintf(file, "data/%c.txt", text[i]);
            coloring_pixel(bbp, file, 22*i, cy, 0xFF, 0x00, 0xFF, &vinfo, &finfo);
            free(file);
		}

		memcpy(fbp, bbp, vinfo.yres_virtual * finfo.line_length);
	
		usleep(10);

	}

	return 0;
}