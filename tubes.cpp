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
#include <unistd.h>
#include <linux/input.h>
using namespace std;

#define MOUSEFILE "/dev/input/mice\0"

// inisialisasi struct
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
struct input_event ie;

// inisialisasi variabel
vector<vector<point> > listPoint_bangunan;
vector<vector<point> > listPoint_jalan;
vector<pair<point,char> > colorTupleListBangunan;
vector<pair <point, char> > colorTupleListJalanan;
vector<viewport> viewTembus;
vector<viewport> viewHid;

typedef struct {
    point startPoint;
    char direction; // wasd
    int size;
    vector<color> resColor;
} memList;
vector<memList> pointerMemList;
point p1; // p1 adalah pivot
int layarx = 800;
int layary = 600;
char *fbp = 0;
int fd;
int selected1 = 0;
int selected2 = 0;
bool mouseOn = true;
bool redrawMap = true;

// inisialisasi daftar warna
color white = { 255, 255, 255, 0 };
color black = {	0, 0, 0, 0 };
color notSoBlack = {50,50,50,0};
color green = {	0, 255, 0, 0 };
color blue = { 0, 0, 255, 0 };
color red = {255, 0, 0, 0};

point piv;
int pivX = 650;
int pivY = 350;
int r = 30;

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
		} else {
			charmap = fopen("font/kotak.txt", "r");
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

void draw_multi_dot(int pivX, int pivY, int curX, int curY, viewport v){
    point cur;

    cur.x = pivX + curX;
    cur.y = pivY + curY;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&green);

    cur.x = pivX - curX;
    cur.y = pivY + curY;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&white);
    
    cur.x = pivX + curX;
    cur.y = pivY - curY;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&white);

    cur.x = pivX - curX;
    cur.y = pivY - curY;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&white);
    
    cur.x = pivX - curY;
    cur.y = pivY - curX;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&white);
    
    cur.x = pivX + curY;
    cur.y = pivY - curX;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&white);
    
    cur.x = pivX - curY;
    cur.y = pivY + curX;
    if(pointPos(v,cur) == 0)
        draw_dot(cur.x, cur.y,&white);
    
    cur.x = pivX + curY;
    cur.y = pivY + curX;
    if(pointPos(v,cur) == 0)
       draw_dot(cur.x, cur.y,&white);
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

