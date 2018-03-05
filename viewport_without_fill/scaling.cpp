#include "scaling.h"
#include <cmath>

using namespace std;

point scalePoint(point pivot, point p2, double kelipatan) {
   point output;

   int jarakx = abs(p2.x - pivot.x);
   int jaraky = abs(p2.y - pivot.y);

    if (pivot.x > p2.x) {
        output.x = pivot.x - jarakx * kelipatan;
    }
    else {
        output.x = pivot.x + jarakx * kelipatan; 
    }

    if (pivot.y > p2.y) {
        output.y = pivot.y - jaraky * kelipatan;
    } else {
        output.y = pivot.y + jaraky * kelipatan;
    }

    return output;
}

void scaleBanyak(point pivot, vector<point> &pp, double kelipatan, int banyak) {
   int i;
   for (i = 0; i < banyak; i++) {
       pp[i] = scalePoint(pivot, pp[i], kelipatan);
   }
}