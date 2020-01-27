#ifndef _EXITCOMMAND_HPP
#define _EXITCOMMAND_HPP

#include "objectStorage.hpp"
#include "command.hpp"
#include <iostream>
#include <map>
#include "memory"

class exitCommand : public command{
private:
    objectStorage & storage;
    
public:
    exitCommand(objectStorage &storage):
    storage(storage)
    {}

    void execute() override{
        storage.setActive("menu.txt");
    }

    virtual void undo(){}
};
#endif