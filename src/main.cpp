// Copyright 2022 Maciej Kaszkowiak (maciej@kaszkowiak.org)
// Apache License 2.0
#include "main.h"

/* 
C++11 features: 
- unique_ptr, shared_ptr
- initalizer lists 
- constructor inheritance

C++14:
- make_unique

OOP techniques used:
- dependency injection
- inheritance
- interfaces (abstract classes) 
- object factories
*/

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
