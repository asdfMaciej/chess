#include "main.h"

Game::Game(): board(std::make_shared<StandardPieceFactory>()) {}

void Game::run() {
    Position from = {4, 4};
    this->board.movePiece({7, 3}, from);
    this->board.printBoard();
    std::vector<Position> positions = this->board.generateLegalMoves(from);
    std::cout << "From: " << from << std::endl;
    for (Position pos: positions) {
        std::cout << "To: " << pos << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    Game game;
    game.run();
    return 0;
}
