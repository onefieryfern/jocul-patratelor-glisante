#include <iostream>
#include <graphics.h>

struct Point {
    short x{};
    short y{};
};

int main() {
    // Window Properties
    constexpr short xAxisLength { 1280 };
    constexpr short yAxisLength { 720 };

    constexpr char title[] { "Jocul Patratelor Glisante" };

    // Title Properties

    const short verticalTitleSize { 100 };
    constexpr short titlePadding { 20 };

    // Board Properties
    constexpr short numOfTilesOnSide { 5 };

    const short tileSize { 80 };

    const short boardSize { numOfTilesOnSide * tileSize };
    const short boardPadding { (yAxisLength - (titlePadding + verticalTitleSize) - boardSize) / 2 };

    // Other setup

    const short minBoardPadding { titlePadding };
    const short maxBoardSize { yAxisLength - (titlePadding + verticalTitleSize + 2 * minBoardPadding) };
    const short maxTileSize { static_cast<short>(maxBoardSize / numOfTilesOnSide) };

    const short minTileSize { yAxisLength <= xAxisLength ? yAxisLength / 9 : xAxisLength / 16 };
    const short minBoardSize { numOfTilesOnSide * minTileSize };
    const short maxBoardPadding { (yAxisLength - (titlePadding + verticalTitleSize) - minBoardSize) / 2 };

    initwindow(xAxisLength, yAxisLength, title);

    // Draw title placeholder
    rectangle(titlePadding, titlePadding, xAxisLength - titlePadding, verticalTitleSize + titlePadding);

    Point boardTopLeft { (xAxisLength - boardSize) / 2, (titlePadding + verticalTitleSize + boardPadding)};

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
