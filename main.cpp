#include <iostream>
#include <string>
#include <graphics.h>
#include "window_logic.hpp"

struct Point {
    int x{};
    int y{};
};

struct Position {
    int row{};
    int col{};
};

struct DrawableText {
    Point topLeft{};

    std::string text{};
    textsettingstype settings{};
};

struct TextBox {
    Point topLeft{};

    int verticalSize{};
    int horizontalSize{};

    int outsidePadding{};
    int insidePadding{};

    int verticalTextSpace{};
    int horizontalTextSpace{};

    fillsettingstype background{};

    DrawableText drawableText{};
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
    Position emptySpace{};
};

struct PlayerPreferences {
    fillsettingstype player1{};
    fillsettingstype player2{};
};

enum directions {
    left = 0, up = 1, right = 2, down = 3
};

constexpr int numOfDirections = 4;
constexpr Position movementInstructions[numOfDirections] { {0, -1}, {-1, 0}, {0, +1}, {+1, 0} };

constexpr int keyToDirection[][numOfDirections] { {KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN}, {left, up, right, down} };
constexpr directions oppositeDirections[numOfDirections] {right, down, left, up};

TitleBox getTitleBox (int verticalSize, int outsidePadding, int insidePadding, const Window& window) {
    TitleBox titleBox{ .verticalSize = verticalSize, .outsidePadding = outsidePadding, .insidePadding = insidePadding, };

    titleBox.verticalTextSpace = verticalSize - 2 * titleBox.insidePadding;
    titleBox.horizontalTextSpace = (window.xAxisLength - 2 * titleBox.outsidePadding) - 2 * titleBox.insidePadding;

    return titleBox;
}

Title getTitle (const TitleBox& titleBox, const std::string& text, const textsettingstype& textSettings) {
    Title title { .box = titleBox, .drawableText = { .text = text, .settings = textSettings } };

    // Find maximum text size
    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);
    settextstyle(title.drawableText.settings.font, title.drawableText.settings.direction, title.drawableText.settings.charsize);

    auto totalPadding = title.box.outsidePadding + title.box.insidePadding;

    title.drawableText.topLeft.x = totalPadding + (title.box.horizontalTextSpace - textwidth( const_cast<char*>(title.drawableText.text.c_str()) )) / 2;
    title.drawableText.topLeft.y = totalPadding + (title.box.verticalTextSpace - textheight( const_cast<char*>(title.drawableText.text.c_str()) )) / 2;

    settextstyle(initialTextSettings.font, initialTextSettings.direction, initialTextSettings.charsize);

    return title;
}

int getMaxTextSize (const char* text, int maxVerticalSpace, textsettingstype textSettings) {
    constexpr int maxCharSize { 10 };

    textsettingstype initialTextSettings{};
    gettextsettings(&initialTextSettings);

    int maxTitleCharSize{ 1 };
    for (int i = 1; i <= maxCharSize; i++) {
        settextstyle(textSettings.font, textSettings.direction, i);

        if (textheight(const_cast<char*>(text)) <= maxVerticalSpace) {
            maxTitleCharSize = i;
        }
        else {
            break;
        }
    }

    settextstyle(initialTextSettings.font, initialTextSettings.direction, initialTextSettings.charsize);

    return maxTitleCharSize;
}

