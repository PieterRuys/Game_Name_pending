#ifndef _FIGHT_CONTROLLER_HPP
#define _FIGHT_CONTROLLER_HPP

//#include "card.hpp"
#include "board.hpp"
#include "objectStorage.hpp"
#include "combatEnemy.hpp"
enum class E_fightState{
    win,
    loss,
    draw,
    inCombat
};

class fightController{
private:
    board gameBoard;
    int_fast8_t playerHP;
    int_fast8_t enemyHP;
    E_fightState fightState;
    int enemyMana;
    int MAX_MANA;
    fightHand& cardHand;
    sf::Texture endTurnButtonTexture;
    sf::Sprite endTurnButton;
    combatEnemy fightEnemy;
    objectStorage & storage;
    
public:
    fightController(fightHand& cardHand, objectStorage & storage): 
        playerHP{15},
        enemyHP{15},
        MAX_MANA{1},
        enemyMana{1},
        gameBoard(playerHP, enemyHP, playerMana, enemyMana),
        cardHand(cardHand),
        fightEnemy(std::string("gameAssets/skeleton.png")),
        storage(storage)
    {
        endTurnButtonTexture.loadFromFile("gameAssets/doneButton.png");
        endTurnButton.setTexture(endTurnButtonTexture);
        endTurnButton.setPosition(sf::Vector2f(10,900));
        endTurnButton.setScale(sf::Vector2f(0.25, 0.25));
        initFight();
        
    }

    int playerMana; 

    void initFight(){
        std::cout<<"initiat
                if(fightResult.selfKilled && !fightResult.opponentKilled){
                    enemyArray[fightResult.selfPosition] = enemyArray[fightResult.opponentPosition];
                    enemyArray[fightResult.opponentPosition] = nullptr;
                }
                if(fightResult.opponentKilled && !fightResult.selfKilled){
                    allyArray[fightResult.opponentPosition] = allyArray[fightResult.selfPosition];
                    allyArray[fightResult.selfPosition] = nullptr;
                }ing fight"<<std::endl;
        playerHP = 15;
        enemyHP = 15;
        playerMana =  MAX_MANA;
        enemyMana = MAX_MANA;
        gameBoard.reset();
        cardHand.newFight();
       // cardHand.newHand();
        std::cout<<"fight initiated"<<std::endl;

    }

    bool getSkyOpen(){
        return gameBoard.getSkyOpen();
    }

    bool getGroundOpen(){
        return gameBoard.getGroundOpen();
    }

    void nextTurn(){
        std::cout << "nextTurn()________________________________________________________________________" << std::endl;
        if(MAX_MANA <= 10) MAX_MANA++;
        playerMana = MAX_MANA;
        enemyMana = MAX_MANA;
        if(enemyHP <= 0){
            storage.factorNewGameState("rewardroom.txt");
            storage.setActive("rewardroom.txt");
            return;
        } 
        if(playerHP <= 0){
            storage.setActive(storage.getReturnTarget());
            return;
        }
        gameBoard.updateAlly();
        std::vector<std::shared_ptr<unit>> newEnemyUnits = fightEnemy.generateEnemyUnits();
        std::for_each(newEnemyUnits.begin(), newEnemyUnits.end(), [this](auto&i){placeUnitOnBoard(i);});
        gameBoard.updateEnemy();

        gameBoard.fightPhase();

        if(playerHP < 1 || enemyHP < 1){
            // exit(0);
        } 
        cardHand.newHand();
        std::cout << "playerMana: " << playerMana << std::endl;
    }

    bool placeUnitOnBoard(std::shared_ptr<unit> unitPointer){
        std::cout << "placing on board via fightController" << std::endl;
        bool success = gameBoard.placeUnit(unitPointer);
        std::cout << "unitPlacement: " << success << std::endl;
        return success;
    }

    bool isDoneButtonPressed(sf::Vector2i mousePosI){
        sf::Vector2f mousePosF(float(mousePosI.x), float(mousePosI.y));
        return endTurnButton.getGlobalBounds().contains(mousePosF);
    }

    void draw(sf::RenderWindow& window){
        gameBoard.draw(window);
        window.draw(endTurnButton);
        //fightEnemy.draw(window);
        
    }

    

};

#endif