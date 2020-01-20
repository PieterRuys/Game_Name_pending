#ifndef _BOARD_HPP
#define _BOARD_HPP

#include "memory"
#include "gameObject.hpp"
#include "card.hpp"

#define LANE_SIZE 7

enum E_lane{
  skyLane, groundLane, trapLane  
};

class lane{
private:
    std::shared_ptr<gameObject> laneArray[LANE_SIZE];
    std::vector<std::shared_ptr<gameObject>> laneEffects[LANE_SIZE];
public:
    lane():
        laneArray{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
    {}

    bool isIndexEmpty(const int index){
        return laneArray[index] == nullptr;
    }

    std::shared_ptr<gameObject> getUnitPointerAtIndex(const int index){
        return laneArray[index];
    }

    void placeUnit(const int index, std::shared_ptr<gameObject> unitPointer){
        laneArray[index] = unitPointer;
    }

    void placeEffect(const int index, std::shared_ptr<gameObject> effectPointer){
        laneEffects[index].push_back(effectPointer);
    }

    void updateLane(board* boardPointer){
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            laneArray[i]->update(i, this, boardPointer);
        }

        //for every position
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            //for every spell on that position
            for(uint_fast8_t j = 0; j < laneEffects[i].size(); j++){
                laneEffects[i][j]->update(i, j, this);
            }
        }
    }
    
    void updateUnits(board* boardPointer){
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            laneArray[i]->update(i, this, boardPointer);
        }
    }

    void updateEffects(board* boardPointer){
        //for every position
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            //for every spell on that position
            for(uint_fast8_t j = 0; j < laneEffects[i].size(); j++){
                laneEffects[i][j]->update(i, j, this);
            }
        }
    }

    void removeAtIndex(const int index){
        laneArray[index] = nullptr;
    }

    void removeEffectAtIndex(const int positionIndex, const int effectIndex){
        auto toRemoveEffect = laneEffects[positionIndex][effectIndex];
        laneEffects[positionIndex][effectIndex] = laneEffects[positionIndex].back();
        laneEffects[positionIndex][laneEffects[positionIndex].size() - 1] = toRemoveEffect;
        laneEffects[positionIndex].pop_back();
    }

    void removeByID(const std::string& id){
        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            if(laneArray[i]->getObjectID() == id){
                laneArray[i] = nullptr;
            }
        }

        for(uint_fast8_t i = 0; i < LANE_SIZE; i++){
            for(uint_fast8_t j = 0; j < laneEffects[i].size(); j++){
                if(laneEffects[i][j]->getObjectID() == id){
                    laneEffects[i][j] = nullptr;
                }
            }
        }
    }
};


class board {
private:
    lane lanes[3];
public:
    board():
        lanes{lane(), lane(), lane()}
    {}

    void update(){
        for(lane& currentLane : lanes){
            currentLane.updateLane(this);
        }
    }

    void updateUnits(const int E_lane){
        for(lane& currentLane : lanes){
            currentLane.updateUnits(this);
        }
    }

    void updateEffects(const int E_lane){
        for(lane& currentLane : lanes){
            currentLane.updateLane(this);
        }
    }

    void updateLane(const int E_lane){
        lanes[E_lane].updateLane(this);
    }

    void updateUnitsOnLane(const int E_lane){
        lanes[E_lane].updateUnits(this);
    }

    void updateEffectsOnLane(const int E_lane){
        lanes[E_lane].updateEffects(this);
    }

    void placeUnit(const int E_lane, const int laneIndex, std::shared_ptr<gameObject> unitPointer){
        lanes[E_lane].placeUnit(laneIndex, unitPointer);
    }
    
    void placeTrapcard(const int index, std::shared_ptr<gameObject> trapcardPointer){
        lanes[E_lane::trapLane].placeUnit(index, trapcardPointer);
    }

    void placeEffect(const int E_lane, const int index, std::shared_ptr<gameObject> effectPointer){
        lanes[E_lane].placeEffect(index, effectPointer);
    }

    void castSpell(const int E_lane, const int index, std::shared_ptr<gameObject> spell){
        spell.activate(&lanes, E_lane, index);
    }

    void removeUnit(const int E_lane, const int index){
        lanes[E_lane].removeAtIndex(index);
    }

    void removeEffect(const int E_lane, const int index, const int effectIndex){
        lanes[E_lane].removeEffectAtIndex(index, effectIndex);
    }

    void removeByID(const std::string& id){
        for(lane& currentLane : lanes){
            currentLane.removeByID(id);
        }
    }

    std::shared_ptr<gameObject> getUnitPointer(const int E_lane, const int laneIndex){
        return lanes[E_lane].getUnitPointerAtIndex(laneIndex);
    }

    bool isPositionEmpty(const int E_lane, const int index){
        return lanes[E_lane].isIndexEmpty(index);
    }

    bool isTrapCardOnPosition(const int index){
        return lanes[E_lane::trapLane].isIndexEmpty(index);
    }
};

#endif