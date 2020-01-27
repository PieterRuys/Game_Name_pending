#ifndef _CARD_HPP
#define _CARD_HPP
#include "gameObject.hpp"
#include "exeptions.hpp"
#include "E_lane.hpp"

#include <vector>
#include <fstream>
#include <algorithm>





std::ifstream& operator>>(std::ifstream& input, E_lane& unitLane);




class unit : public gameObject{
private:
    int unitMaxHealth;
    int unitCurrentHealth;
    int unitDamage;
    E_lane unitLane;
    bool ally = true;
public:
    unit(int unitMaxHealth, int unitDamage, E_lane unitLane, std::map<std::string, sf::Texture> textureMap):
    gameObject(sf::Vector2f(200, 100), sf::Vector2f(1,1), textureMap, std::string("unitTexture"), 5),
        unitMaxHealth(unitMaxHealth),
        unitCurrentHealth(unitMaxHealth),
        unitDamage(unitDamage),
        unitLane(unitLane){}
    
    ~unit(){}

    bool isAlly(){
        return ally;
    }

    bool checkIsDead(){
        return unitCurrentHealth <= 0;
    }

    int getDamage(){
        return unitDamage;
    }

    void takeDamage(int damage){
        unitCurrentHealth -= damage;
    }

    E_lane getLaneType(){
        return unitLane;
    }
    
    void draw(sf::RenderWindow& gameWindow) override{
        gameWindow.draw(objectSprite);
    }
    void scaleObjects(sf::Vector2f newScale){}
    void setPosition(sf::Vector2f newPosition){}

    void interact()override{}
    void move(sf::Vector2f moveDirection)override{}
    void update()override{}
    void setFrame(int maxFrame, int textureRow)override{}
};




class card : public gameObject{
protected:
    sf::Font cardFont;
    sf::Text cardName;
    sf::Text cardManaCost;
    int cardID;
public:

    card(std::string cardNameString, std::map<std::string, sf::Texture> textureMap, int cardID, int manaCost):
    gameObject(sf::Vector2f(10, 10), sf::Vector2f(1,1),  textureMap, std::string("basicCardFrame"), 5),
    cardID(cardID){
        cardFont.loadFromFile("gameAssets/cardAssets/cardFont.otf");
        cardName.setFont(cardFont);
        cardName.setString(cardNameString);
        cardName.setOrigin(cardName.getLocalBounds().left +(cardName.getLocalBounds().width / 2),cardName.getLocalBounds().top + (cardName.getLocalBounds().height / 2));
        cardName.setCharacterSize(80);
        cardName.setFillColor(sf::Color::Black);
        cardName.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.175 * objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.05 * objectSprite.getGlobalBounds().height)));
   
        cardManaCost.setFont(cardFont);
        cardManaCost.setString(std::to_string(manaCost));
        cardManaCost.setOrigin(cardManaCost.getLocalBounds().left +(cardManaCost.getLocalBounds().width / 2),cardManaCost.getLocalBounds().top + (cardManaCost.getLocalBounds().height / 2));
        cardManaCost.setCharacterSize(160);
        cardManaCost.setFillColor(sf::Color::Black);
        cardManaCost.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.85 * objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.02 * objectSprite.getGlobalBounds().height)));
   
    }

    void draw(sf::RenderWindow& gameWindow){}
    virtual void scaleObjects(sf::Vector2f newScale)=0;
    virtual void setPosition(sf::Vector2f newPosition)=0;

    int getCardID(){return cardID;}
    void interact() override {}
    void move(sf::Vector2f moveDirection) override {}
    void update() override {}
    void setFrame(int maxFrame, int textureRow) override {}
    virtual bool checkIfPlayed(sf::Vector2f mousePosition) =0;
    virtual std::shared_ptr<unit> summonUnitFromCard() =0;

};




