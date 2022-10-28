#pragma once
#ifndef HEADER_BOARD
#define HEADER_BOARD

#include "constants.h"
#include "pieces.h"


class BoardState {
    public:
        BoardState();
        BoardState(BoardState& boardState);

        void printBoard();
        Piece* get(Position pos);
        std::unique_ptr<Piece> set(Position pos, std::unique_ptr<Piece> piece);
        bool movePiece(Position from, Position to);
        std::unique_ptr<Piece> clonePiece(Piece* piece);
        Position getKingPosition(Color color);

    protected:
        std::unique_ptr<Piece> board[BOARD_SIZE][BOARD_SIZE];
};

class Board {
    public:
        Board();
        Board(std::shared_ptr<AbstractPieceFactory> pieceFactory);

        void printBoard();
        std::shared_ptr<BoardState> getBoardState();
        Piece* get(Position pos);
        Color getTurn();
        Result getResult();
        bool movePiece(Position from, Position to);
        std::vector<Position> generateLegalMoves(Position from, std::shared_ptr<BoardState> board);
        std::vector<Position> generateAllMoves(Position from, std::shared_ptr<BoardState> board);

    protected:
        bool isFree(Position pos);
        bool isInsideBoard(Position pos);

        std::shared_ptr<BoardState> board;

        Color turn = Color::White;
        Result result;
};


#endif