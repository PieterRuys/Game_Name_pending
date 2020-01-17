#ifndef _CHEST_HPP
#define _CHEST_HPP

#include "gameObject.hpp"
#include "memory"
#include "objectStorage.hpp"
#include <iostream>

class chest: public gameObject{
private:
    sf::RectangleShape chestRectangle;
    std::vector<gameObject> lootVector;
    bool open = false;
public:
    chest(sf::Vector2f position, sf::Vector2f scale, std::map<std::string, sf::Texture> textureMap, int prio):
        gameObject(position, scale, textureMap),
        chestRectangle{position}
    {
        //chestRectangle.setFillColor(sf::Color::Red);
        //chestRectangle.setPosition(position);
        //chestRectangle.setSize( sf::Vector2f(200,125) );
        interactable = true;

	    sprite.setTextureRect(sf::IntRect(0, 0, 20, 16));
        
    }

    void interact() override{
           sprite.setTextureRect(sf::IntRect(20, 0, 20, 16));
     
    }

    void interact(objectStorage& objectStorage, const float& mainCharacterPosition){

    }

    void draw(sf::RenderWindow& window) override{
        window.draw(sprite);
        if(open){
            for(auto& loot : lootVector){
                loot.draw(window);
            }
        }
    }

    void move(sf::Vector2f delta) override{}

    void update(){}
    void setFrame(int max_frame, int row){
    }
};

#endif