class summonCard : public card{
private:
    int cardUnitHealth;
    int cardUnitDamage;
    sf::Font summonCardFont;
    sf::Text summonCardDamage;
    sf::Text summonCardHealth;
    sf::Sprite summonCardArtSprite;
    E_lane cardUnitLane;

public:
    summonCard(std::string cardNameString, int cardUnitDamage, int cardUnitHealth, int manaCost, E_lane cardUnitLane, std::map<std::string, sf::Texture> textureMap, int objectID):
        card(cardNameString, textureMap, objectID, manaCost),
        cardUnitHealth(cardUnitHealth),
        cardUnitDamage(cardUnitDamage),
        cardUnitLane(cardUnitLane)
    {

        auto cardPosition = objectSprite.getGlobalBounds();

        summonCardDamage.setFont(cardFont);
        summonCardDamage.setString(std::to_string(cardUnitDamage));
        summonCardDamage.setOrigin(summonCardDamage.getLocalBounds().left + (summonCardDamage.getLocalBounds().width / 2) , summonCardDamage.getLocalBounds().top +(summonCardDamage.getLocalBounds().height / 2));
        summonCardDamage.setCharacterSize(90);
        summonCardDamage.setFillColor(sf::Color::Black);
        summonCardDamage.setPosition(sf::Vector2f(cardPosition.left + (0.26* cardPosition.width) , cardPosition.top + (0.835 * cardPosition.height)));

        summonCardHealth.setFont(cardFont);
        summonCardHealth.setString(std::to_string(cardUnitHealth));
        summonCardHealth.setOrigin(summonCardHealth.getLocalBounds().left +(summonCardHealth.getLocalBounds().width / 2),summonCardHealth.getLocalBounds().top + (summonCardHealth.getLocalBounds().height / 2));
        summonCardHealth.setCharacterSize(90);
        summonCardHealth.setFillColor(sf::Color::Black);
        summonCardHealth.setPosition(sf::Vector2f(cardPosition.left + (0.68* cardPosition.width) , cardPosition.top + (0.835 * cardPosition.height)));

        summonCardArtSprite.setTexture(gameObject::textureMap["cardArtTexture"]);
        summonCardArtSprite.setPosition(objectSprite.getPosition());

        cardName.setScale(sf::Vector2f(0.24, 0.24));
        cardManaCost.setScale(sf::Vector2f(0.24, 0.24));
        objectSprite.setScale(sf::Vector2f(0.24, 0.24));
        summonCardDamage.setScale(sf::Vector2f(0.24, 0.24));
        summonCardHealth.setScale(sf::Vector2f(0.24, 0.24));
        summonCardArtSprite.setScale(sf::Vector2f(0.24, 0.24));        
    }


    void draw(sf::RenderWindow& gameWindow){
            gameWindow.draw(summonCardArtSprite);
            gameWindow.draw(objectSprite);
            gameWindow.draw(cardName);
            gameWindow.draw(summonCardDamage);
            gameWindow.draw(summonCardHealth);
            gameWindow.draw(cardManaCost);
    }

    void scaleObjects(sf::Vector2f newScale) override{

        cardName.setScale(sf::Vector2f(cardName.getScale().x * newScale.x, cardName.getScale().y * newScale.y));
        objectSprite.setScale(sf::Vector2f(objectSprite.getScale().x * newScale.x, objectSprite.getScale().y * newScale.y));
        summonCardDamage.setScale(sf::Vector2f(summonCardDamage.getScale().x * newScale.x, summonCardDamage.getScale().y * newScale.y));
        summonCardHealth.setScale(sf::Vector2f((summonCardHealth.getScale().x * newScale.x) , (summonCardHealth.getScale().y * newScale.y)));
        summonCardArtSprite.setScale(sf::Vector2f((summonCardArtSprite.getScale().x * newScale.x) , (summonCardArtSprite.getScale().y * newScale.y)));
        cardManaCost.setScale(sf::Vector2f((cardManaCost.getScale().x * newScale.x) , (cardManaCost.getScale().y * newScale.y)));

    }

