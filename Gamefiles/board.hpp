#ifndef _BOARD_HPP
#define _BOARD_HPP

#include "lane.hpp"

class board {
private:
    /// \brief
    /// All lanes, currently is the third lane, the trap lane, not implemented.
    lane lanes[3];

    /// \brief
    /// Not used. Nessesry for future buff systems.
    E_lane priorityLane;

    /// \brief
    /// Tracks players hp so the lanes can update the current hp
    int_fast8_t & playerHP;

    /// \brief
    /// Tracks enemy hp so the lanes can update the current hp
    int_fast8_t & enemyHP;

    int & playerMana;

    int & enemyMana;

    sf::Font mana_healthFont;
    sf::Text playerManaText;
    sf::Text playerHealthText;
    sf::Texture manaBarTexture;
    sf::Texture healthBarTexture;
    sf::Texture statsUITexture;
    sf::Texture enemyHPTexture;
    sf::Sprite manaBarSprite;
    sf::Sprite healthBarSprite;
    sf::Sprite statsUISprite;
    sf::Sprite enemyHPSprite;


public:

    /// \brief
    /// The board constructor.
    /// \details
    /// The constructor needs playerHP and enemyHP, both by reference. 
    board(int_fast8_t & playerHP, int_fast8_t & enemyHP, int & playerMana, int & enemyMana);

    /// \brief
    /// Returns true if the first place in the sky lane is open to place a card on.
    bool getSkyOpen();

    /// \brief
    /// Returns true if the first place in the ground lane is open to place a card on.
    bool getGroundOpen();

    /// \brief
    /// Updates the board to resolve fights and move cards forward one tick. Called once every turn.
    void update();

    void updateStatsUI();

    /// \brief
    /// places a unit on the field and returns whether operation was succesfull.
    bool placeUnit(std::shared_ptr<unit> unitPointer);

    /// \brief
    /// Returns whether a specific place on the board is empty.
    bool isPositionEmpty(const int E_lane, const int index);

    /// \brief
    /// Gets called every frame to draw all the board.
    void draw(sf::RenderWindow& window);

    /// \brief
    /// gets called at the start of the cardgame, to ensure a clean en ready board.
    void reset();
};

#endif