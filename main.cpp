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

    const short verticalTitleSize { 100 };
    constexpr short titlePadding { 20 };

    initwindow(xAxisLength, yAxisLength, title);

    const short boardSize { numOfTilesOnSide * tileSize };
    const short boardPadding { (yAxisLength - (titlePadding + verticalTitleSize) - boardSize) / 2 };

    // Draw title placeholder
    rectangle(titlePadding, titlePadding, xAxisLength - titlePadding, verticalTitleSize + titlePadding);

    const short spaceOccupiedByTitle { verticalTitleSize + 2 * titlePadding };

    Point boardTopLeft { (xAxisLength - boardSize) / 2, (titlePadding + verticalTitleSize + boardPadding)};

    // Offset caused by title
    // boardTopLeft.y += verticalTitleSize + 2 * titlePadding;

    // rectangle(boardTopLeft.x, boardTopLeft.y, boardTopLeft.x + tileSize, boardTopLeft.y + tileSize);

    for (short rowNum { 1 }; rowNum <= numOfTilesOnSide; rowNum++) {
        for (short colNum { 1 }; colNum <= numOfTilesOnSide; colNum++) {
            Point topLeft { static_cast<short>(boardTopLeft.x + (colNum - 1) * tileSize), static_cast<short>(boardTopLeft.y + (rowNum - 1) * tileSize) };
            Point bottomRight { static_cast<short>(boardTopLeft.x + colNum * tileSize), static_cast<short>(boardTopLeft.y + rowNum * tileSize) };

            // Draw tile
            rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
        }
    }

    getch();
    closegraph();

    return 0;
}
