#include "board.h"

BoardState::BoardState() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            this->board[row][col] = nullptr;
        }
    }
}

BoardState::BoardState(BoardState& boardState) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            Piece* piece = boardState.get({row, col});
            this->board[row][col] = std::move(this->clonePiece(piece));
        }
    }
}


std::unique_ptr<Piece> BoardState::clonePiece(Piece* piece) {
    if (!piece)
        return nullptr;
    
    int pieceId = piece->getId() % 6; 
    if (pieceId == 0) {
        return std::make_unique<Pawn>(piece->getColor());
    } else if (pieceId == 1) {
        return std::make_unique<Rook>(piece->getColor());
    } else if (pieceId == 2) {
        return std::make_unique<Knight>(piece->getColor());
    } else if (pieceId == 3) {
        return std::make_unique<Bishop>(piece->getColor());
    } else if (pieceId == 4) {
        return std::make_unique<Queen>(piece->getColor());
    } else if (pieceId == 5) {
        return std::make_unique<King>(piece->getColor());
    }

    return nullptr;
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
    // Check if the to position is legal
    std::vector<Position> legalMoves = this->generateLegalMoves(from, this->board);
    bool toMoveIsLegal = false;
    for (Position pos: legalMoves) {
        toMoveIsLegal = toMoveIsLegal || (pos == to);
    }

    if (!toMoveIsLegal) {
        return false;
    }

    // Move the piece and switch the turn
    this->board->movePiece(from, to);
    this->turn = (this->turn == Color::White) ? Color::Black : Color::White;
    return true;
}

Color Board::getTurn() {
    return this->turn;
}

std::shared_ptr<BoardState> Board::getBoardState() {
    return this->board;
}

std::vector<Position> Board::generateLegalMoves(Position from, std::shared_ptr<BoardState> board) {
    std::vector<Position> moveCandidates;
    // Ensure the moved piece exists
    Piece* fromPiece = board->get(from);
    if (!fromPiece) {
        return moveCandidates;
    }

    // Ensure the moved piece belongs to the current turn
    if (fromPiece->getColor() != this->getTurn()) {
        return moveCandidates;
    }

    moveCandidates = this->generateAllMoves(from, board);

    // Evaluate which move candidates are actually legal
    std::vector<Position> legalMoves;
    for (Position moveCandidate: moveCandidates) {
        // The candidate move is illegal if after playing it out the opponent can capture our king
        // We determine it by playing the candidate move on a copied board
        //  and checking if any of opponent moves can reach our king
        bool moveIsLegal = true;

        // Iterate over all enemy pieces
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Position enemyPiecePos = {row, col};
                Piece* enemyPiece = this->board->get(enemyPiecePos);
                
                // Omit friendly pieces and empty places
                if (!enemyPiece)
                    continue;
                if (enemyPiece->getColor() == this->turn)
                    continue;
                
                std::shared_ptr<BoardState> copiedBoard = std::make_shared<BoardState>(*this->board.get());

                // Play out the move candidate on the copied board
                copiedBoard->movePiece(from, moveCandidate); 

                // We have to check the king position there in case the king moves as the candidate
                // Unefficient: a simple optimalization would be to check only if the king moves
                //   But I will keep the code simple
                Position friendlyKingPosition = copiedBoard->getKingPosition(this->getTurn());

                // Check if any enemy move can reach our king
                std::vector<Position> enemyMoves = this->generateAllMoves(enemyPiecePos, copiedBoard);
                for (Position attackedPos: enemyMoves) {
                    moveIsLegal = moveIsLegal && !(attackedPos == friendlyKingPosition);
                }
            }
        }

        // All enemy moves checked
        if (moveIsLegal) {
            legalMoves.push_back(moveCandidate);
        }
    }
     
    return legalMoves;
}

