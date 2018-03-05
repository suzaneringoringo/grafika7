#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "clip.h"
#include "point_warna.h"

//Mengecek posisi titik terhadap viewport. Jika pos = 0 maka titik berada dalam viewport 
int pointPos(viewport view, point p){
	int pos = 0;
	if (p.x < view.xmin){
		pos += 8;
	} else if (p.x > view.xmax){
		pos += 4;
	}
	
	if (p.y < view.ymin){
		pos += 2;
	} else if (p.y > view.ymax){
		pos += 1;
	}
	
	return pos;
}

int pointerPos(viewport view, point p){
	int pos = 0;
	if (p.x < (view.xmin - 10)){
		pos += 8;
	} else if (p.x > (view.xmax + 10)){
		pos += 4;
	}
	
	if (p.y < (view.ymin - 10)){
		pos += 2;
	} else if (p.y > (view.ymax + 10)){
		pos += 1;
	}
	
	return pos;
}

//Mengisi nilai xmax xmin ymax ymin sebuah viewport
void initialize(viewport* view){
	view->xmax = view->p1.x;
	view->xmin = view->p1.x;
	view->ymax = view->p1.y;
	view->ymin = view->p1.y;
	if(view->p2.x >= view->xmax){
		view->xmax = view->p2.x;
	}
	if(view->p2.x < view->xmin){
		view->xmin = view->p2.x;
	}
	if(view->p3.x >= view->xmax){
		view->xmax = view->p3.x;
	}
	if(view->p3.x < view->xmin){
		view->xmin = view->p3.x;
	}
	if(view->p4.x >= view->xmax){
		view->xmax = view->p4.x;
	}
	if(view->p4.x < view->xmin){
		view->xmin = view->p4.x;
	}
	
	if(view->p2.y >= view->ymax){
		view->ymax = view->p2.y;
	}
	if(view->p2.y < view->ymin){
		view->ymin = view->p2.y;
	}
	if(view->p3.y >= view->ymax){
		view->ymax = view->p3.y;
	}
	if(view->p3.y < view->ymin){
		view->ymin = view->p3.y;
	}
	if(view->p4.y >= view->ymax){
		view->ymax = view->p4.y;
	}
	if(view->p4.y < view->ymin){
		view->ymin = view->p4.y;
	}
}
 
double dot(point a, point b)
{
	return a.x * b.x + a.y * b.y;
}
 
double cross(point a, point b)
{
	return a.x * b.y - a.y * b.x;
}

// &res adalah hasil vektor a - b 
void vsub(point a, point b, point &res)
{
	res.x = a.x - b.x;
	res.y = a.y - b.y;
}

// kurangi vektor b dgn a, c dgn b, lalu hasilnya dicrosskan.
// Jika hasilnya < 0, maka return -1. else return x > 0 ( 1 atau 0 )
int left_of(point a, point b, point c)
{
	point tmp1, tmp2;
	double x;
	vsub(b, a, tmp1);
	vsub(c, b, tmp2);
	x = cross(tmp1, tmp2);
	return x < 0 ? -1 : x > 0;
}
 
int line_sect(point x0, point x1, point y0, point y1, point& res)
{
	point dx, dy, d;
	vsub(x1, x0, dx);
	vsub(y1, y0, dy);
	vsub(x0, y0, d);

	double dyx = cross(dy, dx);
	if (!dyx) return 0;
	dyx = cross(d, dx) / dyx;
	if (dyx <= 0 || dyx >= 1) return 0;
 
	res.x = y0.x + dyx * dy.x;
	res.y = y0.y + dyx * dy.y;

	return 1;
}
 
poly poly_new()
{
	return (poly)calloc(1, sizeof(poly_t));
}
 
void poly_free(poly p)
{
	free(p->v);
	free(p);
}
 
void poly_append(poly p, point* v)
{
	if (p->len >= p->alloc) {
		p->alloc *= 2;
		if (!p->alloc) p->alloc = 4;
		p->v = (point*)realloc(p->v, sizeof(point) * p->alloc);
	}
	p->v[p->len++] = *v;
}

int poly_winding(poly p)
{
	return left_of( *(p->v), *(p->v + 1), *(p->v + 2));
}


void poly_edge_clip(poly sub, point x0, point x1, int left, poly res)
{
	int i, side0, side1;
	point tmp, v1;

	//point v0 adalah point terakhir pada subjek
	point v0 = *(sub->v + sub->len - 1);

	// panjang pointer diset ke 0
	res->len = 0;
 
	side0 = left_of(x0, x1, v0);
	if (side0 != -left){
		poly_append(res, &v0);
	}
 
	for (i = 0; i < sub->len; i++) {
		v1 = *(sub->v + i);
		side1 = left_of(x0, x1, v1);
		if (side0 + side1 == 0 && side0)
			if (line_sect(x0, x1, v0, v1, tmp)){
				poly_append(res, &tmp);

			}
		if (i == sub->len - 1) break;
		if (side1 != -left){
			poly_append(res, &v1);

		};
		v0 = v1;
		side0 = side1;
	}
}
 
poly poly_clip(poly sub, poly clip)
{
	int i;
	poly p1 = poly_new(), p2 = poly_new(), tmp;
 
	int dir = poly_winding(clip);
	poly_edge_clip(sub, *(clip->v + clip->len - 1), *(clip->v), dir, p2);
	// looping setiap sisi dari clip
	for (i = 0; i < clip->len - 1; i++) {
		// menukar hasil clip menjadi inputan selanjutnya
		tmp = p2; p2 = p1; p1 = tmp;
		// kalo titik di inputan sudah tidak ada, break
		if(p1->len == 0) {
			p2->len = 0;
			break;
		}
		poly_edge_clip(p1, *(clip->v + i), *(clip->v + i + 1), dir, p2);
	}
 
	poly_free(p1);
	return p2;
}