#ifndef HEXAGON_H
#define HEXAGON_H
#include <iostream>
#include "simple_svg_1.0.0_mod.hpp"
#include "math.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;
using namespace svg;

class OffsetPoint{
    public:
    OffsetPoint(){};
    OffsetPoint(int x,int y){
        this->x=x;
        this->y=y;
        this->f=0;
    }

    int x,y;
    int f;//front

    const bool operator == (const OffsetPoint a) const
    {return ((this->x == a.x) && (this->y == a.y));}
    const bool operator != (const OffsetPoint a) const
    {return !(*this == a);}
    const bool operator < (const OffsetPoint a) const
    {return std::tie(a.x, a.y) < std::tie(this->x, this->y);}
};

class HexagonGrid
{
    public:
        HexagonGrid(int dimension,OffsetPoint start, OffsetPoint finish);
        virtual ~HexagonGrid();
        void drawSVG(string fileName);
        std::list<OffsetPoint> neighbours(int i,int j);
        void liAlg();
        void findPath();
    protected:

    private:
        int dimension;//size
        float hexSize;//размер сетки,расстояние от середиы до вершины
        float gridWidth, gridHeight;
        float hexW,hexH;
        OffsetPoint start,finish;
        map <OffsetPoint, int> fronts;
        set <OffsetPoint> path;
        set <OffsetPoint> terminates;
        void BuildTerminates();
};

#endif // HEXAGON_H
