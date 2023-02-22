#include <iostream>
#include <string>
#include <graphics.h>
#include "window_logic.hpp"

struct Point {
    int x{};
    int y{};
};

struct DrawableText {
    Point topLeft{};

    std::string text{};
    textsettingstype settings{};
};

struct Window {
    int xAxisLength{};
    int yAxisLength{};
    std::string title{};
};

struct TitleBox {
    int verticalSize{};
    int outsidePadding{};
    int insidePadding{};

    int verticalTextSpace{};
    int horizontalTextSpace{};
};

struct Title {
    TitleBox box{};
    DrawableText drawableText{};
};

struct Board {
    int sideSize{};
    int padding{};

    int tilesOnSide{};
    int tileSize{};
    int tilePadding{};

    Point topLeft{};
};

struct Piece {
    Point topLeft{};
    Point centre{};
    int size{};

    fillsettingstype fillSettings{};
};

enum pieces {
    playerOne = 1,
    playerTwo = 2,
    empty = 0
};

struct GameStatus {
    Point emptySpace{};
};

struct PlayerPreferences {
    fillsettingstype player1{};
    fillsettingstype player2{};
};

enum directions {
    left, up, right, down
};

TitleBox getTitleBox (int verticalSize, int outsidePadding, int insidePadding, const Window& window) {
    TitleBox titleBox{ .verticalSize = verticalSize, .outsidePadding = outsidePadding, .insidePadding = insidePadding, };

    titleBox.verticalTextSpace = verticalSize - 2 * titleBox.insidePadding;
    titleBox.horizontalTextSpace = (window.xAxisLength - 2 * titleBox.outsidePadding) - 2 * titleBox.insidePadding;

    return titleBox;
}

Title getTitle (const TitleBox& titleBox, const std::string& text, const textsettingstype& textSettings) {
    Title title { .box = titleBox, .drawableText = { .text = text, .settings = textSettings } };

    // Find maximum text sideSize
    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);
    settextstyle(title.drawableText.settings.font, title.drawableText.settings.direction, title.drawableText.settings.charsize);

    auto totalPadding = title.box.outsidePadding + title.box.insidePadding;

    title.drawableText.topLeft.x = totalPadding + (title.box.horizontalTextSpace - textwidth( const_cast<char*>(title.drawableText.text.c_str()) )) / 2;
    title.drawableText.topLeft.y = totalPadding + (title.box.verticalTextSpace - textheight( const_cast<char*>(title.drawableText.text.c_str()) )) / 2;

    settextstyle(initialTextSettings.font, initialTextSettings.direction, initialTextSettings.charsize);

    return title;
}

Board getBoard (int tilesOnSide, int tileSize, int tilePadding, int padding, const TitleBox& titleBox, const Window& window) {
    Board board { .padding = padding, .tilesOnSide = tilesOnSide, .tileSize = tileSize, .tilePadding = tilePadding };

    board.sideSize = board.tilesOnSide * board.tileSize;
    board.topLeft = {(window.xAxisLength - board.sideSize) / 2, (titleBox.outsidePadding + titleBox.verticalSize + board.padding) };

    return board;
}

Piece getPiece (const Point& topLeft, const fillsettingstype& fillSettings, const Board& board) {
    Piece piece { .fillSettings = fillSettings };

    piece.size = board.tileSize - 2 * board.tilePadding;

    piece.topLeft = {topLeft.x + board.tilePadding, topLeft.y + board.tilePadding };
    piece.centre = {piece.topLeft.x + piece.size / 2, piece.topLeft.y + piece.size / 2 };

    return piece;
}

void drawPiece (const Piece& piece) {
    auto radius {piece.size / 2 };

    auto currentColour = static_cast<colors>(getcolor());
    fillsettingstype currentFillSettings{};
    getfillsettings(&currentFillSettings);

    setcolor(piece.fillSettings.color);
    setfillstyle(piece.fillSettings.pattern, piece.fillSettings.color);

    fillellipse(piece.centre.x, piece.centre.y, radius, radius);

    setcolor(currentColour);
    setfillstyle(currentFillSettings.pattern, currentFillSettings.color);
}

void drawBoard (const Board& board, pieces matrix[][5], const PlayerPreferences& playerPreferences) {
    for (int rowNum { 1 }; rowNum <= board.tilesOnSide; rowNum++) {
        for (int colNum { 1 }; colNum <= board.tilesOnSide; colNum++) {
            Point tileTopLeft {board.topLeft.x + (colNum - 1) * board.tileSize, board.topLeft.y + (rowNum - 1) * board.tileSize };
            Point tileBottomRight {board.topLeft.x + colNum * board.tileSize, board.topLeft.y + rowNum * board.tileSize };

            // Draw tile
            rectangle(tileTopLeft.x, tileTopLeft.y, tileBottomRight.x, tileBottomRight.y);

            // Draw piece
            if (matrix[rowNum - 1][colNum - 1] == empty) {
                continue;
            }

            drawPiece(getPiece(tileTopLeft, matrix[rowNum - 1][colNum - 1] == playerOne ? playerPreferences.player1 : playerPreferences.player2, board));
        }
    }
}

