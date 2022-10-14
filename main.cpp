#include <iostream>

enum class Color {
    White, Black
};

enum class GameState {
    Playing, Check, Checkmate, Stalemate
};

class Piece {
    public:
        virtual bool isMoveLegal();

    protected:
        Color color;
};

class Rook: Piece {};
class Knight: Piece {};
class Bishop: Piece {};
class Queen: Piece {};
class King: Piece {};

class Board {};
class Renderer {};

class Game {

    protected:
        Board board;
        Renderer renderer;
};



int main(int argc, char const *argv[])
{
    std::cout << "test" << std::endl;
    /* code */
    return 0;
}
