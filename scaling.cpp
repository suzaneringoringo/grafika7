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

void translasi(point &pp, char direction, int jarakTranslasi){
    if (direction == 'w') {
        pp.y = pp.y - jarakTranslasi;
        
    } else if (direction == 'a') {
            pp.x = pp.x - jarakTranslasi;      
    } else if (direction == 's') {
        
            pp.y = pp.y + jarakTranslasi;
 
    } else if (direction == 'd') {
        
            pp.x = pp.x + jarakTranslasi;
               
    }
}

void translasiBanyak(vector<point> &pp, char direction, int jarakTranslasi) {
    if (direction == 'w') {
        for (int i = 0; i < pp.size(); i++) {
            pp[i].y = pp[i].y - jarakTranslasi;
        }
    } else if (direction == 'a') {
        for (int i = 0; i < pp.size(); i++) {
            pp[i].x = pp[i].x - jarakTranslasi;
        }        
    } else if (direction == 's') {
        for (int i = 0; i < pp.size(); i++) {
            pp[i].y = pp[i].y + jarakTranslasi;
        }
    } else if (direction == 'd') {
        for (int i = 0; i < pp.size(); i++) {
            pp[i].x = pp[i].x + jarakTranslasi;
        }        
    }
}