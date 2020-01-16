#ifndef _INPUTHANDLER_HPP
#define _INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include "command.hpp"
#include "interactCommand.hpp"
#include "moveCommand.hpp"
#include "objectStorage.hpp"
#include "math.h"
#include "selectedCommand.hpp"

class inputHandler{
private:
    std::array<sf::Keyboard::Key, 4>  moveKeys = {
        sf::Keyboard::Up, sf::Keyboard::Down,
        sf::Keyboard::Left, sf::Keyboard::Right
    };

    std::array<sf::Keyboard::Key, 2> interactionKeys ={
        sf::Keyboard::E,
        sf::Keyboard::Up
    };

    std::array<sf::Mouse::Button, 2> selectKeys = {
        sf::Mouse::Left, sf::Mouse::Right
    };

    objectStorage &inputStorage;

    std::shared_ptr<gameObject> mainCharacter;

    float currentDistance(std::shared_ptr<gameObject> objectPointer){
        sf::Vector2f mainCharPosition = mainCharacter->getPosition();
        sf::Vector2f objectPosition = objectPointer->getPosition();
        std::cout << "getting distance" << std::endl;
        
        return sqrt( pow(objectPosition.x - mainCharPosition.x, 2) + pow(objectPosition.y - mainCharPosition.y, 2) );
    }

    bool inRange(std::shared_ptr<gameObject> objectPointer){
        return currentDistance(objectPointer) <= 80;
    }
    
public:

    inputHandler(objectStorage &inputStorage):
        inputStorage{inputStorage}
    {
        mainCharacter = inputStorage.character;
    }

    command* handleInput(){

        // for dungeonGamestate
        for( auto movementKey : moveKeys){
            if(sf::Keyboard::isKeyPressed(movementKey)){
                std::cout << "creating move command" << std::endl;
                return new moveCommand( movementKey, inputStorage.character);
            }
        }

        //for dungeonGamestate
        for( auto interactKey : interactionKeys){
            if(sf::Keyboard::isKeyPressed(interactKey)){

                std::shared_ptr<gameObject> closestInteractablePointer = nullptr; //needs to be changed to a "nullObject" ie distance = infinite

                for(std::shared_ptr<gameObject> interactableObject : *inputStorage.game){
                    if(interactableObject->isInteractable() && inRange(interactableObject)){
                        closestInteractablePointer = interactableObject;
                    }
                }

                for(std::shared_ptr<gameObject> objectPointer : *inputStorage.game ){

                    if(objectPointer->isInteractable() && inRange(objectPointer) && (currentDistance(objectPointer) < currentDistance(closestInteractablePointer) && closestInteractablePointer != nullptr)){
                        closestInteractablePointer = objectPointer;
                    }

                }
                
                if(closestInteractablePointer != nullptr){
                    return new interactCommand(closestInteractablePointer);
                }
                else{
                    return NULL;
                }
            }
        }

        //for DungeonGameState
        for( auto automovementKey : selectKeys ){
            if(sf::Mouse::isButtonPressed(automovementKey)){

                sf::Vector2i position = sf::Mouse::getPosition();
                auto objects = inputStorage.game.get();
                for( auto object : *objects ){
                    if(object.get()->isInteractable()){
                        if( object->getPosition().x <= position.x && int(object->getPosition().x + object->getSize()) >= position.x  
                        && int(object->getPosition().y) <= position.y && int(object->getPosition().x + object->getSize()) >= position.y ){
                            return new selectedCommand(object);
                        }
                    }
                }
            }
        }


    return NULL;
    }
};


#endif