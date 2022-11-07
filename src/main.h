#pragma once
#ifndef HEADER_MAIN
#define HEADER_MAIN

#include "board.h"
#include "renderer.h"

class Game {
    public:
        Game();
        void run();

    protected:
        Board board;
        Renderer renderer;
        std::vector<Move> moveHistory;
};

#endif