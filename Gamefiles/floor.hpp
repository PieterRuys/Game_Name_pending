#ifndef _FLOOR_HPP
#define _FLOOR_HPP
#include <SFML/Graphics.hpp>
#include "gameObject.hpp"
#include <iostream>
#include "collisionBox.hpp"
class floorObject : public gameObject {
public:

	floorObject( sf::Vector2f position, sf::Vector2f size, std::string textureFile):
	    position( position ),
	    size( size ){
            objectRectangle.setPosition(position);
            objectRectangle.setSize(size);
            auto i = new sf::Texture;
            i->loadFromFile(textureFile);
            objectRectangle.setTexture(i);
            floorBoxes.setBoxes(objectRectangle.getGlobalBounds());
        };
	~floorObject(){}

	void move( sf::Vector2f delta) override{
		std::cout<<"move function called" << std::endl;
        position+= delta;
        objectRectangle.setPosition(objectRectangle.getPosition() + delta);
        floorBoxes.move(delta);
    };
	void draw( sf::RenderWindow & window ){
        window.draw(objectRectangle);
        floorBoxes.draw(window);
    };

private:
	sf::Vector2f position;
	sf::Vector2f size;
	sf::RectangleShape objectRectangle;
    CollisionBox floorBoxes;
};

#endif