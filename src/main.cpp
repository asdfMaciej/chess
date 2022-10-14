#include "main.h"

Game::Game(): board(std::make_shared<StandardPieceFactory>()) {}

void Game::run() {
    this->board.printBoard();
}

int main(int argc, char const *argv[])
{
    Game game;
    game.run();
    return 0;
}
