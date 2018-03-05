#include "point_warna.h"
#include <vector>

using namespace std;

point scalePoint(point pivot, point p2, double kelipatan);

void scaleBanyak(point pivot, vector<point> &pp, double kelipatan, int banyak);

void translasi(point &pp, char direction, int jarakTranslasi);

void translasiBanyak(vector<point> &pp, char direction, int jarakTranslasi);