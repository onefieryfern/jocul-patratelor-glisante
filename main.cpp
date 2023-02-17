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

    short padding{};
};

struct Board {
    BoardProperties properties{};

    Point topLeft{};
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

Board initBoard (BoardProperties boardProperties, TitleProperties titleProperties, short xAxisLength) {
    Board board { .properties = boardProperties };

    board.topLeft = { static_cast<short>((xAxisLength - boardProperties.size) / 2), static_cast<short>((titleProperties.outsidePadding + titleProperties.totalVerticalSize + boardProperties.padding)) };

    return board;
}

PieceProperties initPieceProperties (Point topLeft, fillsettingstype fillSettings, BoardProperties boardProperties) {
    PieceProperties pieceProperties { .fillSettings = fillSettings };

    pieceProperties.size = static_cast<short>(boardProperties.tileSize - 2 * boardProperties.tilePadding);

    pieceProperties.topLeft = { static_cast<short>(topLeft.x + boardProperties.tilePadding), static_cast<short>(topLeft.y + boardProperties.tilePadding) };
    pieceProperties.centre = { static_cast<short>(pieceProperties.topLeft.x + pieceProperties.size / 2), static_cast<short>(pieceProperties.topLeft.y + pieceProperties.size / 2) };

    return pieceProperties;
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

void drawBoard (Board board) {
    for (short rowNum { 1 }; rowNum <= board.properties.numOfTilesOnSide; rowNum++) {
        for (short colNum { 1 }; colNum <= board.properties.numOfTilesOnSide; colNum++) {
            Point tileTopLeft {static_cast<short>(board.topLeft.x + (colNum - 1) * board.properties.tileSize), static_cast<short>(board.topLeft.y + (rowNum - 1) * board.properties.tileSize) };
            Point tileBottomRight {static_cast<short>(board.topLeft.x + colNum * board.properties.tileSize), static_cast<short>(board.topLeft.y + rowNum * board.properties.tileSize) };

            // Draw tile
            rectangle(tileTopLeft.x, tileTopLeft.y, tileBottomRight.x, tileBottomRight.y);

            drawPiece(initPieceProperties(tileTopLeft, {SOLID_FILL, LIGHTBLUE }, initBoardProperties(board.properties.numOfTilesOnSide, board.properties.tileSize, board.properties.padding, 4)));
        }
    }
}

short getMaxTitleSize (const char* title, TitleProperties titleProperties, textsettingstype textSettings) {
    constexpr short maxCharSize { 10 };

    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);

    short maxTitleCharSize{ 1 };
    for (short i = 1; i <= maxCharSize; i++) {
        settextstyle(textSettings.font, textSettings.direction, i);

        if (textheight(const_cast<char*>(title)) <= (titleProperties.totalVerticalSize - 2 * titleProperties.insidePadding)) {
            maxTitleCharSize = i;
        }
        else {
            break;
        }
    }

    settextstyle(initialTextSettings.font, initialTextSettings.direction, initialTextSettings.charsize);

    return maxTitleCharSize;
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

    // Other setup
    const short minBoardPadding { titleProperties.outsidePadding };
    const short maxBoardSize { static_cast<short>(yAxisLength - (titleProperties.outsidePadding + titleProperties.totalVerticalSize + 2 * minBoardPadding)) };
    const short maxTileSize { static_cast<short>(maxBoardSize / numOfTilesOnSide) };

    const short minTileSize { 80 };
    const short minBoardSize { static_cast<short>(numOfTilesOnSide * minTileSize) };
    const short maxBoardPadding { static_cast<short>((yAxisLength - (titleProperties.outsidePadding + titleProperties.totalVerticalSize) - minBoardSize) / 2) };

    const Board smallestBoard { initBoard(initBoardProperties(numOfTilesOnSide, minTileSize, 4, maxBoardPadding), titleProperties, xAxisLength) };
    const Board largestBoard { initBoard(initBoardProperties(numOfTilesOnSide, maxTileSize, 4, minBoardPadding), titleProperties, xAxisLength) };

    /*
    const short tileSize { 80 };

    const short boardSize { numOfTilesOnSide * tileSize };
    const short boardPadding { static_cast<short>((yAxisLength - (titleProperties.outsidePadding + titleProperties.totalVerticalSize) - boardSize) / 2) };
*/

    // Create window
    initwindow(xAxisLength, yAxisLength, title);
    std::cout << "Window has been created!\n";

    // Draw title
    // rectangle(titleProperties.outsidePadding, titleProperties.outsidePadding, xAxisLength - titleProperties.outsidePadding, titleProperties.totalVerticalSize + titleProperties.outsidePadding);

    textsettingstype textSettings { .font = SANS_SERIF_FONT, .direction = HORIZ_DIR };
    short maxTitleCharSize{ getMaxTitleSize(title, titleProperties, textSettings) };

    settextstyle(textSettings.font, textSettings.direction, maxTitleCharSize);

    Point titleTopLeft {};
    titleTopLeft.x = static_cast<short>(titleProperties.outsidePadding + titleProperties.insidePadding + (titleProperties.horizontalTextSpace - textwidth(const_cast<char*>(title))) / 2);
    titleTopLeft.y = static_cast<short>(titleProperties.outsidePadding + titleProperties.insidePadding + (titleProperties.verticalTextSpace - textheight(const_cast<char*>(title))) / 2);
    outtextxy(titleTopLeft.x, titleTopLeft.y, const_cast<char*>(title));

    // Draw board
    drawBoard(largestBoard);

    getch();
    closegraph();

    return 0;
}
