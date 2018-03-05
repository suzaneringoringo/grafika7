#ifndef CLIP_H
#define CLIP_H

#include "point_warna.h"
#include <vector>
#include <cstdio>
#include <bitset>

typedef struct{
	point p1,p2,p3,p4;
	double xmax,xmin,ymax,ymin;
} viewport;

typedef struct { int len, alloc; point* v; } poly_t, *poly;

void initialize(viewport* view);

int pointPos(viewport view, point p);

double dot(point a, point b);

double cross(point a, point b);

void vsub(point a, point b, point &res);

int left_of(point a, point b, point c);

int line_sect(point x0, point x1, point y0, point y1, point& res);

poly poly_new();

void poly_free(poly p);

void poly_append(poly p, point* v);

int poly_winding(poly p);

void poly_edge_clip(poly sub, point x0, point x1, int left, poly res);

poly poly_clip(poly sub, poly clip);

#endif