TextBox getTextBox (int yCoordinate, int outerPadding, int verticalSize, int innerPadding, int xAxisLength, fillsettingstype background, std::string text, textsettingstype textSettings) {
    TextBox textBox{};

    textBox.outsidePadding = outerPadding;
    textBox.insidePadding = innerPadding;

    textBox.topLeft = {textBox.outsidePadding, yCoordinate };
    textBox.verticalSize = verticalSize;
    textBox.horizontalSize = xAxisLength - 2 * textBox.outsidePadding;

    textBox.verticalTextSpace = textBox.verticalSize - 2 * textBox.insidePadding;
    textBox.horizontalTextSpace = textBox.horizontalSize - 2 * textBox.insidePadding;

    textBox.background = background;

    textBox.drawableText.text = text;
    textBox.drawableText.settings = { textSettings.font, textSettings.direction, getMaxTextSize(const_cast<char*>(textBox.drawableText.text.c_str()), textBox.verticalSize, textSettings) };

    //
    textsettingstype currentTextSettings{};
    gettextsettings(&currentTextSettings);
    settextstyle(textBox.drawableText.settings.font, textBox.drawableText.settings.direction, textBox.drawableText.settings.charsize);

    textBox.drawableText.topLeft.x = textBox.topLeft.x + ( textBox.horizontalTextSpace - textwidth(const_cast<char*>(textBox.drawableText.text.c_str())) ) / 2;
    textBox.drawableText.topLeft.y = textBox.topLeft.y + ( textBox.verticalTextSpace - textheight(const_cast<char*>(textBox.drawableText.text.c_str())) ) / 2;

    settextstyle(currentTextSettings.font, currentTextSettings.direction, currentTextSettings.charsize);

    return textBox;
}

