#include "lane.hpp"
#include "functional"
    
    lane::lane(E_lane laneID, int_fast8_t &playerHP, int_fast8_t& enemyHP):
        laneID{laneID},
        playerHP{playerHP},
        enemyHP{enemyHP}
    {
        nullUnitPointer = std::make_shared<nullUnit>();
        allyArray = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        enemyArray = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        std::cout << "lane made" << std::endl;
    }

    bool lane::isIndexEmpty(const int index){
        return allyArray.at(index) == nullptr && enemyArray.at(index) == nullptr;
    }

    std::shared_ptr<unit> lane::getUnitPointerAtIndex(const int index){
        if(allyArray.at(index) == nullptr){
            return enemyArray.at(index);
        }
        else{
            return allyArray.at(index);
        }
    }

    void lane::placeUnit(std::shared_ptr<unit> unitPointer){
        std::cout << "lane::placeUnit()" << std::endl;
        if(unitPointer->isAlly()){
            allyArray.at(0) = unitPointer;
        }
    }

    void lane::updateLane(){
        std::cout << "entered updateLane()" << std::endl;
        std::vector<unitUpdateResult> updateResults = {};
        uint_fast8_t maxLaneIndex = LANE_SIZE - 1;

        std::function<void(std::vector<unitUpdateResult>&)> removeDeadUnitsFromResults = [&](std::vector<unitUpdateResult>& results)->void{
            std::cout << "removing dead units" << std::endl;
            // filterOutInValidResults(results);
            for(auto& result : results){
                if(result.valid == false){
                    continue;
                }

                if(result.isAlly){
                    if(result.openentKilled){
                        enemyArray[result.opponentPosition] = nullptr;
                    }
                    if(result.selfKilled){
                        allyArray[result.selfPosition] = nullptr;
                    }
                }
                else{
                    if(result.openentKilled){
                        allyArray.at(result.opponentPosition) = nullptr;
                    }
                    if(result.selfKilled){
                        enemyArray.at(result.selfPosition) = nullptr;
                    }
                }
            }
            results = {};
        };

        std::function<void(std::array<std::shared_ptr<unit>, LANE_SIZE>&, const sf::Vector2f&)> drawArray = [](std::array<std::shared_ptr<unit>, LANE_SIZE>& array, const sf::Vector2f& laneStartPosition)->void{
            sf::Vector2f drawPosition = laneStartPosition;
            for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
                if(array[i] != nullptr){
                    array[i]->jumpLocationTo(drawPosition);
                    drawPosition.x += 350;
                }
            }
        };


        // update allies
        std::cout << "hey" <<std::endl;
        for(int_fast8_t i = maxLaneIndex; i >= 0; i--){
            if(allyArray[i] != nullptr){
                std::cout << "no nullptr" << std::endl;  
                updateResults.push_back(updateUnit(i, allyArray.at(i)));
            }
        }
        removeDeadUnitsFromResults(updateResults);

        // update enemies
        std::cout << "hey2" << std::endl;
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            if(enemyArray[i] != nullptr){
                updateResults.push_back(updateUnit(i, enemyArray.at(i)));
            }
        }
        removeDeadUnitsFromResults(updateResults);

        // set drawingPosition for the units
        std::cout << "getting laneStartPosition" << std::endl;
        sf::Vector2f laneStartPosition = lanePositionMap[laneID];

        std::cout << "drawing allies" << std::endl;
        drawArray(allyArray, laneStartPosition);
        
        std::cout << "drawing enemies" << std::endl;
        drawArray(enemyArray, laneStartPosition);
    }

    void lane::filterOutInValidResults(std::vector<unitUpdateResult>& rawResults){
        std::vector<unitUpdateResult> cleanVector;
        for(const auto result : rawResults){
            if(result.valid){
                cleanVector.push_back(result);
            }
        }

        rawResults = cleanVector;
    }

    unitUpdateResult lane::updateUnit(const int index, std::shared_ptr<unit> unit){
        if(unit == nullptr){
            std::cout << "nullptr found" << std::endl;
            return unitUpdateResult(false);
        }
        
        std::function<unitUpdateResult(int, int)> updateSequence = [=](int index, int nextIndex)->unitUpdateResult{
            if(nextIndex < LANE_SIZE && isIndexEmpty(nextIndex)){
                allyArray.at(nextIndex) = unit;
                allyArray.at(index) = nullptr;
            }
            else if(index == LANE_SIZE - 1){
                enemyHP -= unit->getDamage();
            }
            else if(nextIndex < LANE_SIZE){
                unitUpdateResult result = fight(unit, enemyArray.at(nextIndex), index);

                if(result.openentKilled && !result.selfKilled){
                    allyArray.at(nextIndex) = unit;
                    allyArray.at(index) = nullptr;
                }

                return result;
            }

                return unitUpdateResult(false);
        };

        if(unit->isAlly()){
            return updateSequence(index, index + 1);
        }
        else{
            return updateSequence(index, index - 1);
        }
    }

    unitUpdateResult lane::fight(std::shared_ptr<unit> initiator, std::shared_ptr<unit> assaulted, const int index){
        assaulted->takeDamage(initiator->getDamage());
        initiator->takeDamage(assaulted->getDamage());

        int selfPosition = index;
        int opponentPosition;

        if(initiator->isAlly()){
            opponentPosition = index + 1;
        }
        else{
            opponentPosition = index - 1;
        }

        bool selfKilled = initiator->checkIsDead();
        bool opponentKilled = assaulted->checkIsDead();

        return unitUpdateResult(true, selfPosition, opponentPosition ,opponentKilled, selfKilled, initiator->isAlly());
    }
    
    void lane::updateAllUnits(){
        std::vector<unitUpdateResult> updateResults = {};
        
        // update allies
        for(int_fast8_t i = LANE_SIZE -1; i >= 0; i--){
            updateResults.push_back(updateUnit(i, allyArray.at(i)));
        }

        // filter result of fights and act on it
        filterOutInValidResults(updateResults);
        for(auto& result : updateResults){
            if(result.openentKilled){
                enemyArray.at(result.opponentPosition) = nullptr;
            }
            if(result.selfKilled){
                allyArray.at(result.selfPosition) = nullptr;
            }
        }

        // update enemies
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            updateResults.push_back(updateUnit(i, enemyArray.at(i)));
        }

        // filter result of fights and act on it
        filterOutInValidResults(updateResults);
        for(auto& result : updateResults){
            if(result.openentKilled){
                allyArray.at(result.opponentPosition) = nullptr;
            }
            if(result.selfKilled){
                enemyArray.at(result.selfPosition) = nullptr;
            }
        }

        // set positions of the unit sprites
        sf::Vector2f laneStartPosition = lanePositionMap[laneID];

        sf::Vector2f drawPosition = laneStartPosition;
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            allyArray.at(i)->jumpLocationTo(drawPosition);
            drawPosition.x += 350;
        }
        
        drawPosition = laneStartPosition;
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            enemyArray.at(i)->jumpLocationTo(drawPosition);
            drawPosition.x += 350;
        }
    }

    void lane::removeAtIndex(const int index){
        allyArray.at(index) = nullptr;
        enemyArray.at(index) = nullptr;
    }

    void lane::removeByID(const std::string& id){
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            if(allyArray.at(i)->getObjectID() == id){
                allyArray.at(i) = nullptr;
            }

            if(enemyArray.at(i)->getObjectID() == id){
                enemyArray.at(i) = nullptr;
            }
        }
    }

    std::shared_ptr<unit> lane::getUnitPointerByID(const std::string& id){
        for(auto& unit : allyArray){
            if(unit->getObjectID() == id){
                return unit;
            }
        }
        for(auto& unit : enemyArray){
            if(unit->getObjectID() == id){
                return unit;
            }
        }
        return nullptr;
    }

    void lane::draw(sf::RenderWindow& window){
        std::cout << "drawing board" << std::endl;
        sf::Vector2f laneStartPosition = lanePositionMap[laneID];

        sf::Vector2f drawPosition = laneStartPosition;
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            if(allyArray.at(i) != nullptr){
                allyArray.at(i)->jumpLocationTo(drawPosition);
                allyArray.at(i)->draw(window);
            }
            drawPosition.x += 350;
        }
        
        drawPosition = laneStartPosition;
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            if(allyArray.at(i) != nullptr){
                enemyArray.at(i)->jumpLocationTo(drawPosition);
                enemyArray.at(i)->draw(window);
            }
            drawPosition.x += 350;
        }
    }
