#ifndef _CHEST_HPP
#define _CHEST_HPP

#include "gameObject.hpp"
#include "memory"
#include "objectStorage.hpp"
#include <iostream>

class chest: public gameObject{
private:
    std::vector<gameObject> lootObjectVector;
    bool open = false;
public:
    chest(sf::Vector2f spritePosition, sf::Vector2f spriteScale, std::map<std::string, sf::Texture> textureMap, std::string firstKey, int objectPriority):
        gameObject(spritePosition, spriteScale, textureMap, firstKey)
    {
        interactable = true;
        gameObject::objectPriority = objectPriority;
	    objectSprite.setTextureRect(sf::IntRect(0, 0, 20, 16));
    }

    void interact() override{
           objectSprite.setTextureRect(sf::IntRect(20, 0, 20, 16));
     
    }

    void interact(objectStorage& gameStorage, const float& mainCharacterPosition){

    }

    void draw(sf::RenderWindow& gameWindow) override{
        gameWindow.draw(objectSprite);
        if(open){
            for(auto& loot : lootObjectVector){
                loot.draw(gameWindow);
            }
        }
    }

    void move(sf::Vector2f moveDirection) override{}

    void update(){}
    void setFrame(int maxFrame, int textureRow){
    }
};

#endif