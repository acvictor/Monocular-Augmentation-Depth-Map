#include <bits/stdc++.h>

using namespace std;

class ImColor {
public:
    int r, g, b;
    ImColor(int _r, int _g, int _b) {
        r = _r;
        g = _g;
        b = _b;
    }
};

class Box {
public:
    int minx, miny, maxx, maxy;
    float minz, maxz;
    Box(int _minx, int _maxx, int _miny, int _maxy, float _minz, float _maxz) {
        minx = _minx;
        miny = _miny;
        maxx = _maxx;
        maxy = _maxy;
        minz = _minz;
        maxz = _maxz;
    }    
};

class BoundingBox {
public:
    vector<ImColor> colors;
    vector<Box> boxes;
    void computeBoundingBoxes(int, char**, float[][768]);
};