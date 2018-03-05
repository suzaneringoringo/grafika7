#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <iostream>

typedef struct{
    int r,g,b,a;
} color;

// INISIALISASI VARIABEL
int layarx = 1366;
int layary = 700;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;

color white = {
			 255,
			 255,
			 255,
			 0
	 };

color green = {
	0,255,255,0
};

int min(int y1, int y2){
	if (y1 < y2) {
		return y1;
	} else {
		return y2;
	}
}

int max(int y1, int y2) {
	if (y2 < y1) {
		return y1;
	} else {
		return y2;
	}
}

void draw_dot(int x, int y, color* c) {
	if((x<1) || (x>layarx) || (y<1) || (y>layary)){
		return ;
	}
    long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
       (y + vinfo.yoffset) * finfo.line_length;
    if(vinfo.bits_per_pixel == 32){
        *(fbp + position) = c->b;
        *(fbp + position + 1) = c->g;
        *(fbp + position + 2) = c->r;
        *(fbp + position + 3) = c->a;
    }
    else
    { //assume 16 bit color
        int b = c->b;
        int g = c->g;
        int r = c->r;
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + position)) = t;
    }
}

int draw_line(int x1, int y1, int x2, int y2, color* c) {
	if (x2 < x1) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

   int y = y1;
   int x = x1;
   int dy = y2 - y1;
   int dx = x2 - x1;


    //kasus vertikal 
    if (x1 == x2) {
        for (int y = min(y1, y2); y <= max(y1, y2); y++) {
            draw_dot(x,y,c);
        }
    }

    //kasus_horizontal 
    else if (y1 == y2) {
        for(int x = x1; x <= x2; x++) {
            draw_dot(x,y,c);
        }
    }

	//kasus miring
    else {
        float grad = (float)(y2-y1)/(float)(x2-x1);

        //gradien > 0
        if (grad > 0) {
                //gradien <= 1
                if (grad <= 1) {       
                    int dxdy = y2 - y1 + x1 - x2;
                    int F = y2 - y1 + x1 - x2; 
                    for (int x = x1; x <= x2; x++) {
                        draw_dot(x,y,c);
                        if (F < 0) {
                            F += dy;
                        } else {
                            y++;
                            F += dxdy;
                        }
                    }
                }

                //gradien > 1
                else {
                    int x = x1;
                    int dx = x2 -x1;
                    int dxdy = x2 - x1 + y1 - y2;
                    int F = x2 - x1 + y1 - y2;
                    for (int y = y1; y <= y2; y++) {
                        
                        draw_dot(x,y,c);
                        if (F < 0) {
                            F += dx;
                        } else {
                            x++;
                            F += dxdy;
                        }
                    }
                }
		//gradien < 0
        } else {
                //gradien >= -1
                if (grad >= -1) {
                    int dy = y2 - y1;
                    if (dy < 0) {
                        dy *= -1;
                    }
                    int dx = x2 - x1;
                    int F = 2*dy - dx;
                    int y = y1;

			        for (x = x1; x <= x2; x++) {
                        draw_dot(x,y,c);
                        if (F > 0) {
                            y--;
                            F = F - 2*dx;
                        } 
                        F = F + 2*dy;
			        }
                }
                //gradien < -1
                else {
                    int dx = x1 - x2;
                    if (dx < 0) {
                        dx *= -1;
                    }
                    int dy = y1 - y2;
                    int F = 2*dx - dy;
                    int x = x2;

			        for (y = y2; y <= y1; y++) {
                        draw_dot(x,y,c);
                        if (F > 0) {
                            x--;
                            F = F - 2*dy;
                        } 
                        F = F + 2*dx;
			        } 
                }
        }
   }	
}


void clear_screen(int width, int height)
{
    int x = 0;
    int y = 0;

    for(x=0; x<width; x++)
    {
        for(y=0; y<height; y++)
        {
            long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
               (y + vinfo.yoffset) * finfo.line_length;
            *(fbp + position) = 0;
            *(fbp + position + 1) = 0;
            *(fbp + position + 2) = 0;
            *(fbp + position + 3) = 0;
        }
    }
}

void draw_multi_dot(int pivX, int pivY, int curX, int curY){
	draw_dot(pivX + curX, pivY + curY,&green);
	draw_dot(pivX - curX, pivY + curY,&green);
    draw_dot(pivX + curX, pivY - curY,&green);
    draw_dot(pivX - curX, pivY - curY,&green);
    draw_dot(pivX - curY, pivY - curX,&green);
    draw_dot(pivX + curY, pivY - curX,&green);
    draw_dot(pivX - curY, pivY + curX,&green);
    draw_dot(pivX + curY, pivY + curX,&green);
}

int main() {
	int fbfd = 0;

	long int screensize = 0;

  	int x = 0, y = 0;
  	long int location = 0;

  	// Open the file for reading and writing
  	fbfd = open("/dev/fb0", O_RDWR);
  	if (fbfd == -1) {
  		perror("Error: cannot open framebuffer device");
		exit(1);
	}

	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
			fbfd, 0);
	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	//read char
	int charlength = 120;
	int charheight = 60;
	int i;

	char **pixelmap = (char **)malloc(charheight * sizeof(char *));
	for (i=0; i<charheight; i++)
		pixelmap[i] = (char *)malloc(charlength * sizeof(char));

	FILE *charmap;

    // SETUP DRAW CIRCLE
    int pivX = (int)(vinfo.xres)/2;
    int pivY = (int)(vinfo.yres)-400;
    int r = 150;

    int curX = 0;
    int curY = r;
    
    int Fe = (pivX+1)*(pivX+1) + (pivY)*(pivY) - r*r;
    int Fse = (pivX+1)*(pivX+1) + (pivY-1)*(pivY-1) - r*r;
    int d = Fe + Fse;
    // END SETUP DRAW CIRCLE

    while(1) {
        //menghitamkan layar
        for (y = 0; y < 760; y++) {
            for (x = 0; x < vinfo.xres; x++) {
                location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset) * finfo.line_length;
                    *(fbp + location) = 0;        // hitam
                    *(fbp + location + 1) = 0;     // hitam
                    *(fbp + location + 2) = 0;    // hitam
                    *(fbp + location + 3) = 0;      // No transparency
            }
        }

	    // DRAW CIRCLE
        draw_multi_dot(pivX,pivY,curX,curY);
        
        while(curX <= curY){
            if(d > 0){
                curY--;
                d = d + 4*(curX-curY) + 10;
            }else{
                d = d + 4*curX + 6;
            }
            curX++;
            draw_multi_dot(pivX,pivY,curX,curY);
            std::cout << curX << "," << curY << "\n";
        }

        int d = Fe + Fse;
        curX = 0;
        curY = r;
        // END DRAW CIRCLE
	}

	munmap(fbp, screensize);

	close(fbfd);

	return 0;

}
