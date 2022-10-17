#pragma once
#ifndef HEADER_PIECES
#define HEADER_PIECES

#include "constants.h"

class Piece {
    public:
        Piece(Color color);
        Color getColor();
        virtual std::string getSymbol() = 0;

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

#endif