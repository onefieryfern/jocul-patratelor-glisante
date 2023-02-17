#include <iostream>
#include <graphics.h>
#include "window_logic.hpp"

struct Point {
    short x{};
    short y{};
};

struct TitleProperties {
    short totalVerticalSize { 100 };
    short outsidePadding { 20 };
    short insidePadding { 5 };
    
    short verticalTextSpace{};
    short horizontalTextSpace{};
};

struct BoardProperties {
    short size{};
    short numOfTilesOnSide{};

    short tileSize{};
    short tilePadding{};

    Point topLeft{}; // { static_cast<short>((xAxisLength - maxBoardSize) / 2), static_cast<short>((titleProperties.outsidePadding + titleProperties.totalVerticalSize + minBoardPadding)) };

    short padding{};
};

struct PieceProperties {
    Point topLeft{};
    Point centre{};
    short size{};

    fillsettingstype fillSettings{};
};

TitleProperties initTitleProperties (short verticalSize, short outsidePadding, short insidePadding, short xAxisLength) {
    TitleProperties titleProperties{};
    
    titleProperties.totalVerticalSize = verticalSize;
    titleProperties.outsidePadding = outsidePadding;
    titleProperties.insidePadding = insidePadding;
    
    titleProperties.verticalTextSpace = static_cast<short>(verticalSize - 2 * titleProperties.insidePadding);
    titleProperties.horizontalTextSpace = static_cast<short>((xAxisLength - 2 * titleProperties.outsidePadding) - 2 * titleProperties.insidePadding);
    
    return titleProperties;
}

BoardProperties initBoardProperties (short numOfTilesOnSide, short tileSize, short tilePadding, short padding) {
    BoardProperties boardProperties { .numOfTilesOnSide = numOfTilesOnSide, .tileSize = tileSize, .tilePadding = tilePadding, .padding = padding };

    boardProperties.size = static_cast<short>(numOfTilesOnSide * tileSize);

    // boardProperties.topLeft = { static_cast<short>((xAxisLength - boardProperties.size) / 2), static_cast<short>((titleProperties.outsidePadding + titleProperties.totalVerticalSize + boardProperties.padding)) };

    return boardProperties;
}

PieceProperties initPieceProperties (Point topLeft, fillsettingstype fillSettings, BoardProperties boardProperties) {
    PieceProperties pieceProperties { .fillSettings = fillSettings };

    pieceProperties.size = static_cast<short>(boardProperties.tileSize - 2 * boardProperties.tilePadding);

    pieceProperties.topLeft = { static_cast<short>(topLeft.x + boardProperties.tilePadding), static_cast<short>(topLeft.y + boardProperties.tilePadding) };
    pieceProperties.centre = { static_cast<short>(pieceProperties.topLeft.x + pieceProperties.size / 2), static_cast<short>(pieceProperties.topLeft.y + pieceProperties.size / 2) };

    return pieceProperties;
}

short readWithValidation (short minIncl, short maxIncl) {
    short data{};

    do {
        std::cin >> data;
    } while (data < minIncl || data > maxIncl);

    return data;
}

void drawPiece (PieceProperties pieceProperties) {
    auto radius { static_cast<short>(pieceProperties.size / 2) };

    auto currentColour = static_cast<colors>(getcolor());
    fillsettingstype currentFillSettings{};
    getfillsettings(&currentFillSettings);

    setcolor(pieceProperties.fillSettings.color);
    setfillstyle(pieceProperties.fillSettings.pattern, pieceProperties.fillSettings.color);

    fillellipse(pieceProperties.centre.x, pieceProperties.centre.y, radius, radius);

    setcolor(currentColour);
    setfillstyle(currentFillSettings.pattern, currentFillSettings.color);
}

void drawBoard (BoardProperties boardProperties, Point topLeft) {
    for (short rowNum { 1 }; rowNum <= boardProperties.numOfTilesOnSide; rowNum++) {
        for (short colNum { 1 }; colNum <= boardProperties.numOfTilesOnSide; colNum++) {
            Point tileTopLeft { static_cast<short>(topLeft.x + (colNum - 1) * boardProperties.tileSize), static_cast<short>(topLeft.y + (rowNum - 1) * boardProperties.tileSize) };
            Point tileBottomRight { static_cast<short>(topLeft.x + colNum * boardProperties.tileSize), static_cast<short>(topLeft.y + rowNum * boardProperties.tileSize) };

            // Draw tile
            rectangle(tileTopLeft.x, tileTopLeft.y, tileBottomRight.x, tileBottomRight.y);

            drawPiece(initPieceProperties(tileTopLeft, {SOLID_FILL, LIGHTBLUE }, initBoardProperties(boardProperties.numOfTilesOnSide, boardProperties.tileSize, boardProperties.padding, 4)));
        }
    }
}

