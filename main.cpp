// Copyright 2022 Maciej Kaszkowiak (maciej@kaszkowiak.org)
// Apache License 2.0
#include <iostream>
#include <memory>
#include <vector>
#include <string>

/* 
C++11 features: 
- unique_ptr, shared_ptr
- initalizer lists 
- constructor inheritance

C++14:
- make_unique

OOP techniques used:
- dependency injection
- inheritance
- interfaces (abstract classes) 
- object factories
*/

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::string;

const int BOARD_SIZE = 8;

enum class Color {
    White, Black
};

enum class Result {
    Playing, Check, Checkmate, Stalemate
};

class Piece {
    public:
        Piece(Color color): color(color) {}

        Color getColor() {
            return this->color;
        }

        virtual string getSymbol() = 0;
        //virtual bool isMoveLegal();

    protected:
        Color color;
};

class Pawn: public Piece {
    using Piece::Piece;

    public:
        string getSymbol() {return "Pawn";}
};

class Rook: public Piece {
    using Piece::Piece;

    public:
        string getSymbol() {return "Rook";}
};

class Knight: public Piece {
    using Piece::Piece;

    public:
        string getSymbol() {return "Knight";}
};

class Bishop: public Piece {
    using Piece::Piece;

    public:
        string getSymbol() {return "Bishop";}
};

class Queen: public Piece {
    using Piece::Piece;

    public:
        string getSymbol() {return "Queen";}
};

class King: public Piece {
    using Piece::Piece;

    public:
        string getSymbol() {return "King";}
};

class AbstractPieceFactory {
    public:
        virtual unique_ptr<Piece> getPiece(int row, int column) = 0;
};

class StandardPieceFactory: public AbstractPieceFactory {
    public:
        unique_ptr<Piece> getPiece(int row, int column) {
            Color color = (row <= 3) ? Color::Black : Color::White;

            if (row == 1 || row == 6 ) {
                return std::make_unique<Pawn>(color);
            }

            if (row == 0 || row == 7) {
                if (column == 0 || column == 7) {
                    return std::make_unique<Rook>(color);
                } else if (column == 1 || column == 6) {
                    return std::make_unique<Knight>(color);
                } else if (column == 2 || column == 5) {
                    return std::make_unique<Bishop>(color);
                } else if (column == 3) {
                    return std::make_unique<Queen>(color);
                } else if (column == 4) {
                    return std::make_unique<King>(color);
                }
            }

            return nullptr;
        }
};

class Board {
    public:
        Board(shared_ptr<AbstractPieceFactory> pieceFactory) {
            for (int row = 0; row < BOARD_SIZE; row++) {
                for (int col = 0; col < BOARD_SIZE; col++) {
                    this->board[row][col] = pieceFactory->getPiece(row, col);
                }
            }
        }

        void printBoard() {
            for (int row = 0; row < BOARD_SIZE; row++) {
                for (int col = 0; col < BOARD_SIZE; col++) {
                    if (this->board[row][col]) {
                        std::cout << this->board[row][col]->getSymbol() << " ";
                    } else {
                        std::cout << "--- "; 
                    }
                }
                std::cout << std::endl;
            }
        }

    protected:
        unique_ptr<Piece> board[BOARD_SIZE][BOARD_SIZE];
        Color turn;
        Result result;
};


class Renderer {
    public:
        Renderer() {}
};

class Game {
    public:
        Game(): board(std::make_shared<StandardPieceFactory>()) {}

        void run() {
            this->board.printBoard();
        }

    protected:
        Board board;
        Renderer renderer;
};



int main(int argc, char const *argv[])
{
    Game game;
    game.run();

    return 0;
}
