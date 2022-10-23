#include "pieces.h"

Piece::Piece(Color color): color(color) {}
Color Piece::getColor() const {
    return this->color;
}

std::string Pawn::getSymbol() {return "Pawn";}
std::string Rook::getSymbol() {return "Rook";}
std::string Knight::getSymbol() {return "Knight";}
std::string Bishop::getSymbol() {return "Bishop";}
std::string Queen::getSymbol() {return "Queen";}
std::string King::getSymbol() {return "King";}

/*Piece* Pawn::clone() const {return new Pawn(*this);}
Piece* Rook::clone() const {return new Rook(*this);}
Piece* Knight::clone() const {return new Knight(*this);}
Piece* Bishop::clone() const {return new Bishop(*this);}
Piece* Queen::clone() const {return new Queen(*this);}
Piece* King::clone() const {return new King(*this);}*/

Piece::Piece() {}
Pawn::Pawn(const Pawn& other) {this->color = other.color;}
Rook::Rook(const Rook& other) {this->color = other.color;}
Knight::Knight(const Knight& other) {this->color = other.color;}
Bishop::Bishop(const Bishop& other) {this->color = other.color;}
Queen::Queen(const Queen& other) {this->color = other.color;}
King::King(const King& other) {this->color = other.color;}

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