int main() {
    // Window Properties
    constexpr short xAxisLength { 1280 };
    constexpr short yAxisLength { 720 };
    constexpr char title[] { "Jocul Patratelor Glisante" };

    // Title Properties
    const TitleProperties titleProperties { initTitleProperties(100, 20, 5, xAxisLength) };

    // Board Properties
    constexpr short numOfTilesOnSide { 5 };

/*
    const short tileSize { 80 };

    const short boardSize { numOfTilesOnSide * tileSize };
    const short boardPadding { static_cast<short>((yAxisLength - (titleProperties.outsidePadding + titleProperties.totalVerticalSize) - boardSize) / 2) };
*/

    // Other setup
    const short minBoardPadding { titleProperties.outsidePadding };
    const short maxBoardSize { static_cast<short>(yAxisLength - (titleProperties.outsidePadding + titleProperties.totalVerticalSize + 2 * minBoardPadding)) };
    const short maxTileSize { static_cast<short>(maxBoardSize / numOfTilesOnSide) };

    const short minTileSize { 80 };
    const short minBoardSize { static_cast<short>(numOfTilesOnSide * minTileSize) };
    const short maxBoardPadding { static_cast<short>((yAxisLength - (titleProperties.outsidePadding + titleProperties.totalVerticalSize) - minBoardSize) / 2) };

    const BoardProperties smallestBoard { initBoardProperties(numOfTilesOnSide, minTileSize, 4, maxBoardPadding) };
    const BoardProperties largestBoard { initBoardProperties(numOfTilesOnSide, maxTileSize, 4, minBoardPadding) };

    //
    initwindow(xAxisLength, yAxisLength, title);
    std::cout << "Window has been created!\n";

    // Draw title
    rectangle(titleProperties.outsidePadding, titleProperties.outsidePadding, xAxisLength - titleProperties.outsidePadding, titleProperties.totalVerticalSize + titleProperties.outsidePadding);

    constexpr short maxCharSize { 10 };
    constexpr short charSizePixels { 8 };

    short maxTitleCharSize{ 1 };
    textsettingstype textSettings{};
    gettextsettings(&textSettings);
    for (short i = 1; i <= maxCharSize; i++) {
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, i);

        if (textheight(const_cast<char*>(title)) <= (titleProperties.totalVerticalSize - 2 * titleProperties.insidePadding)) {
            maxTitleCharSize = i;
        }
        else {
            break;
        }
    }
    settextstyle(textSettings.font, textSettings.direction, textSettings.charsize);

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, maxTitleCharSize);

    //
    Point titleTopLeft {};
    titleTopLeft.x = static_cast<short>(titleProperties.outsidePadding + titleProperties.insidePadding + (titleProperties.horizontalTextSpace - textwidth(const_cast<char*>(title))) / 2);
    titleTopLeft.y = static_cast<short>(titleProperties.outsidePadding + titleProperties.insidePadding + (titleProperties.verticalTextSpace - textheight(const_cast<char*>(title))) / 2);
    outtextxy(titleTopLeft.x, titleTopLeft.y, const_cast<char*>(title));

    /*
    Point boardTopLeft { static_cast<short>((xAxisLength - boardSize) / 2), static_cast<short>((titleProperties.outsidePadding + titleProperties.totalVerticalSize + boardPadding)) };

    for (short rowNum { 1 }; rowNum <= numOfTilesOnSide; rowNum++) {
        for (short colNum { 1 }; colNum <= numOfTilesOnSide; colNum++) {
            Point topLeft { static_cast<short>(boardTopLeft.x + (colNum - 1) * tileSize), static_cast<short>(boardTopLeft.y + (rowNum - 1) * tileSize) };
            Point bottomRight { static_cast<short>(boardTopLeft.x + colNum * tileSize), static_cast<short>(boardTopLeft.y + rowNum * tileSize) };

            // Draw tile
            rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
        }
    }
     */

    Point boardTopLeft {static_cast<short>((xAxisLength - largestBoard.size) / 2), static_cast<short>((titleProperties.outsidePadding + titleProperties.totalVerticalSize + largestBoard.padding)) };
    drawBoard(largestBoard, boardTopLeft);

    /*
    for (short rowNum { 1 }; rowNum <= largestBoard.numOfTilesOnSide; rowNum++) {
        for (short colNum { 1 }; colNum <= largestBoard.numOfTilesOnSide; colNum++) {
            Point topLeft { static_cast<short>(boardTopLeft.x + (colNum - 1) * largestBoard.tileSize), static_cast<short>(boardTopLeft.y + (rowNum - 1) * largestBoard.tileSize) };
            Point bottomRight { static_cast<short>(boardTopLeft.x + colNum * largestBoard.tileSize), static_cast<short>(boardTopLeft.y + rowNum * largestBoard.tileSize) };

            // Draw tile
            rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);

            drawPiece(initPieceProperties(topLeft, { SOLID_FILL, LIGHTBLUE }, initBoardProperties(largestBoard.numOfTilesOnSide, largestBoard.tileSize, largestBoard.padding, 4)));
        }
    }
     */

    // drawPiece(initPieceProperties( {70, 70}, { SOLID_FILL, LIGHTBLUE }, initBoardProperties(numOfTilesOnSide, maxTileSize, minBoardPadding, 4) ));

    getch();
    closegraph();

    return 0;
}