int getMaxTitleSize (const char* text, TitleBox titleBox, textsettingstype textSettings) {
    constexpr int maxCharSize { 10 };

    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);

    int maxTitleCharSize{ 1 };
    for (int i = 1; i <= maxCharSize; i++) {
        settextstyle(textSettings.font, textSettings.direction, i);

        if (textheight(const_cast<char*>(text)) <= (titleBox.verticalSize - 2 * titleBox.insidePadding)) {
            maxTitleCharSize = i;
        }
        else {
            break;
        }
    }

    settextstyle(initialTextSettings.font, initialTextSettings.direction, initialTextSettings.charsize);

    return maxTitleCharSize;
}

bool isInBounds (const Board& board, const Point& position) {
    if (position.x < 0 || position.x >= board.tilesOnSide) {
        return false;
    }
    if (position.y < 0 || position.y >= board.tilesOnSide) {
        return false;
    }

    return true;
}

bool isOccupied (pieces matrix[][5], const Point& position) {
    return matrix[position.x][position.y] != empty;
}

bool canMoveInDirection (const Board& board, pieces matrix[][5], const Point& from, const directions& direction) {
    Point to {};
    switch (direction) {
        case left:
            to = { from.x - 1, from.y };
            break;
        case up:
            to = { from.x, from.y - 1 };
            break;
        case right:
            to = { from.x + 1, from.y };
            break;
        case down:
            to = { from.x, from.y + 1 };
            break;
    }

    if (!isInBounds(board, to)) {
        return false;
    }
    if (isOccupied(matrix, to)) {
        return false;
    }

    return true;
}

int main() {
    using namespace std::literals;

    // Create window
    const Window window { 1280, 720, "Jocul Patratelor Glisante"s };
    initwindow(window.xAxisLength, window.yAxisLength, window.title.c_str());

    // Title Properties
    const TitleBox titleBox { getTitleBox(100, 20, 5, window) };

    textsettingstype titleTextSettings { .font = SANS_SERIF_FONT, .direction = HORIZ_DIR };
    titleTextSettings.charsize = getMaxTitleSize(window.title.c_str(), titleBox, titleTextSettings);
    const Title title { getTitle(titleBox, window.title, titleTextSettings) };

    // Board Properties
    constexpr int numOfTilesOnSide { 5 };

    const int minBoardPadding { title.box.outsidePadding };
    const int maxBoardSize { window.yAxisLength - (title.box.outsidePadding + title.box.verticalSize + 2 * minBoardPadding) };
    const int maxTileSize { maxBoardSize / numOfTilesOnSide };

    const int minTileSize { 80 };
    const int minBoardSize { numOfTilesOnSide * minTileSize };
    const int maxBoardPadding { (window.yAxisLength - (title.box.outsidePadding + title.box.verticalSize) - minBoardSize) / 2 };

    const Board smallestBoard { getBoard(numOfTilesOnSide, minTileSize, 20, maxBoardPadding, title.box, window) };
    const Board largestBoard { getBoard(numOfTilesOnSide, maxTileSize, 20, minBoardPadding, title.box, window) };

    // Player preferences
    PlayerPreferences playerPreferences { {SOLID_FILL, LIGHTRED}, {SOLID_FILL, LIGHTBLUE} };

    // Draw title
    settextstyle(title.drawableText.settings.font, title.drawableText.settings.direction, title.drawableText.settings.charsize);
    outtextxy(title.drawableText.topLeft.x, title.drawableText.topLeft.y, const_cast<char*>(title.drawableText.text.c_str()));

    // Create game status
    GameStatus gameStatus{ .emptySpace { numOfTilesOnSide / 2, numOfTilesOnSide / 2 } };

    // Create board matrix
    pieces boardMatrix[numOfTilesOnSide][numOfTilesOnSide];
    pieces nextPiece { playerOne };
    for (int i = 0; i < numOfTilesOnSide; i++) {
        for (int j = 0; j < numOfTilesOnSide; j++) {
            if (i == gameStatus.emptySpace.x && j == gameStatus.emptySpace.y) {
                boardMatrix[i][j] = empty;
            }
            else {
                boardMatrix[i][j] = nextPiece;
            }
            nextPiece = nextPiece == playerOne ? playerTwo : playerOne;
        }
    }

    // Draw board
    drawBoard(largestBoard, boardMatrix, playerPreferences);

    // Close window
    getch();
    closegraph();

    return 0;
}
