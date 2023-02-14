#include <iostream>
#include <graphics.h>

struct Point {
    short x{};
    short y{};
};

int main() {
    constexpr short xAxisLength { 1280 };
    constexpr short yAxisLength { 720 };

    constexpr char title[] { "Jocul Patratelor Glisante" };

    constexpr short numOfTilesOnSide { 5 };
    const short tileSize { 80 };

    const short verticalOffset { 100 };

    initwindow(xAxisLength, yAxisLength, title);

    const short boardSize { numOfTilesOnSide * tileSize };

    Point boardTopLeft { (xAxisLength - boardSize) / 2,  (yAxisLength - boardSize) / 2};

    // rectangle(boardTopLeft.x, boardTopLeft.y, boardTopLeft.x + tileSize, boardTopLeft.y + tileSize);

    for (short rowNum { 1 }; rowNum <= numOfTilesOnSide; rowNum++) {
        for (short colNum { 1 }; colNum <= numOfTilesOnSide; colNum++) {
            rectangle(boardTopLeft.x + ((colNum - 1) * tileSize), boardTopLeft.y + ((rowNum - 1) * tileSize) + verticalOffset, boardTopLeft.x + (colNum * tileSize), boardTopLeft.y + (rowNum * tileSize) + verticalOffset);
        }
    }

    getch();
    closegraph();

    return 0;
}
