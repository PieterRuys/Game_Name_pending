#ifndef _CARD_SELECT_COMMAND_HPP
#define _CARD_SELECT_COMMAND_HPP

#include "command.hpp"
#include "fightController.hpp"

class cardSelectCommand : public command{
private:
    fightController& controlPointer;
    std::shared_ptr<unit> unitPointer;
public:
    cardSelectCommand(fightController& controlPointer, std::shared_ptr<unit> unitPointer):
        controlPointer{controlPointer},
        unitPointer{unitPointer}
    {
    }

    void execute() override{
        
        controlPointer.placeUnitOnBoard(unitPointer);
    }

    void undo() override{}

};

#endif