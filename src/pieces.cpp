#include "pieces.h"

Piece::Piece(Color color): color(color) {}
Color Piece::getColor() {
    return this->color;
}

std::string Pawn::getSymbol() {return "Pawn";}
std::string Rook::getSymbol() {return "Rook";}
std::string Knight::getSymbol() {return "Knight";}
std::string Bishop::getSymbol() {return "Bishop";}
std::string Queen::getSymbol() {return "Queen";}
std::string King::getSymbol() {return "King";}

std::unique_ptr<Piece> StandardPieceFactory::getPiece(int row, int column) {
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