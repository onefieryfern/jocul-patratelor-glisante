#include <iostream>
#include <graphics.h>

struct Point {
    short x{};
    short y{};
};

short readWithValidation (short minIncl, short maxIncl) {
    short data{};

    do {
        std::cin >> data;
    } while (data < minIncl || data > maxIncl);

    return data;
}

int main() {
    // Read data
    std::cout << "Window Properties: ";

    std::cout << "X Axis [0 - 1280]: ";
    const short xAxisLength { readWithValidation(0, 1280) };
    std::cout << "Y Axis [0 - 720]: ";
    const short yAxisLength { readWithValidation(0 , 720) };

    // Window Properties
    constexpr char title[] { "Jocul Patratelor Glisante" };

    // Title Properties
    const short verticalTitleSize { 100 };
    constexpr short titlePadding { 20 };

    // Board Properties
    constexpr short numOfTilesOnSide { 5 };

    const short tileSize { 80 };

    const short boardSize { numOfTilesOnSide * tileSize };
    const short boardPadding { static_cast<short>((yAxisLength - (titlePadding + verticalTitleSize) - boardSize) / 2) };

    // Other setup
    const short minBoardPadding { titlePadding };
    const short maxBoardSize { static_cast<short>(yAxisLength - (titlePadding + verticalTitleSize + 2 * minBoardPadding)) };
    const short maxTileSize { static_cast<short>(maxBoardSize / numOfTilesOnSide) };

    const short minTileSize { static_cast<short>(yAxisLength <= xAxisLength ? yAxisLength / 9 : xAxisLength / 16) };
    const short minBoardSize { static_cast<short>(numOfTilesOnSide * minTileSize) };
    const short maxBoardPadding { static_cast<short>((yAxisLength - (titlePadding + verticalTitleSize) - minBoardSize) / 2) };

    //
    initwindow(xAxisLength, yAxisLength, title);
    std::cout << "Window has been created!\n";

    // Draw title placeholder
    rectangle(titlePadding, titlePadding, xAxisLength - titlePadding, verticalTitleSize + titlePadding);

    Point boardTopLeft { static_cast<short>((xAxisLength - boardSize) / 2), static_cast<short>((titlePadding + verticalTitleSize + boardPadding)) };

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
