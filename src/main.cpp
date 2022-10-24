#include "main.h"

Game::Game(): board(std::make_shared<StandardPieceFactory>()) {}

sf::Vector2f _pos(Position p) {
    return sf::Vector2f(p.col * 60, p.row * 60);
}
sf::Vector2f _pos(int row, int col) {
    return sf::Vector2f(col * 60, row * 60);
}
Position _pos(sf::Vector2f v) {
    return {(int) std::floor(v.y / 60), (int) std::floor(v.x / 60)};
}

enum class GameState {
    NotSelected, Selected
};

void Game::run() {
    this->board.movePiece({7, 3}, {4, 4});

    sf::RenderWindow window(sf::VideoMode(60*8, 60*9), "Chess");
    window.setFramerateLimit(60);

    // Load textures

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

    sf::Font font;
    if (!font.loadFromFile("img/Roboto-Regular.ttf")) {
        std::cout << "Could not load font" << std::endl;
        return;
    }

    sf::Text turnText;

    turnText.setFont(font);
    turnText.setString("Turn:");
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::Black);
    turnText.setPosition({0, 60 * 8});

    sf::Sprite boardSprite;
    boardSprite.setTexture(boardTexture);
    boardSprite.setPosition(0, 0);

    std::vector<sf::Sprite> pieces;
    std::vector<sf::RectangleShape> availableMoves;

    GameState currentState = GameState::NotSelected;
    Position selectedPosition;

    auto updateBoard = [&] () {
        pieces.clear();
        availableMoves.clear();

        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Piece* piece = this->board.get({row, col});
                if (!piece)
                    continue;
                
                sf::Sprite pieceSprite;
                pieceSprite.setTexture(pieceTextures[piece->getId()]);
                pieceSprite.setPosition(_pos(row, col));
                pieces.push_back(pieceSprite);
            }
        }

        sf::Sprite currentTurn;
        currentTurn.setTexture(pieceTextures[this->board.getTurn() == Color::Black ? 6 : 0]);
        currentTurn.setPosition({60, 60 * 8});
        pieces.push_back(currentTurn);

        if (currentState == GameState::Selected) {
            std::vector<Position> legalMoves = this->board.generateLegalMoves(selectedPosition);
            legalMoves.push_back(selectedPosition);

            for (Position pos: legalMoves) {
                sf::RectangleShape move;
                move.setSize(sf::Vector2f(60, 60));
                move.setFillColor(sf::Color(255, 0, 0, 100));
                move.setPosition(_pos(pos));
                availableMoves.push_back(move);
            }
        }
        
    };
    
    updateBoard();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f v = {event.mouseButton.x, event.mouseButton.y};
                Position clickedPos = _pos(v);

                if (currentState == GameState::NotSelected) {
                    Piece* piece = this->board.get(clickedPos);
                    if (piece) {
                        selectedPosition = clickedPos;
                        currentState = GameState::Selected;
                    }
                } else if (currentState == GameState::Selected) {
                    this->board.movePiece(selectedPosition, clickedPos);
                    currentState = GameState::NotSelected;
                }

                updateBoard();
            }
        }

        window.clear(sf::Color(255, 255, 255));
        window.draw(boardSprite);
        window.draw(turnText);

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
