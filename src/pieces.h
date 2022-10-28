#pragma once
#ifndef HEADER_PIECES
#define HEADER_PIECES

#include "constants.h"

class Piece {
    public:
        Piece();
        Piece(Color color);
        Color getColor() const;
        virtual std::string getSymbol() = 0;
        virtual int getId() = 0;

    protected:
        Color color;
        
};

class Pawn: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
        int getId();
};

class Rook: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
        int getId();
};

class Knight: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
        int getId();
};

class Bishop: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
        int getId();
};

class Queen: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
        int getId();
};

class King: public Piece {
    using Piece::Piece;
    public:
        std::string getSymbol();
        int getId();
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