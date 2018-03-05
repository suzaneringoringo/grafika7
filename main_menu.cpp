#include "clip.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fstream>
using namespace std;

// inisialisasi struct
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

// insialisasi variabel
char *fbp = 0;
point piv;
int pivX = 650;
int pivY = 350;
int layarx = 800;
int layary = 1200;

// inisialisasi daftar warna
color notSoBlack = {50,50,50,0};
color white = {255,255,255,0};

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
        //cout << (int)(*(fbp + position)) << endl;
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

int draw_line(point p1, point p2, color* c) {
	if (p2.x < p1.x) {
		int temp = p1.x;
		p1.x = p2.x;
		p2.x = temp;
		temp = p1.y;
		p1.y = p2.y;
		p2.y = temp;
	}

   int y = p1.y;
   int x = p1.x;
   int dy = p2.y - p1.y;
   int dx = p2.x - p1.x;


    //kasus vertikal
    if (p1.x == p2.x) {
        for (int y = min(p1.y, p2.y); y <= max(p1.y, p2.y); y++) {
            draw_dot(x,y,c);
        }
    }

    //kasus_horizontal
    else if (p1.y == p2.y) {
        for(int x = p1.x; x <= p2.x; x++) {
            draw_dot(x,y,c);
        }
    }

	//kasus miring
    else {
        float grad = (float)(p2.y - p1.y)/(float)(p2.x - p1.x);

        //gradien > 0
        if (grad > 0) {
                //gradien <= 1
                if (grad <= 1) {
                    int dxdy = p2.y - p1.y + p1.x - p2.x;
                    int F = p2.y - p1.y + p1.x - p2.x;
                    for (int x = p1.x; x <= p2.x; x++) {
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
                    int x = p1.x;
                    int dx = p2.x - p1.x;
                    int dxdy = p2.x - p1.x + p1.y - p2.y;
                    int F = p2.x - p1.x + p1.y - p2.y;
                    for (int y = p1.y; y <= p2.y; y++) {

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
                    int dy = p2.y - p1.y;
                    if (dy < 0) {
                        dy *= -1;
                    }
                    int dx = p2.x - p1.x;
                    int F = 2*dy - dx;
                    int y = p1.y;

			        for (x = p1.x; x <= p2.x; x++) {
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
                    int dx = p1.x - p2.x;
                    if (dx < 0) {
                        dx *= -1;
                    }
                    int dy = p1.y - p2.y;
                    int F = 2*dx - dy;
                    int x = p2.x;

			        for (y = p2.y; y <= p1.y; y++) {
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

void readFont(string pStr, int length, int first_y, int first_x) {
	//read char
	int charlength = 20;
	int charheight = 20;

	char **pixelmap = (char **)malloc(charheight * sizeof(char *));
	for (int i=0; i<charheight; i++)
		pixelmap[i] = (char *)malloc(charlength * sizeof(char));
	
	FILE *charmap;

	int current_y = first_y; //y untuk karakter sementara
	int current_x = first_x; //x untuk karakter sementara
	for (int i = 0; i < length; i++) {
		
		//baca map untuk pixel karakter
		if (pStr[i] == 'A') {
			charmap = fopen("font/A.txt", "r");
		} else  if (pStr[i] == 'B') {
			charmap = fopen("font/B.txt", "r");
		} else  if (pStr[i] == 'C') {
			charmap = fopen("font/C.txt", "r");
		} else  if (pStr[i] == 'D') {
			charmap = fopen("font/D.txt", "r");
		} else  if (pStr[i] == 'E') {
			charmap = fopen("font/E.txt", "r");
		} else  if (pStr[i] == 'F') {
			charmap = fopen("font/F.txt", "r");
		} else  if (pStr[i] == 'G') {
			charmap = fopen("font/G.txt", "r");
		} else  if (pStr[i] == 'H') {
			charmap = fopen("font/H.txt", "r");
		} else  if (pStr[i] == 'I') {
			charmap = fopen("font/I.txt", "r");
		} else  if (pStr[i] == 'J') {
			charmap = fopen("font/J.txt", "r");
		} else  if (pStr[i] == 'K') {
			charmap = fopen("font/K.txt", "r");
		} else  if (pStr[i] == 'L') {
			charmap = fopen("font/L.txt", "r");
		} else  if (pStr[i] == 'M') {
			charmap = fopen("font/M.txt", "r");
		} else  if (pStr[i] == 'N') {
			charmap = fopen("font/N.txt", "r");
		} else  if (pStr[i] == 'O') {
			charmap = fopen("font/O.txt", "r");
		} else  if (pStr[i] == 'P') {
			charmap = fopen("font/P.txt", "r");
		} else  if (pStr[i] == 'Q') {
			charmap = fopen("font/Q.txt", "r");
		} else  if (pStr[i] == 'R') {
			charmap = fopen("font/R.txt", "r");
		} else  if (pStr[i] == 'S') {
			charmap = fopen("font/S.txt", "r");
		} else  if (pStr[i] == 'T') {
			charmap = fopen("font/T.txt", "r");
		} else  if (pStr[i] == 'U') {
			charmap = fopen("font/U.txt", "r");
		} else  if (pStr[i] == 'V') {
			charmap = fopen("font/V.txt", "r");
		} else  if (pStr[i] == 'W') {
			charmap = fopen("font/W.txt", "r");
		} else  if (pStr[i] == 'X') {
			charmap = fopen("font/X.txt", "r");
		} else  if (pStr[i] == 'Y') {
			charmap = fopen("font/Y.txt", "r");
		} else  if (pStr[i] == 'Z') {
			charmap = fopen("font/Z.txt", "r");
		} else  if (pStr[i] == 'a') {
			charmap = fopen("font/a.txt", "r");
		} else  if (pStr[i] == 'b') {
			charmap = fopen("font/b.txt", "r");
		} else  if (pStr[i] == 'c') {
			charmap = fopen("font/c.txt", "r");
		} else  if (pStr[i] == 'd') {
			charmap = fopen("font/d.txt", "r");
		} else  if (pStr[i] == 'e') {
			charmap = fopen("font/e.txt", "r");
		} else  if (pStr[i] == 'f') {
			charmap = fopen("font/f.txt", "r");
		} else  if (pStr[i] == 'g') {
			charmap = fopen("font/g.txt", "r");
		} else  if (pStr[i] == 'h') {
			charmap = fopen("font/h.txt", "r");
		} else  if (pStr[i] == 'i') {
			charmap = fopen("font/i.txt", "r");
		} else  if (pStr[i] == 'j') {
			charmap = fopen("font/j.txt", "r");
		} else  if (pStr[i] == 'k') {
			charmap = fopen("font/k.txt", "r");
		} else  if (pStr[i] == 'l') {
			charmap = fopen("font/l.txt", "r");
		} else  if (pStr[i] == 'm') {
			charmap = fopen("font/m.txt", "r");
		} else  if (pStr[i] == 'n') {
			charmap = fopen("font/n.txt", "r");
		} else  if (pStr[i] == 'o') {
			charmap = fopen("font/o.txt", "r");
		} else  if (pStr[i] == 'p') {
			charmap = fopen("font/p.txt", "r");
		} else  if (pStr[i] == 'q') {
			charmap = fopen("font/q.txt", "r");
		} else  if (pStr[i] == 'r') {
			charmap = fopen("font/r.txt", "r");
		} else  if (pStr[i] == 's') {
			charmap = fopen("font/s.txt", "r");
		} else  if (pStr[i] == 't') {
			charmap = fopen("font/t.txt", "r");
		} else  if (pStr[i] == 'u') {
			charmap = fopen("font/u.txt", "r");
		} else  if (pStr[i] == 'v') {
			charmap = fopen("font/v.txt", "r");
		} else  if (pStr[i] == 'w') {
			charmap = fopen("font/w.txt", "r");
		} else  if (pStr[i] == 'x') {
			charmap = fopen("font/x.txt", "r");
		} else  if (pStr[i] == 'y') {
			charmap = fopen("font/y.txt", "r");
		} else  if (pStr[i] == 'z') {
			charmap = fopen("font/z.txt", "r");
		} else  if (pStr[i] == '1') {
			charmap = fopen("font/1.txt", "r");
		} else  if (pStr[i] == '2') {
			charmap = fopen("font/2.txt", "r");
		} else  if (pStr[i] == '3') {
			charmap = fopen("font/3.txt", "r");
		} else  if (pStr[i] == '4') {
			charmap = fopen("font/4.txt", "r");
		} else  if (pStr[i] == '5') {
			charmap = fopen("font/5.txt", "r");
		} else  if (pStr[i] == '6') {
			charmap = fopen("font/6.txt", "r");
		} else  if (pStr[i] == '7') {
			charmap = fopen("font/7.txt", "r");
		} else  if (pStr[i] == '8') {
			charmap = fopen("font/8.txt", "r");
		} else  if (pStr[i] == '9') {
			charmap = fopen("font/9.txt", "r");
		} else  if (pStr[i] == '0') {
			charmap = fopen("font/0.txt", "r");
		}else  if (pStr[i] == ' ') {
			charmap = fopen("font/spasi.txt", "r");
		} else if (pStr[i] == '\n') {
			current_y += 20;
			current_x = 100;
		} else if (pStr[i] == '[') {
			charmap = fopen("font/[.txt", "r");
		} else if (pStr[i] == ']') {
			charmap = fopen("font/].txt", "r");
		}
		
		for (int i = 0; i < charheight; i++) {
			fscanf (charmap, "%s", pixelmap[i]);
		}
		fclose;

		//menulis ke framebuffer
		int max_length = (int)(vinfo.xres);
		for (int y = current_y; y < current_y+charheight; y++) {
			for (int x = current_x; x < current_x+charlength; x++) {
				long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
						(y+vinfo.yoffset) * finfo.line_length;

				if (vinfo.bits_per_pixel == 32) {
					if (pixelmap[y-current_y][x-current_x] == '1') {
						*(fbp + location) = 255;        // putih
						*(fbp + location + 1) = 255;     // putih
						*(fbp + location + 2) = 255;    // putih
						*(fbp + location + 3) = 0;      // No transparency
					} else if (pixelmap[y-current_y][x-current_x] == '0') {
						*(fbp + location) = 0;        // hitam
						*(fbp + location + 1) = 0;     // hitam
						*(fbp + location + 2) = 0;    // hitam
						*(fbp + location + 3) = 0;      // No transparency
					}
				}
			}
		}
		if (current_x > max_length-first_x-charlength) {
			current_y += 20;
			current_x = 100;
		} else {
			current_x += 10;
		}
	}

}

void showMenu() {

	string menus[6] = {"MENU", "Tugas1", "Tugas2", "Tugas3", "Tugas4", "Tugas5"};

	int first_x = 100;
	int first_y = 100;
	readFont(menus[0], menus[0].length(), first_y + 20, first_x + 20);
	
	point p1, p2;
	for (int i = 1; i < 6; i++) {
		p1.x = first_x; p1.y = first_y;
		p2.x = first_x + 160; p2.y = p1.y;
		draw_line(p1, p2, &white);
		p1.x = p2.x; p1.y = p2.y + 60;
		draw_line(p2, p1, &white);
		p2.x = p1.x - 160; p2.y = p1.y;
		draw_line(p2, p1, &white);
		p1.x = p2.x; p1.y = p2.y - 60;
		draw_line(p2, p1, &white);
		readFont(menus[i], menus[i].length(), first_y + 20, first_x + 20);
		first_y += 100;
	}
}

int main () {

    int fbfd = 0;

	long int screensize = 0;

  	int x = 0, y = 0;

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
	if (atoi(fbp) == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	showMenu();
    return 0;
}
