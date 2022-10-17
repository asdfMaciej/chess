#include "board.h"

BoardState::BoardState() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            this->board[row][col] = nullptr;
        }
    }
}

void BoardState::printBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col]) {
                std::cout << this->board[row][col]->getSymbol() << " ";
            } else {
                std::cout << "--- "; 
            }
        }
        std::cout << std::endl;
    }
}

std::unique_ptr<Piece> BoardState::set(Position pos, std::unique_ptr<Piece> piece) {
    std::unique_ptr<Piece> old = nullptr;
    if (this->board[pos.row][pos.col] == nullptr) {
        old = std::move(this->board[pos.row][pos.col]);
    }
    
    if (piece) {
        this->board[pos.row][pos.col] = std::move(piece); 
    } else {
        this->board[pos.row][pos.col] = nullptr;
    }

    return old;
}

Piece* BoardState::get(Position pos) {
    if (!this->board[pos.row][pos.col]) {
        return nullptr;
    }
    return this->board[pos.row][pos.col].get();
}

bool BoardState::movePiece(Position from, Position to) {
    this->board[to.row][to.col] = std::move(this->board[from.row][from.col]);
    return true;
}


Board::Board(std::shared_ptr<AbstractPieceFactory> pieceFactory) {
    this->board = std::make_shared<BoardState>();
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            std::unique_ptr<Piece> piece = std::move(pieceFactory->getPiece(row, col));
            this->board->set({row, col}, std::move(piece));
        }
    }
}

Board::Board() {
    this->board = std::make_shared<BoardState>();
}
void Board::printBoard() {
    this->board->printBoard();
}


bool Board::movePiece(Position from, Position to) {
    return this->board->movePiece(from, to);
}


std::vector<Position> Board::generateLegalMoves(Position from) {
    std::vector<Position> legalMoves;
    // Check if we're moving a piece of the current player
    if (!this->isFriendlyPiece(from)) {
        return legalMoves;
    }

    return legalMoves;
}

bool Board::isFree(Position pos) {
    return !this->board->get(pos);
}

bool Board::isFriendlyPiece(Position pos) {
    return (
        this->board->get(pos) &&
        this->board->get(pos)->getColor() == this->turn
    );
}

bool Board::isEnemyPiece(Position pos) {
    return (
        this->board->get(pos) &&
        this->board->get(pos)->getColor() != this->turn
    );
}
bool Board::isInsideBoard(Position pos) {
    return (
        pos.col >= 0 && pos.col < BOARD_SIZE &&
        pos.row >= 0 && pos.row < BOARD_SIZE
    );
}