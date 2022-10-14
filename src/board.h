#pragma once
#ifndef HEADER_BOARD
#define HEADER_BOARD

#include "constants.h"
#include "pieces.h"

class Board {
    public:
        Board(std::shared_ptr<AbstractPieceFactory> pieceFactory);

        void printBoard();

    protected:
        std::unique_ptr<Piece> board[BOARD_SIZE][BOARD_SIZE];
        Color turn;
        Result result;
};

#endif