std::vector<Position> Board::generateAllMoves(Position from, std::shared_ptr<BoardState> board) {
    // Explicitly not using this->turn in this function 
    //   in order to enable move generation for all pieces
    std::vector<Position> legalMoves;
    Piece* fromPiece = board->get(from);
    if (!fromPiece) {
        return legalMoves;
    }

    Color friendlyColor = fromPiece->getColor(); 

    auto isFriendlyPiece = [&] (Position pos) {
        return board->get(pos) && board->get(pos)->getColor() == friendlyColor;
    };

    auto isEnemyPiece = [&] (Position pos) {
        return board->get(pos) && board->get(pos)->getColor() != friendlyColor;
    };

    auto isLegalMove = [&] (Position to) {
        // Ensure the move is within board boundaries 
        //   and we're not moving to a friendly piece
        return !(to.col < 0 || to.col >= BOARD_SIZE || to.row < 0 || to.row >= BOARD_SIZE || isFriendlyPiece(to));
    };

    auto addMove = [&] (Position to) {
        if (isLegalMove(to)) {
            legalMoves.push_back(to);
        }
    };

    auto addIfSpace = [&] (Position to) {
        if (!isLegalMove(to) || isFriendlyPiece(to)) {
            return false;
        }
        
        legalMoves.push_back(to);
        return !isEnemyPiece(to);
    };  

    if (dynamic_cast<Pawn*>(fromPiece) != nullptr) {
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
        // TODO BUG: King can walk into pawns
        if (isLegalMove(leftCrosswise) && isEnemyPiece(leftCrosswise))
            addMove(leftCrosswise);
        
        if (isLegalMove(rightCrosswise) && isEnemyPiece(rightCrosswise))
            addMove(rightCrosswise);
    }
    
    // Treat the Queen as a Rook & Bishop combined
    bool isRook = dynamic_cast<Rook*>(fromPiece) != nullptr;
    bool isBishop = dynamic_cast<Bishop*>(fromPiece) != nullptr;
    if (isRook || isBishop || dynamic_cast<Queen*>(fromPiece) != nullptr) {
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
    
    if (dynamic_cast<Knight*>(fromPiece) != nullptr) {
        for (int row = -1; row <= 1; row += 2) {
            for (int col = -1; col <= 1; col += 2) {
                addMove({from.row + row * 1, from.col + col * 2});
                addMove({from.row + row * 2, from.col + col * 1});
            }
        }
    } 
    
    if (dynamic_cast<King*>(fromPiece) != nullptr) {
        for (int row = -1; row <= 1; row++)
            for (int col = -1; col <= 1; col++)
                addMove({from.row + row, from.col + col}); 
    }

    return legalMoves;
}

Position BoardState::getKingPosition(Color color) {
    // Returns {-1, -1} if the king is not found
    Position kingPosition = {-1, -1};
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            Position pos = {x, y};
            Piece* piece = this->get(pos);
            if (dynamic_cast<King*>(piece) != nullptr) {
                if (piece->getColor() == color) {
                    kingPosition = pos;
                }
            }
        }
    }
    return kingPosition;
}

Result Board::getResult() {
    // Check for invalid position
    Position friendlyKingPosition = this->board->getKingPosition(this->turn);
    if (friendlyKingPosition.col == -1) {
        return Result::InvalidPosition;
    }
        
    // 1. Does any enemy piece attack the friendly king?
    // 2. Do any of our pieces have legal moves?
    // IF  1 &  2:     check
    // IF  1 & !2:     checkmate
    // IF !1 &  2:     playing
    // IF !1 & !2:     stalemate
    
    bool friendlyKingIsAttacked = false;
    bool anyFriendlyPieceCanMove = false;

    // Check every friendly and enemy piece during a single pass over the board
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            Position piecePosition = {x, y};
            Piece* piece = this->get(piecePosition);
            if (!piece)
                continue;
            
            // Check if friendly pieces can move 
            if (piece->getColor() == this->turn) {
                std::vector<Position> legalMoves = this->generateLegalMoves(piecePosition, this->board);
                anyFriendlyPieceCanMove = anyFriendlyPieceCanMove || !legalMoves.empty();
            
            // Check if enemy pieces are attacking our king
            } else {
                std::vector<Position> attackedPositions = this->generateAllMoves(piecePosition, this->board);
                for (Position attackedPos: attackedPositions) {
                    if (attackedPos == friendlyKingPosition) {
                        friendlyKingIsAttacked = true;
                        break;
                    }
                }
            }
        }
    }

    if (friendlyKingIsAttacked)
        if (anyFriendlyPieceCanMove)
            return Result::Check;
        else
            return Result::Checkmate;
    else
        if (anyFriendlyPieceCanMove)
            return Result::Playing;
        else
            return Result::Stalemate;
}

bool Board::isFree(Position pos) {
    return !this->board->get(pos);
}

bool Board::isInsideBoard(Position pos) {
    return (
        pos.col >= 0 && pos.col < BOARD_SIZE &&
        pos.row >= 0 && pos.row < BOARD_SIZE
    );
}