void redraw(vector<memList> &listEntry) {
    for (int j = 0; j < listEntry.size(); j++) {
        memList entry = listEntry[j];
        int addx = 0, addy = 0;
        for (int i = 0; i < entry.size; i++) {
            long int position = (entry.startPoint.x + addx + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (entry.startPoint.y + addy + vinfo.yoffset) * finfo.line_length;
            color temp = entry.resColor[i];
            *(fbp + position) = temp.b;
            *(fbp + position + 1) = temp.g;
            *(fbp + position + 2) = temp.r;
            
            if (entry.direction == 'w') {
                addy--;
            } else if (entry.direction == 's') {
                addy++;
            } else if (entry.direction == 'a') {
                addx--;
            } else if (entry.direction == 'd') {
                addx++;
            } 
        }
    }
}

void redraw(point center) {
    // garis lurus keatas
    for (int i = 0; i < 3; i++) {
        point temp = {center.x+i -1, center.y-10};
        point dest = {center.x + i - 1, center.y + 10};
        draw_line(temp,dest,&notSoBlack);
    }
    for (int i = 0; i < 3; i++) {
        point temp = {center.x - 10, center.y+i-1};
        point dest = {center.x + 10, center.y+i-1};
        draw_line(temp,dest,&notSoBlack);
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

void insertToVectorBangunan(vector<point> &insertedVector, string nama_file, point shift) {
	
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
        colorTupleListBangunan.push_back(tempPair);
    }
	fclose;
}

void insertToVectorJalanan(vector<point> &insertedVector, string nama_file, point shift) {
	
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
        colorTupleListJalanan.push_back(tempPair);
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
        insertToVectorBangunan(temp,namaFile,shift);
        listPoint_bangunan.push_back(temp);
	}
    charmap.close();

    charmap.open("listPolygon2.txt");
	charmap >> jumlah_bidang;
	for (int k = 0; k < jumlah_bidang; k++) {
		int x,y;
        string namaFile;
		charmap >> namaFile;
		vector<point> temp;
        insertToVectorJalanan(temp,namaFile,shift);
        listPoint_jalan.push_back(temp);
	}
    charmap.close();
}

void readMouseInput(point &result, int &terminate) {
    unsigned char *ptr = (unsigned char*)&ie;
    
    unsigned char button,bLeft,bMiddle,bRight;
    char x,y;                                                            // the relX , relY datas
    int absolute_x,absolute_y;
    while (1) {
        if(read(fd, &ie, sizeof(struct input_event))!=-1) {
            
            //redraw(result);
            button=ptr[0];
            bLeft = button & 0x1;
            bMiddle = ( button & 0x4 ) > 0;
            bRight = ( button & 0x2 ) > 0;
            x=(char) ptr[1];y=(char) ptr[2];
            if (bLeft ==1) {
                // check coordinate range
                int hasil = -1;
                int traversal = 0;
                while ((hasil == -1) && (traversal < viewTembus.size())) {
                    if (pointPos(viewTembus[traversal],result) == 0) {
                        hasil = traversal;
                    } else {
                        traversal++;
                    }
                }
                // warnain viewport ke result;
                if (hasil == 0) {
                    selected1 = !selected1;
                }
                if (hasil == 1) {
                    selected2 = !selected2;
                }
                redrawMap = true;

            } else if (bRight == 1) {
                mouseOn = false;
                
            }
            // computes absolute x,y coordinates
            result.x +=x;
            result.y -=y;
            // set absolute reference ?
            // pengaman
            if (result.x < 15) {
                result.x = 15;
            }
            if (result.x > 785) {
                result.x = 785;
            }
            if (result.y < 15) {
                result.y = 15;
            }
            if (result.y > 585) {
                result.y = 585;
            }
            break;
        }
    }
}

void drawPointer(point center) {
    // garis lurus keatas
    for (int i = 0; i < 3; i++) {
        point temp = {center.x+i -1, center.y-10};
        point dest = {center.x + i - 1, center.y + 10};
        draw_line(temp,dest,&white);
    }
    for (int i = 0; i < 3; i++) {
        point temp = {center.x - 10, center.y+i-1};
        point dest = {center.x + 10, center.y+i-1};
        draw_line(temp,dest,&white);
    }
}

void mouseMovement(vector<viewport> viewHid, vector<viewport> viewTembus, point &center, int &terminate) {
    int result = 1;

    for (int i = 0; i < viewHid.size(); i++) {
        if (pointerPos(viewHid[i], p1) == 0) {
            result = 0;
            break;
        }
    }
    if (result != 0) {
        drawPointer(p1);
    }
    point tempP = {p1.x, p1.y};
    
    readMouseInput(p1, terminate);
    // cout << terminate;
    result = 1;

    for (int i = 0; i < viewHid.size(); i++) {
        if (pointerPos(viewHid[i], tempP) == 0) {
            result = 0;
            break;
        }
    }
    if (result != 0) {
        redraw(tempP);
    }
    
    // redraw viewport yang ketimpa padahal hrsnya enggak
    for (int i = 0; i < viewTembus.size(); i++) {
        
        clear_screen(viewTembus[i].xmin, viewTembus[i].ymin, viewTembus[i].xmax+1, viewTembus[i].ymax+1, &black);
        
        draw_line(viewTembus[i].p1, viewTembus[i].p2, &white);
        draw_line(viewTembus[i].p2, viewTembus[i].p3, &white);
        draw_line(viewTembus[i].p3, viewTembus[i].p4, &white);
        draw_line(viewTembus[i].p4, viewTembus[i].p1, &white);
        if (selected1) {
            fil(111,555,0,green,black);
        }
        if (selected2) {
            fil(450, 555, 0, green, black);
        }
    }

}

void moveViewport(int& terminate) {
    system ("/bin/stty raw -echo");
    char cin = ' ';
    do {
        cin = getchar();
        
    } while ((cin != 'e') && (cin != 'w') && (cin != 'q') && (cin != 'a') && (cin != 's') && (cin != 'd'));
    system ("/bin/stty cooked echo");
    
    if (cin == 'q') {
        terminate = 1;
    } else if (cin == 'e') {
        mouseOn = true;
    } else {
        for (int ite = 0; ite < listPoint_bangunan.size(); ite++) {
            translasiBanyak(listPoint_bangunan[ite],cin,10);
        }

        for (int ite = 0; ite < listPoint_jalan.size(); ite++) {
            translasiBanyak(listPoint_jalan[ite],cin,10);
        }

        for (int ite = 0; ite < colorTupleListBangunan.size(); ite++) {
            translasi((colorTupleListBangunan[ite].first),cin,10);
        }      

        for (int ite = 0; ite < colorTupleListJalanan.size(); ite++) {
            translasi((colorTupleListJalanan[ite].first),cin,10);
        }

        translasi(p1,cin,10);
        
    }
}



int main () {
    piv.x=650;
    piv.y=350;

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

 // initialize mouse
        if((fd = open(MOUSEFILE, O_RDONLY | O_NONBLOCK )) == -1)
    {
        printf("NonBlocking %s open ERROR\n",MOUSEFILE);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("NonBlocking %s open OK\n",MOUSEFILE);
    }

	clear_screen(0,0,800, 600, &notSoBlack);
	addListPoint("listPolygon.txt",p1);

    // initialize viewport
    // urutan c[] tidak boleh diubah -> urutan algo sutherland
    point c[] = {{100,50}, {700,50}, {700,450}, {100,450}};
    char clen = 4;

    viewport view;
    view.p1 = c[0];
    view.p2 = c[1];
    view.p3 = c[2];
    view.p4 = c[3];
    initialize(&view);
    viewHid.push_back(view);
  
    int terminate = 0;

    point cx[] = {{100,550}, {200,550}, {200,570}, {100,570}};
    point cp[] = {{400, 550}, {500, 550}, {500,570}, {400,570}};

    viewport viewx;
    viewx.p1 = cx[0];
    viewx.p2 = cx[1];
    viewx.p3 = cx[2];
    viewx.p4 = cx[3];
    initialize(&viewx);
    viewTembus.push_back(viewx);
    
    viewport viewp;
    viewp.p1 = cp[0];
    viewp.p2 = cp[1];
    viewp.p3 = cp[2];
    viewp.p4 = cp[3];
    initialize(&viewp);
    viewTembus.push_back(viewp);
    // SETUP DRAW CIRCLE
    
    pivX=piv.x;
    int curX = 0;
    int curY = r;
    
    int Fe = (pivX+1)*(pivX+1) + (pivY)*(pivY) - r*r;
    int Fse = (pivX+1)*(pivX+1) + (pivY-1)*(pivY-1) - r*r;
    int d = 3 - (2 * r);
    
    // END SETUP DRAW CIRCLE
    

    selected1 = 0;
    selected2 = 0;
    while (!terminate) {
        if (redrawMap) {
            clear_screen(view.xmin,view.ymin,view.xmax+1,view.ymax+1,&black);
            draw_line(c[0], c[1],&white);
            draw_line(c[1], c[2],&white);
            draw_line(c[2], c[3],&white);
            draw_line(c[3], c[0],&white);
            
            
            // --------------------------- Start Clip Plane ---------------------------
            poly_t clipper = {clen, 0, c};

            
            if(selected1) {
                for (int listPolygonIte = 0; listPolygonIte < listPoint_bangunan.size(); listPolygonIte++) {
                    poly_t subject = {listPoint_bangunan[listPolygonIte].size(), 0, &listPoint_bangunan[listPolygonIte][0]};
                    poly res = poly_clip(&subject, &clipper);
                    if(res->len > 0){
                        for (int i = 0; i < res->len -1; i++) {
                            draw_line(res->v[i], res->v[i+1], &white);
                        }
                        draw_line(res->v[res->len -1], res->v[0], &white);
                    }
                }

                for (int i = 0; i < colorTupleListBangunan.size(); i++) {
                    if (pointPos(view,colorTupleListBangunan[i].first) == 0) {
                        fillPolygon(colorTupleListBangunan[i],black);
                    }
                }
            }

            if (selected2) {
                for (int listPolygonIte = 0; listPolygonIte < listPoint_jalan.size(); listPolygonIte++) {
                    poly_t subject = {listPoint_jalan[listPolygonIte].size(), 0, &listPoint_jalan[listPolygonIte][0]};
                    poly res = poly_clip(&subject, &clipper);
                    if(res->len > 0){
                        for (int i = 0; i < res->len -1; i++) {
                            draw_line(res->v[i], res->v[i+1], &white);
                        }
                        draw_line(res->v[res->len -1], res->v[0], &white);
                    }
                }

                for (int i = 0; i < colorTupleListJalanan.size(); i++) {
                    if (pointPos(view,colorTupleListJalanan[i].first) == 0) {
                        fillPolygon(colorTupleListJalanan[i],black);
                    }
                }
            }

        
            // Bagian pewarnaan
            
            // Akhir pewarnaan
            
            // DRAW CIRCLE
            draw_multi_dot(pivX,pivY,curX,curY,view);

            while(curX <= curY){
                if(d > 0){
                    curY--;
                    d = d + 4*(curX-curY) + 10;
                }else{
                    d = d + 4*curX + 6;
                }
                curX++;
                draw_multi_dot(pivX,pivY,curX,curY,view);
                
            }

            d = 3 - (2 * r);
            curX = 0;
            curY = r;
            // END DRAW CIRCLE
        }
        if (mouseOn) {
            redrawMap = false;
            mouseMovement(viewHid,viewTembus,p1,terminate);
            
        } else { // keyboard is on
            redrawMap = true;
            moveViewport(terminate);
            
        }   
	}

    return 0;
}
