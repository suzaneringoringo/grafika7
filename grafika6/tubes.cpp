#include "rotasi.h"
#include "scaling.h"
#include "point_warna.h"
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

// inisialisasi variabel
vector<vector<point> > listPoint;
vector<pair<point,char> > colorTupleList;
int layarx = 800;
int layary = 600;
char *fbp = 0;

// inisialisasi daftar warna
color white = { 255, 255, 255, 0 };
color black = {	0, 0, 0, 0 };
color notSoBlack = {50,50,50,0};
color green = {	0, 255, 0, 0 };
color blue = { 0, 0, 255, 0 };

float degreeToRad(float degree) {
    return (degree * M_PI / 180);
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

void clear_screen(int xx, int yy, int width, int height, color *desired) {
    for(int x=xx; x<width; x++) {
        for(int y=yy; y<height; y++) {
            long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
               (y + vinfo.yoffset) * finfo.line_length;
            *(fbp + position) = desired->b;
            *(fbp + position + 1) = desired->g;
            *(fbp + position + 2) = desired->r;
            *(fbp + position + 3) = desired->a;
        }
    }
}

void fil(int x,int y,char prev, color &desired, color &replaced){
    char tm = 255;
	long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
	if ((x<1)||(y<1)||(x>1366)||(y>700) || ( ((*(fbp + position) ^ replaced.b) != 0) ||  ((*(fbp + position + 1) ^ replaced.g) != 0) || ((*(fbp + position + 2) ^ replaced.r) != 0))) {
		return;
	} else {
		draw_dot(x,y,&desired);
        // ignore 0, up 1, down 2, left 3, right 4, berdasarkan arah layar
		if (prev != 3){
            fil(x+1,y,4,desired,replaced);
        }
        if (prev != 4)  {
		    fil(x-1,y,3,desired,replaced);
        }
        if (prev != 1) {
		    fil(x,y-1,2,desired,replaced);
        }
        if (prev != 2)  {
		    fil(x,y+1,1,desired,replaced);	
        }
	}
}

void insertToVector(vector<point> &insertedVector, string nama_file, point shift) {
	
	FILE* charmap;

	charmap = fopen(nama_file.c_str(), "r");

	int jumlah_loop;
	point tempCharPoint;
		
	int i = 0;
	int jumlah_titik;
	fscanf(charmap, "%d", &jumlah_titik);
	for (int k = 0; k < jumlah_titik; k++) {
		int x,y;
		fscanf(charmap, "%d  %d", &x, &y);
		tempCharPoint.x = x+shift.x;
		tempCharPoint.y = y+shift.y;
		insertedVector.push_back(tempCharPoint);
	}
    insertedVector.push_back(insertedVector[0]);
	int jumlah_loop_warna;
	int xx,yy, numColour;
    char c;
    fscanf(charmap, "%d", &numColour);
    for (int i = 0; i < numColour; i++) {
        fscanf(charmap, "%d %d %c", &xx, &yy , &c);
        point tempPoint;
        tempPoint.x = xx + shift.x;
        tempPoint.y = yy + shift.y;
        pair<point, char> tempPair(tempPoint,c);
        colorTupleList.push_back(tempPair);
    }
	fclose;
}

void fillPolygon(pair<point,char> p, color &replaced) {
    if (p.second == 'g') {
        fil(p.first.x,p.first.y,0,green,replaced);
    } else if (p.second == 'w') {
        fil(p.first.x,p.first.y,0,white,replaced);
    } else if (p.second == 'b') {
        fil(p.first.x,p.first.y,0,blue,replaced);
    }
}

void addListPoint(string listPointFileName, point shift){
	int jumlah_bidang;
    ifstream charmap;
    charmap.open("listPolygon.txt");
	charmap >> jumlah_bidang;
	for (int k = 0; k < jumlah_bidang; k++) {
		int x,y;
        string namaFile;
		charmap >> namaFile;
		vector<point> temp;
        insertToVector(temp,namaFile,shift);
        listPoint.push_back(temp);
	}
}

void moveViewport(int& terminate) {
    system ("/bin/stty raw -echo");
    char cin = ' ';
    do {
        cin = getchar();
        
    } while ((cin != 'w') && (cin != 'q') && (cin != 'a') && (cin != 's') && (cin != 'd'));
    system ("/bin/stty cooked echo");
    
    if (cin == 'q') {
        terminate = 1;
    } else {
        for (int ite = 0; ite < listPoint.size(); ite++) {
            translasiBanyak(listPoint[ite],cin,10);
        }
        for (int ite = 0; ite < colorTupleList.size(); ite++) {
            translasi((colorTupleList[ite].first),cin,10);
        }
        
    }
}

void drawCircle(int r, point pivot, vector<point> &result) {
    // ambil titik atas
    point temp = {pivot.x, pivot.y - r};
    result.push_back(temp);
    for (int i = 0; i < 72; i++) {
        temp = rotasi(pivot,temp,degreeToRad(5));
        result.push_back(temp);
    }
}

int main () {
    point p1; // p1 adalah pivot
    p1.x = 650;
    p1.y = 350;

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

	clear_screen(0,0,800, 600, &notSoBlack);
	addListPoint("listPolygon.txt",p1);

    // initialize viewport
    // urutan c[] tidak boleh diubah -> urutan algo sutherland
    point c[] = {{100,100}, {700,100}, {700,500}, {100,500}};
    char clen = 4;

    viewport view;
    view.p1 = c[0];
    view.p2 = c[1];
    view.p3 = c[2];
    view.p4 = c[3];
    initialize(&view);

    point pusatLingkaran = {650, 350};
    vector<point> resultCircle;
    drawCircle(30,pusatLingkaran,resultCircle);
    listPoint.push_back(resultCircle);

    
    int terminate = 0;
    while (!terminate) {
        clear_screen(view.xmin,view.ymin,view.xmax+1,view.ymax+1,&black);
		draw_line(c[0], c[1],&white);
        draw_line(c[1], c[2],&white);
        draw_line(c[2], c[3],&white);
        draw_line(c[3], c[0],&white);
        
        // --------------------------- Start Clip Plane ---------------------------
        poly_t clipper = {clen, 0, c};

        for (int listPolygonIte = 0; listPolygonIte < listPoint.size(); listPolygonIte++) {
            poly_t subject = {listPoint[listPolygonIte].size(), 0, &listPoint[listPolygonIte][0]};
            poly res = poly_clip(&subject, &clipper);
            if(res->len > 0){
                for (int i = 0; i < res->len -1; i++) {
                    draw_line(res->v[i], res->v[i+1], &white);
                }
                draw_line(res->v[res->len -1], res->v[0], &white);
            }
        }
        
        // Bagian pewarnaan
        for (int i = 0; i < colorTupleList.size(); i++) {
            if (pointPos(view,colorTupleList[i].first) == 0) {
                fillPolygon(colorTupleList[i],black);
            }
        }
        // Akhir pewarnaan

        moveViewport(terminate);
	}
    return 0;
}

