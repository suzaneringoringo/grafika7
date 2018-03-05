#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

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

void draw_dot(int x, int y, color* c)
{
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
    {//assume 16 bit color
        int b = c->b;
        int g = c->g;
        int r = c->r;
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + position)) = t;
    }
}

int draw_line(int x1, int y1, int x2, int y2, color* c) {
   int grad = (y2-y1)*(x2-x1);
	 int y = y1;
	 int dy = y2 - y1;
   if (grad > 0) {
    int dxdy = y2 - y1 + x1 - x2;
    int F = y2 - y1 + x1 - x2;
    for (int x = x1; x <= x2; x++) {
        draw_dot(x, y, c);
        if (F < 0) {
            F += dy;
        } else {
            y++;
            F += dxdy;
        }
    }
   } else {
        int dxdy = y2 - y1 + x1 - x2;
        int F = y2 - y1 + x1 - x2;
        for (int x = x1; x <= x2; x++) {
            draw_dot(x, y, c);
            if (F > 0) {
                F += dy;
            } else {
                y--;
                F += dxdy;
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

	//baca map untuk pixel karakter
	charmap = fopen("pesawat.txt", "r");
	for (i = 0; i < charheight; i++) {
			fscanf (charmap, "%s", pixelmap[i]);
		}
	fclose;

	// Figure out where in memory to put the pixel
	int first_y_pesawat = 100; //y awal;
	int first_x_pesawat = (int)(vinfo.xres)-200;
	int first_y_blok1 = (int)(vinfo.yres)-200;
	int first_x_blok1 = (int)(vinfo.xres)/2;

	//BLOK KIRI
	int kiri_y_blok1 = (int)(vinfo.yres)-200;
	int kiri_x_blok1 = (int)(vinfo.xres)/2;

	//BLOK KANAN
	int kanan_y_blok1 = (int)(vinfo.yres)-200;
	int kanan_x_blok1 = (int)(vinfo.xres)/2;

	int height_blok1 = 80;
	int width_blok1 = 20;
	int current_y_pesawat = first_y_pesawat; //y untuk karakter sementara
	int current_x_pesawat = first_x_pesawat; //x untuk karakter sementara
	int current_y_blok1 = first_y_blok1;
	int current_x_blok1 = first_x_blok1;
	while(1) {
		//menghitamkan layar
		for (y = 0; y < 760; y++) {
			for (x = 0; x < vinfo.xres; x++) {
				location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
						(y+vinfo.yoffset) * finfo.line_length;

				// if ((y >= (current_y_blok1 - height_blok1)) || (y <= (current_y_blok1 + height_blok1))) {
				// 	if ((x >= (current_x_blok1 - width_blok1)) || ((x <= (current_x_blok1 + width_blok1)))) {
				// 		*(fbp + location) = 0;        // kuning
				// 		*(fbp + location + 1) = 255;     // kuning
				// 		*(fbp + location + 2) = 255;    // kuning
				// 		*(fbp + location + 3) = 0;      // No transparency
				// 	}
				// } else {

					*(fbp + location) = 0;        // hitam
					*(fbp + location + 1) = 0;     // hitam
					*(fbp + location + 2) = 0;    // hitam
					*(fbp + location + 3) = 0;      // No transparency
				//}
			}
		}

		// 	BLOK KIRI
		for(int ii = 0; ii < 20; ii++) {
			draw_line(kiri_x_blok1+ii-100, kiri_y_blok1, kiri_x_blok1+ii, kiri_y_blok1+200, &green);
		}
		if(kiri_y_blok1 < vinfo.yres){
			kiri_x_blok1 -= 30;
			kiri_y_blok1 -= 60;
		}else{
			kiri_x_blok1 = (int)(vinfo.xres)/2;
			kiri_y_blok1 = (int)(vinfo.yres)-200;
		}

		// 	BLOK KANAN
		for(int ii = 0; ii < 10; ii++) {
			draw_line(kanan_x_blok1+ii, kanan_y_blok1+100, kanan_x_blok1+ii+100, kanan_y_blok1-100, &white);
		}
		if(kanan_y_blok1 < vinfo.yres){
			kanan_x_blok1 += 30;
			kanan_y_blok1 -= 60;
		}else{
			kanan_y_blok1 = (int)(vinfo.yres)-200;
			kanan_x_blok1 = (int)(vinfo.xres)/2;
		}

		//menulis pesawat ke framebuffer
		int max_length = (int)(vinfo.xres);
		for (y = current_y_pesawat; y < current_y_pesawat+charheight; y++) {
			for (x = current_x_pesawat; x < current_x_pesawat+charlength; x++) {
				location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
						(y+vinfo.yoffset) * finfo.line_length;

				if (vinfo.bits_per_pixel == 32) {
					if (pixelmap[y-current_y_pesawat][x-current_x_pesawat] == '*') {
						*(fbp + location) = 255;        // putih
						*(fbp + location + 1) = 255;     // putih
						*(fbp + location + 2) = 255;    // putih
						*(fbp + location + 3) = 0;      // No transparency
					}
				}
			}
		}

		//menggambar blok1
		for (y = current_y_blok1; y < current_y_blok1+height_blok1; y++) {
			if (y >= 0) {
				for (x = current_x_blok1; x < current_x_blok1+width_blok1; x++) {
					location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
							(y+vinfo.yoffset) * finfo.line_length;
					if (vinfo.bits_per_pixel == 32) {
						*(fbp + location) = 0;        // putih
						*(fbp + location + 1) = 255;     // putih
						*(fbp + location + 2) = 255;    // putih
						*(fbp + location + 3) = 0;      // No transparency
					}
				}
			}
		}
		current_x_pesawat -= 60;
		if(current_y_blok1 < vinfo.yres){
			current_y_blok1 -= 60;
		}else{
			current_y_blok1 = (int)(vinfo.yres)-200;
		}
		usleep(500000);
	}

	munmap(fbp, screensize);

	close(fbfd);

	return 0;

}