void drawTextBox (const TextBox& textBox) {
    // Store initial settings
    fillsettingstype currentFillSettings{};
    getfillsettings(&currentFillSettings);

    textsettingstype currentTextSettings{};
    gettextsettings(&currentTextSettings);

    auto currentBkColour = static_cast<colors>(getbkcolor());

    // Set TextBox settings
    setfillstyle(textBox.background.pattern, textBox.background.color);
    settextstyle(textBox.drawableText.settings.font, textBox.drawableText.settings.direction, textBox.drawableText.settings.charsize);
    setbkcolor(textBox.background.color);

    // Draw TextBox
    bar(textBox.topLeft.x, textBox.topLeft.y, textBox.topLeft.x + textBox.horizontalSize, textBox.topLeft.y + textBox.verticalSize);
    outtextxy(textBox.drawableText.topLeft.x, textBox.drawableText.topLeft.y, const_cast<char*>(textBox.drawableText.text.c_str()));

    // Restore initial settings
    setfillstyle(currentFillSettings.pattern, currentFillSettings.color);
    settextstyle(currentTextSettings.font, currentTextSettings.direction, currentTextSettings.charsize);
    setbkcolor(currentBkColour);
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

void eraseBoard (const Board& board) {
    fillsettingstype currentFillSettings{};
    getfillsettings(&currentFillSettings);

    fillsettingstype erase {SOLID_FILL, getbkcolor()};
    setfillstyle(erase.pattern, erase.color);
    bar(board.topLeft.x, board.topLeft.y, board.topLeft.x + board.sideSize, board.topLeft.y + board.sideSize);

    setfillstyle(currentFillSettings.pattern, currentFillSettings.color);
}

int getMaxTitleSize (const char* text, TitleBox titleBox, textsettingstype textSettings) {
    return getMaxTextSize(text, titleBox.verticalTextSpace, textSettings);
}

bool isInBounds (const Board& board, const Position& position) {
    if (position.row < 0 || position.row >= board.tilesOnSide) {
        return false;
    }
    if (position.col < 0 || position.col >= board.tilesOnSide) {
        return false;
    }

    return true;
}

bool isOccupied (pieces matrix[][5], const Position& position) {
    return matrix[position.row][position.col] != empty;
}

bool canMoveInDirection (const Board& board, const GameStatus& gamestatus, pieces matrix[][5], directions direction, pieces currentPlayer) {
    Position from { gamestatus.emptySpace.row + movementInstructions[oppositeDirections[direction]].row, gamestatus.emptySpace.col + movementInstructions[oppositeDirections[direction]].col };

    if (matrix[from.row][from.col] != currentPlayer) {
        return false;
    }
    if (!isInBounds(board, from)) {
        return false;
    }
    if (isOccupied(matrix, gamestatus.emptySpace)) {
        return false;
    }

    return true;
}

void moveInDirection (GameStatus& gamestatus, pieces matrix[][5], directions direction) {
    Position from { gamestatus.emptySpace.row + movementInstructions[oppositeDirections[direction]].row, gamestatus.emptySpace.col + movementInstructions[oppositeDirections[direction]].col };

    matrix[gamestatus.emptySpace.row][gamestatus.emptySpace.col] = matrix[from.row][from.col];
    matrix[from.row][from.col] = empty;

    gamestatus.emptySpace = from;
}

pieces whoHasWon (const Board& board, pieces matrix[][5], const GameStatus& gameStatus) {
    pieces firstPieceChecked{};
    for (int i = 0; i < numOfDirections; i++) {
        Position positionToBeChecked { gameStatus.emptySpace.row + movementInstructions[i].row, gameStatus.emptySpace.col + movementInstructions[i].col };

        if (isInBounds(board, positionToBeChecked)) {
            firstPieceChecked = matrix[positionToBeChecked.row][positionToBeChecked.col];
            break;
        }
    }

    for (int i = 0; i < numOfDirections; i++) {
        Position positionToBeChecked { gameStatus.emptySpace.row + movementInstructions[i].row, gameStatus.emptySpace.col + movementInstructions[i].col };

        if (!isInBounds(board, positionToBeChecked)) {
            continue;
        }
        if (matrix[positionToBeChecked.row][positionToBeChecked.col] != firstPieceChecked) {
            return empty; // has not won
        }
    }

    return firstPieceChecked; // has won
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
    pieces nextPiece { playerTwo };
    for (int i = 0; i < numOfTilesOnSide; i++) {
        for (int j = 0; j < numOfTilesOnSide; j++) {
            if (i == gameStatus.emptySpace.row && j == gameStatus.emptySpace.col) {
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

    bool firstMoveMade { false };
    pieces playerTurn { playerOne };
    while (whoHasWon(largestBoard, boardMatrix, gameStatus) == empty || !firstMoveMade) {
        directions movementDirection {};
        bool moveMade { false };

        if (playerTurn == playerOne) {
            bool gotValidInput{ false };

            if (kbhit()) {
                int input{getch()};
                for (int i = 0; i < numOfDirections; i++) {
                    if (input == keyToDirection[0][i]) {
                        movementDirection = static_cast<directions>(keyToDirection[1][i]);
                        gotValidInput = true;
                    }
                }
            }

            if (gotValidInput && canMoveInDirection(largestBoard, gameStatus, boardMatrix, movementDirection, playerTurn)) {
                moveInDirection(gameStatus, boardMatrix, movementDirection);

                eraseBoard(largestBoard);
                drawBoard(largestBoard, boardMatrix, playerPreferences);

                if (!firstMoveMade) {
                    firstMoveMade = true;
                }

                moveMade = true;
            }
        }
        else { // playerTurn == playerTwo
            while (!moveMade) {
                auto randomDirection = static_cast<directions>(std::rand() % 4);

                if (canMoveInDirection(largestBoard, gameStatus, boardMatrix, randomDirection, playerTurn)) {
                    moveInDirection(gameStatus, boardMatrix, randomDirection);

                    eraseBoard(largestBoard);
                    drawBoard(largestBoard, boardMatrix, playerPreferences);

                    moveMade = true;
                }
            }
        }

        if (moveMade) {
            playerTurn = playerTurn == playerOne ? playerTwo : playerOne;
        }

        delay(100);
    }

    TextBox playerOneWin { getTextBox(20, 40, 160, 5, window.xAxisLength, {SOLID_FILL, playerPreferences.player1.color}, "Player One has won!"s, { .font = SANS_SERIF_FONT, .direction = HORIZ_DIR }) };
    TextBox playerTwoWin { getTextBox(20, 40, 160, 5, window.xAxisLength, {SOLID_FILL, playerPreferences.player2.color}, "Player Two has won!"s, { .font = SANS_SERIF_FONT, .direction = HORIZ_DIR }) };

    if (whoHasWon(largestBoard, boardMatrix, gameStatus) == playerOne) {
        drawTextBox(playerOneWin);
    }
    else {
        drawTextBox(playerTwoWin);
    }

    // Close window
    getch();
    closegraph();

    return 0;
}
