#include <iostream>
#include "Hexagon.h"


int main() {
    HexagonGrid grid(10, OffsetPoint(1,0), OffsetPoint(9, 5));
    grid.liAlg();
    return 0;
}
