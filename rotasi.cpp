#include "rotasi.h"
#include <math.h>

using namespace std;

point rotasi(point pivot, point p2, double degree) {
    point result;
    double sind = sin(degree);
    double cosd = cos(degree);

    double deltaX = p2.x - pivot.x;
    double deltaY = p2.y - pivot.y;
    result.x = (deltaX * cosd) - (deltaY * sind) + pivot.x;
    result.y = (deltaX * sind) + (deltaY * cosd) + pivot.y;

    return result;
}

void rotasiBanyak (point pivot, vector<point> &pp, double degree, double banyak) {
    int i;
    for(i = 0; i < banyak; i++) {
        pp[i] = rotasi(pivot, pp[i], degree);
    }
}