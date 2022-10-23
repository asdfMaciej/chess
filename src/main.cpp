#include "main.h"

Game::Game(): board(std::make_shared<StandardPieceFactory>()) {}

void Game::run() {
    this->board.movePiece({7, 3}, {4, 4});
    
    sf::RenderWindow window(sf::VideoMode(480, 480), "Chess");

    std::string piecePaths[12] = {
        "img/WPawn.png", "img/WRook.png", "img/WKnight.png", "img/WBishop.png", "img/WQueen.png", "img/WKing.png",
        "img/BPawn.png", "img/BRook.png", "img/BKnight.png", "img/BBishop.png", "img/BQueen.png", "img/BKing.png"
    };
    sf::Texture pieceTextures[12];

    std::string boardPath = "img/Board.png";
    sf::Texture boardTexture;

    if (!boardTexture.loadFromFile("img/Board.png")) {
        std::cout << "Could not load texture" << std::endl;
        return;
    }

    for (int i = 0; i < 12; i++) {
        if(!pieceTextures[i].loadFromFile(piecePaths[i])) {
            std::cout << "Could not load texture" << std::endl;
            return;
        }
    }

    sf::Sprite boardSprite;
    boardSprite.setTexture(boardTexture);
    boardSprite.setPosition(0, 0);

    std::vector<sf::Sprite> pieces;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            Piece* piece = this->board.get({row, col});
            if (!piece)
                continue;
            
            sf::Sprite pieceSprite;
            pieceSprite.setTexture(pieceTextures[piece->getId()]);
            pieceSprite.setPosition(col*60, row*60);
            pieces.push_back(pieceSprite);
        }
    }

    
    Position startPos = {4, 4};
    
    std::vector<Position> legalMoves = this->board.generateLegalMoves(startPos);
    legalMoves.push_back(startPos);

    std::vector<sf::RectangleShape> availableMoves;
    for (Position pos: legalMoves) {
        sf::RectangleShape move;
        move.setSize(sf::Vector2f(60, 60));
        move.setFillColor(sf::Color(255, 0, 0, 100));
        move.setPosition(pos.col*60, pos.row*60);
        availableMoves.push_back(move);
    }
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(boardSprite);

        for (sf::RectangleShape sprite: availableMoves) {
            window.draw(sprite);
        }

        for (sf::Sprite sprite: pieces) {
            window.draw(sprite);
        }

        window.display();
    }
}


int main(int argc, char const *argv[])
{
    Game game;
    game.run();

    // Pawn copied = pawn;
    return 0;
}