    void setPosition(sf::Vector2f newPosition)override{
        objectSprite.setPosition(newPosition);
        cardName.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.175 * objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.05 * objectSprite.getGlobalBounds().height)));
        summonCardDamage.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.26* objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.835 * objectSprite.getGlobalBounds().height)));
        summonCardHealth.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.68* objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.835 * objectSprite.getGlobalBounds().height)));
        summonCardArtSprite.setPosition(objectSprite.getPosition());
        cardName.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.175 * objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.05 * objectSprite.getGlobalBounds().height)));
        cardManaCost.setPosition(sf::Vector2f(objectSprite.getGlobalBounds().left + (0.85 * objectSprite.getGlobalBounds().width) , objectSprite.getGlobalBounds().top + (0.02 * objectSprite.getGlobalBounds().height)));

    }

    bool checkIfPlayed(sf::Vector2f mousePosition){
        return (objectSprite.getGlobalBounds().contains(mousePosition));
    }

    std::shared_ptr<unit> summonUnitFromCard(){
        return std::shared_ptr<unit>(new unit(cardUnitHealth, cardUnitDamage, cardUnitLane, textureMap));
    }
};

class fightHand{
private:
    std::map<int, sf::Vector2f> handPositionMap;
    std::array<std::shared_ptr<card>, 7> cardsInHand;
    std::vector<int> &discardPile;
    sf::Texture handTexture;
    sf::Sprite handSprite;
    int cardCount = 0;
    int lastPlayedCard = 0;
public:

    fightHand(std::vector<int> &discardPile):
    discardPile(discardPile){
        handPositionMap[0] = sf::Vector2f(630, 825);            
        handPositionMap[1] = sf::Vector2f(810, 825);
        handPositionMap[2] = sf::Vector2f(990, 825);
        handPositionMap[3] = sf::Vector2f(1170, 825);
        handPositionMap[4] = sf::Vector2f(1350, 825);
        handPositionMap[5] = sf::Vector2f(1530, 825);
        handPositionMap[6] = sf::Vector2f(1710, 825);

        std::for_each(cardsInHand.begin(), cardsInHand.end(), [](auto &i){i = nullptr;});
        handTexture.loadFromFile("gameAssets/cardAssets/handParchment.png");
        handSprite.setTexture(handTexture);
        handSprite.setPosition(sf::Vector2f(400, 725));
        handSprite.setScale(sf::Vector2f(5, 3));
    }

    void drawHand(sf::RenderWindow& gameWindow){
        gameWindow.draw(handSprite);
        std::for_each(cardsInHand.begin(), cardsInHand.end(), [this, &gameWindow](auto &i){if(i != nullptr){i->draw(gameWindow);}});
    }

    int currentCardCount(){
        return cardCount;
    }

    void emptyHand(){
        if(cardCount > 0){
            std::for_each(cardsInHand.begin(), cardsInHand.end(), [this](std::shared_ptr<card>& i){if(i != nullptr){discardPile.push_back(i->getCardID());}});
            std::for_each(cardsInHand.begin(), cardsInHand.end(), [](std::shared_ptr<card> &i){std::cout<< i.use_count() << std::endl;});
            std::for_each(cardsInHand.begin(), cardsInHand.end(), [](std::shared_ptr<card> &i){i = nullptr;});
        }
        cardCount = 0;

    }

    bool addCard(std::shared_ptr<card> newCard){
        for(int i = 0; i < 7 ; i++){
            if(cardsInHand[i] == nullptr){
                cardsInHand[i] = newCard;
                newCard->setPosition(handPositionMap[i]);
                cardCount++;
                return true;
            }
        }
        return false;
    }



    int isCardClicked(sf::Vector2f mousePosition){
        if(cardCount > 0){
            for(int i = 0; i < 7 ; i++){
                if(cardsInHand[i] != nullptr){
                    if(cardsInHand[i]->checkIfPlayed(mousePosition)){
                        return i;
                    }
                }
            }
        }
        return -1;
    }

    std::shared_ptr<unit> playUnitCard(int cardPositionInHand){
        std::shared_ptr<unit> unitFromCard = cardsInHand[cardPositionInHand]->summonUnitFromCard();
        discardPile.push_back(cardsInHand[cardPositionInHand]->getCardID());
        cardsInHand[cardPositionInHand] = nullptr;
        return unitFromCard;
    }
};

enum class deckState_E {IDLE_E, DECKVIEW_E, FIGHT_E};







class deckClass{
private:
    std::vector<int> &drawPile;
    std::vector<int> &discardPile;
    std::vector<int> &completeDeck;
    std::vector<std::shared_ptr<card>> & cardsInHand;
    std::vector<std::shared_ptr<card>> deckViewerCards;
    std::map<int, sf::Vector2f> handPositionMap;
    sf::Font deckStatsFont;
    fightHand cardHand;
    deckState_E deckState = deckState_E::IDLE_E;

