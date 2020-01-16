#include "game.hpp"

void game::gameLoop(){

	//objectRenderer.factorObjects();
	auto defaultSize = sf::Vector2f((float)window.getSize().x, (float)window.getSize().y);
	auto lastSize = defaultSize;

    while (window.isOpen()) {
		time(&timerCurrent);
		auto elapsed = timerCurrent - timerPrevious;
		timerPrevious = timerCurrent;
		lag += elapsed;
		

		command* newCommand;
		newCommand  = gameInputHandler.handleInput();
		if(newCommand != NULL){
			newCommand->execute();
		}
		delete newCommand;

		while(lag >= MS_PER_FRAME){

			lag -= MS_PER_FRAME;
		}


        window.clear();
        objectRenderer.update();
        objectRenderer.draw();
		window.display();

        sf::Event event;		
	    while( window.pollEvent(event) ){
			if( event.type == sf::Event::Closed ){
				window.close();
			}else if(event.type == sf::Event::Resized){
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				for (auto & i : storage.allVectors)
				{
					for(auto & j : *i.second){
						j->resize(sf::Vector2f(event.size.width, event.size.height), defaultSize, lastSize);
						lastSize = sf::Vector2f(event.size.width, event.size.height);
					}
				}
        		window.setView(sf::View(visibleArea));
			}
		}
	}
}