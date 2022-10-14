// Copyright 2022 Maciej Kaszkowiak (maciej@kaszkowiak.org)
// Apache License 2.0
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "main.h"

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




Piece::Piece(Color color): color(color) {}
Color Piece::getColor() {
    return this->color;
}

string Pawn::getSymbol() {return "Pawn";}
string Rook::getSymbol() {return "Rook";}
string Knight::getSymbol() {return "Knight";}
string Bishop::getSymbol() {return "Bishop";}
string Queen::getSymbol() {return "Queen";}
string King::getSymbol() {return "King";}

unique_ptr<Piece> StandardPieceFactory::getPiece(int row, int column) {
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

Renderer::Renderer() {}


Board::Board(shared_ptr<AbstractPieceFactory> pieceFactory) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            this->board[row][col] = pieceFactory->getPiece(row, col);
        }
    }
}

void Board::printBoard() {
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

Game::Game(): board(std::make_shared<StandardPieceFactory>()) {}
void Game::run() {
    this->board.printBoard();
}



int main(int argc, char const *argv[])
{
    Game game;
    game.run();
    return 0;
}