    sf::Texture deckViewerTexture;
    sf::Sprite deckViewerSprite;
    std::map<int, sf::Vector2f> deckViewerCardPositions;
    


public:
    sf::Text deckStats_drawPile;
    sf::Text deckStats_discardPile;
    bool fightActive = false;

    deckClass(std::vector<int>& drawPile, std::vector<int>&discardPile, std::vector<int>& completeDeck, std::vector<std::shared_ptr<card>> & cardsInHand):
        drawPile(drawPile),
        discardPile(discardPile),
        completeDeck(completeDeck),
        cardsInHand(cardsInHand),
        cardHand(discardPile)
        {

            completeDeck = {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3};

            deckStatsFont.loadFromFile("gameAssets/cardAssets/cardFont.otf");
            deckStats_drawPile.setFont(deckStatsFont);
            deckStats_drawPile.setString("DrawPile size: " + std::to_string(drawPile.size()));
            deckStats_discardPile.setFont(deckStatsFont);
            deckStats_discardPile.setString("DiscardPile size: " + std::to_string(discardPile.size()));
            
            deckStats_drawPile.setPosition(sf::Vector2f(20, 900));
            deckStats_discardPile.setPosition(sf::Vector2f(20, 1000));


            deckViewerTexture.loadFromFile("gameAssets/cardAssets/book2.png");
            deckViewerSprite.setTexture(deckViewerTexture);
            deckViewerSprite.setPosition(sf::Vector2f(100,50));
            deckViewerSprite.setScale(sf::Vector2f(0.35,0.35));
            deckViewerCardPositions[1] = sf::Vector2f(200, 100);
            deckViewerCardPositions[2] = sf::Vector2f(500, 100);
            deckViewerCardPositions[3] = sf::Vector2f(200, 600);
            }

    void newFight(){
        cardHand.emptyHand();
        drawPile.clear();
        discardPile.clear();
        drawPile = completeDeck;
        std::random_shuffle(drawPile.begin(), drawPile.end());
        deckStats_discardPile.setString("DrawPile size: " + std::to_string(discardPile.size()));
        deckStats_drawPile.setString("DrawPile size: " + std::to_string(drawPile.size()));
        fightActive = true;
        deckState = deckState_E::FIGHT_E;
    }

    void drawDeckViewer(sf::RenderWindow & gameWindow){
        gameWindow.draw(deckViewerSprite);
        std::for_each(deckViewerCards.begin(), deckViewerCards.end(), [&gameWindow](auto & i){i->draw(gameWindow);});
    }

    void startDeckViewer(){
        for(int i = 1; i < 4 ; i++ ){
            auto newViewerCard = factorCard(i);
            newViewerCard->scaleObjects(sf::Vector2f(1.3,1.3));
            newViewerCard->setPosition(deckViewerCardPositions[i]);
            deckViewerCards.push_back(newViewerCard);
        }
        changeDeckState(deckState_E::DECKVIEW_E);
    }

    void DrawHand(sf::RenderWindow& gameWindow){
        switch(deckState){
            case(deckState_E::IDLE_E):{
                break;
            }
            case(deckState_E::FIGHT_E):{
                deckStats_discardPile.setString("DiscardPile size: " + std::to_string(discardPile.size()));
                deckStats_drawPile.setString("DrawPile size: " + std::to_string(drawPile.size()));

                gameWindow.draw(deckStats_drawPile);
                gameWindow.draw(deckStats_discardPile);
                cardHand.drawHand(gameWindow);
                break;
            }

            case(deckState_E::DECKVIEW_E):{
                drawDeckViewer(gameWindow);
                break;
            }
        }
    }

