#include <iostream>
#include <string>
#include <graphics.h>
#include "window_logic.hpp"

struct Point {
    short x{};
    short y{};
};

struct Window {
    short xAxisLength{};
    short yAxisLength{};
    std::string title{};
};

struct TitleBox {
    short totalVerticalSize{};
    short outsidePadding{};
    short insidePadding{};
    
    short verticalTextSpace{};
    short horizontalTextSpace{};
};

struct Title {
    TitleBox box{};

    Point topLeft{};

    std::string text{};
    textsettingstype textSettings{};
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

TitleBox initTitleBox (short verticalSize, short outsidePadding, short insidePadding, const Window& window) {
    TitleBox titleBox{ .totalVerticalSize = verticalSize, .outsidePadding = outsidePadding, .insidePadding = insidePadding, };

    titleBox.verticalTextSpace = static_cast<short>(verticalSize - 2 * titleBox.insidePadding);
    titleBox.horizontalTextSpace = static_cast<short>((window.xAxisLength - 2 * titleBox.outsidePadding) - 2 * titleBox.insidePadding);

    return titleBox;
}

Title initTitle (TitleBox titleBox, const std::string& text, textsettingstype textSettings) {
    Title title { .box = titleBox, .text = text, .textSettings = textSettings };

    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);
    settextstyle(title.textSettings.font, title.textSettings.direction, title.textSettings.charsize);

    title.topLeft.x = static_cast<short>(title.box.outsidePadding + title.box.insidePadding + (title.box.horizontalTextSpace - textwidth(const_cast<char*>(title.text.c_str()))) / 2);
    title.topLeft.y = static_cast<short>(title.box.outsidePadding + title.box.insidePadding + (title.box.verticalTextSpace - textheight(const_cast<char*>(title.text.c_str()))) / 2);

    settextstyle(initialTextSettings.font, initialTextSettings.direction, initialTextSettings.charsize);

    return title;
}

BoardProperties initBoardProperties (short numOfTilesOnSide, short tileSize, short tilePadding, short padding) {
    BoardProperties boardProperties { .numOfTilesOnSide = numOfTilesOnSide, .tileSize = tileSize, .tilePadding = tilePadding, .padding = padding };

    boardProperties.size = static_cast<short>(numOfTilesOnSide * tileSize);

    // boardProperties.topLeft = { static_cast<short>((xAxisLength - boardProperties.size) / 2), static_cast<short>((titleProperties.outsidePadding + titleProperties.totalVerticalSize + boardProperties.padding)) };

    return boardProperties;
}

Board initBoard (BoardProperties boardProperties, TitleBox titleProperties, short xAxisLength) {
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

short getMaxTitleSize (const char* text, TitleBox titleBox, textsettingstype textSettings) {
    constexpr short maxCharSize { 10 };

    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);

    short maxTitleCharSize{ 1 };
    for (short i = 1; i <= maxCharSize; i++) {
        settextstyle(textSettings.font, textSettings.direction, i);

        if (textheight(const_cast<char*>(text)) <= (titleBox.totalVerticalSize - 2 * titleBox.insidePadding)) {
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
    using namespace std::literals;

    // Create window
    const Window window { 1280, 720, "Jocul Patratelor Glisante"s };
    initwindow(window.xAxisLength, window.yAxisLength, window.title.c_str());

    // Title Properties
    const TitleBox titleBox { initTitleBox(100, 20, 5, window) };

    textsettingstype titleTextSettings { .font = SANS_SERIF_FONT, .direction = HORIZ_DIR };
    titleTextSettings.charsize = getMaxTitleSize(window.title.c_str(), titleBox, titleTextSettings);
    const Title title { initTitle(titleBox, window.title, titleTextSettings) };

    // Board Properties
    constexpr short numOfTilesOnSide { 5 };

    const short minBoardPadding { title.box.outsidePadding };
    const short maxBoardSize { static_cast<short>(window.yAxisLength - (title.box.outsidePadding + title.box.totalVerticalSize + 2 * minBoardPadding)) };
    const short maxTileSize { static_cast<short>(maxBoardSize / numOfTilesOnSide) };

    const short minTileSize { 80 };
    const short minBoardSize { static_cast<short>(numOfTilesOnSide * minTileSize) };
    const short maxBoardPadding { static_cast<short>((window.yAxisLength - (title.box.outsidePadding + title.box.totalVerticalSize) - minBoardSize) / 2) };

    const Board smallestBoard { initBoard(initBoardProperties(numOfTilesOnSide, minTileSize, 4, maxBoardPadding), title.box, window.xAxisLength) };
    const Board largestBoard { initBoard(initBoardProperties(numOfTilesOnSide, maxTileSize, 4, minBoardPadding), title.box, window.xAxisLength) };

    // Draw title
    settextstyle(title.textSettings.font, title.textSettings.direction, title.textSettings.charsize);
    outtextxy(title.topLeft.x, title.topLeft.y, const_cast<char*>(title.text.c_str()));

    // Draw board
    drawBoard(largestBoard);

    // Close window
    getch();
    closegraph();

    return 0;
}
