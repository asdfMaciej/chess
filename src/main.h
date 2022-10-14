#pragma once
#ifndef HEADER_MAIN
#define HEADER_MAIN

#include <iostream>
#include <memory>
#include <vector>
#include <string>

const int BOARD_SIZE = 8;

enum class Color {
    White, Black
};

enum class Result {
    Playing, Check, Checkmate, Stalemate
};

class Piece {
    public:
        Piece(Color color);
        Color getColor();
        virtual std::string getSymbol() = 0;
        //virtual bool isMoveLegal() = 0;

    protected:
        Color color;
};

class Pawn: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
};

class Rook: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
};

class Knight: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
};

class Bishop: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
};

class Queen: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
};

class King: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
};

class AbstractPieceFactory {
    public:
        virtual std::unique_ptr<Piece> getPiece(int row, int column) = 0;
};

class StandardPieceFactory: public AbstractPieceFactory {
    public:
        std::unique_ptr<Piece> getPiece(int row, int column);
};

class Renderer {
    public:
        Renderer();
};

class Board {
    public:
        Board(std::shared_ptr<AbstractPieceFactory> pieceFactory);

        void printBoard();

    protected:
        std::unique_ptr<Piece> board[BOARD_SIZE][BOARD_SIZE];
        Color turn;
        Result result;
};


class Game {
    public:
        Game();
        void run();

    protected:
        Board board;
        Renderer renderer;
};

#endif