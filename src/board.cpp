#include "board.h"

Board::Board(std::shared_ptr<AbstractPieceFactory> pieceFactory) {
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