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

Piece* Board::get(Position pos) {
    return this->board->get(pos);
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
    std::vector<Position> legalMoves = this->generateLegalMoves(from);
    bool canMove = false;
    for (Position pos: legalMoves) {
        canMove = canMove || (pos == to);
    }

    if (canMove) {
        this->board->movePiece(from, to);
        this->turn = (this->turn == Color::White) ? Color::Black : Color::White;
    }
    
    return canMove;
}

Color Board::getTurn() {
    return this->turn;
}

std::vector<Position> Board::generateLegalMoves(Position from) {
    std::vector<Position> legalMoves;
    // Check if we're moving a piece of the current player
    if (this->isEnemyPiece(from)) {
        return legalMoves;
    }

    
    auto isLegalMove = [&] (Position to) {
        // Ensure the move is within board boundaries 
        //   and we're not moving to a friendly piece
        return !(to.col < 0 || to.col >= BOARD_SIZE || to.row < 0 || to.row >= BOARD_SIZE || this->isFriendlyPiece(to));
    };

    auto addMove = [&] (Position to) {
        if (isLegalMove(to)) {
            legalMoves.push_back(to);
        }
    };

    auto addIfSpace = [&] (Position to) {
        if (!isLegalMove(to) || this->isFriendlyPiece(to)) {
            return false;
        }
        
        legalMoves.push_back(to);
        return !this->isEnemyPiece(to);
    };  

    if (dynamic_cast<Pawn*>(this->board->get(from)) != nullptr) {
        const int startingRow = this->turn == Color::White ? 6 : 1;
        const int direction = this->turn == Color::White ? -1 : 1;

        const Position ahead = {from.row + direction, from.col};
        const Position aheadTwo = {from.row + (direction * 2), from.col};
        if (isLegalMove(ahead) && this->isFree(ahead))
            addMove(ahead);
        
        if (from.row == startingRow && isLegalMove(aheadTwo) && this->isFree(aheadTwo))
            addMove(aheadTwo);
        
        const Position leftCrosswise = {from.row + direction, from.col - 1};
        const Position rightCrosswise = {from.row + direction, from.col + 1};
        if (isLegalMove(leftCrosswise) && this->isEnemyPiece(leftCrosswise))
            addMove(leftCrosswise);
        
        if (isLegalMove(rightCrosswise) && this->isEnemyPiece(rightCrosswise))
            addMove(rightCrosswise);
    }
    
    // Treat the Queen as a Rook & Bishop combined
    bool isRook = dynamic_cast<Rook*>(this->board->get(from)) != nullptr;
    bool isBishop = dynamic_cast<Bishop*>(this->board->get(from)) != nullptr;
    if (isRook || isBishop || dynamic_cast<Queen*>(this->board->get(from)) != nullptr) {
        for (int row = -1; row <= 1; row++) {
            for (int col = -1; col <= 1; col++) {
                // Don't allow Rook diagonal moves
                if (isRook && col != 0 && row != 0)
                    continue;
                
                // Don't allow Bishop straight moves
                if (isBishop && ((col == 0) || (row == 0)))
                    continue;
                
                // Check for moving in place
                if (col == 0 && row == 0)
                    continue;
                
                for (int i = 0; i < BOARD_SIZE; i++)
                    if (!addIfSpace({from.row + row * (i + 1), from.col + col * (i + 1)}))
                        break;
            }
        }
    } 
    
    if (dynamic_cast<Knight*>(this->board->get(from)) != nullptr) {
        for (int row = -1; row <= 1; row += 2) {
            for (int col = -1; col <= 1; col += 2) {
                addMove({from.row + row * 1, from.col + col * 2});
                addMove({from.row + row * 2, from.col + col * 1});
            }
        }
    } 
    
    if (dynamic_cast<King*>(this->board->get(from)) != nullptr) {
        for (int row = -1; row <= 1; row++)
            for (int col = -1; col <= 1; col++)
                addMove({from.row + row, from.col + col}); 
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