    void newHand(){
        
        std::random_shuffle(drawPile.begin(), drawPile.end());
        cardHand.emptyHand();

        if(drawPile.size() < 7){
            for(int i=0; i< drawPile.size(); i++){
                cardHand.addCard(factorCard(drawPile[i]));
            }
            drawPile.clear();
            drawPile = discardPile;
            discardPile.clear();
            std::random_shuffle(drawPile.begin(), drawPile.end());
            int cardsInCurrentHand = cardHand.currentCardCount();
            std::for_each(drawPile.begin(), drawPile.begin() +(7-cardsInCurrentHand), 
                     [this](auto & i){cardHand.addCard(factorCard(i));});
            drawPile.erase(drawPile.begin(), drawPile.begin() + (7-cardsInCurrentHand) );

        }else{
            std::for_each(drawPile.begin(), drawPile.begin()+7, [this](auto & i){cardHand.addCard(factorCard(i));});
            drawPile.erase(drawPile.begin(), drawPile.begin()+7);      
        }

        deckStats_discardPile.setString("DiscardPile size: " + std::to_string(discardPile.size()));
        deckStats_drawPile.setString("DrawPile size: " + std::to_string(drawPile.size()));
    }


    std::shared_ptr<unit> checkForCardPlay(sf::Vector2i mousePosI){
        sf::Vector2f mousePosF = sf::Vector2f(float(mousePosI.x), float(mousePosI.y));
        int clickedCardPos = cardHand.isCardClicked(mousePosF);
        if(clickedCardPos > -1){
            std::shared_ptr<unit> newUnit = cardHand.playUnitCard(clickedCardPos);
            return newUnit;
        }else{
            return nullptr;
         
        }
    }

    void endDeckView(){
        changeDeckState(deckState_E::IDLE_E);
    }

    void viewDeck(){
        changeDeckState(deckState_E::DECKVIEW_E);
    }


    void changeDeckState(deckState_E newDeckState){
        deckState = newDeckState;
    }


    std::shared_ptr<card> factorCard(int cardID){
        std::ifstream cardFactoryFile("cardFactoryFile.txt");
        int objectID;
        try{
            while(true){
                cardFactoryFile>>objectID;
                if(objectID != 0){
                }
                if(objectID == cardID){
                    std::string cardType;
                    char fileBind;
                    std::string cardName;
                    std::string cardTextureFileName;

                    cardFactoryFile >> cardType >> fileBind;
                    if(!(fileBind == '{')){throw invalid_Factory_Binds("invalid Factory Bind found: " + fileBind);}
                    if(!(cardFactoryFile >> cardName)){throw end_of_file("no cardName found!");}
                    if(!(cardFactoryFile >> cardTextureFileName)){throw end_of_file("invalid textureFilename!");}
                    
                    if(cardType == "summon"){
                        E_lane cardUnitLane;
                        int cardUnitDamage;
                        int cardUnitHealth;
                        int cardManaCost;
                        std::string unitTextureFileName;
                        cardFactoryFile >> cardUnitLane >> cardUnitDamage >> fileBind >> cardUnitHealth >> fileBind >> cardManaCost >> unitTextureFileName;
                        std::map<std::string, sf::Texture> textureMap;

            
                        sf::Texture unitCardFrame;
                        if(cardUnitLane == E_lane::groundLane){
                            unitCardFrame.loadFromFile("gameAssets/cardAssets/summonCardFrame_W.png");
                        }else{
                            unitCardFrame.loadFromFile("gameAssets/cardAssets/summonCardFrame_F.png");
                        }

                        sf::Texture summonCardTexture;
                        summonCardTexture.loadFromFile(cardTextureFileName); 
                        
                        sf::Texture unitTexture;
                        unitTexture.loadFromFile(unitTextureFileName);


                        textureMap["basicCardFrame"] = unitCardFrame;
                        textureMap["cardArtTexture"] = summonCardTexture;
                        textureMap["unitTexture"] = unitTexture;

                        cardFactoryFile >> fileBind;
                        if(!(fileBind == '}')){
                            throw invalid_Factory_Binds("invalid end factory bind");
                        }
                        return std::shared_ptr<card>(new summonCard(cardName, cardUnitDamage, cardUnitHealth, cardManaCost, cardUnitLane, textureMap, objectID)); 
                    }
                }else{
                    cardFactoryFile.ignore(300, '\n');
                }
            }
        }catch(problem & e){std::cerr<< e.what() <<std::endl;}
        
        
        return nullptr;

    }

};


#endif