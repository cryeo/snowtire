#pragma once
#include "Common.h"
#include "CRAutomaton.h"
#include "Socket.h"

class CRSignal {
public:
    CRSignal() {
        this->automaton = new CRAutomaton();
    }
    
    virtual ~CRSignal() {
        delete this->automaton;
    }

    void listen();

    inline States getState() {
        return this->automaton->getState();
    }

private:
    void listenFromRaspberryPi();
    void listenFromKeyboard();

    CRAutomaton *automaton;
};