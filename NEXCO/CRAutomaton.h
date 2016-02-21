#pragma once

enum States { INITIAL = 0, TIRE_IN = 1, TIRE_OUT = 2 };

class CRAutomaton {
public:
    CRAutomaton() : _state(States::INITIAL) {
        // current_state, action
        this->transition[States::INITIAL][0] = States::TIRE_IN;
        this->transition[States::INITIAL][1] = States::INITIAL;
        this->transition[States::TIRE_IN][0] = States::INITIAL;
        this->transition[States::TIRE_IN][1] = States::TIRE_OUT;
        this->transition[States::TIRE_OUT][0] = States::TIRE_IN;
        this->transition[States::TIRE_OUT][1] = States::INITIAL;
    }

    virtual ~CRAutomaton() {}

    inline void transit(char _action) {
        // front_switch : 0, rear_switch : 1
        switch (_action) {
        case 'f':
        case 'r':
            int action = (_action == 'f') ? 0 : 1;
            this->_state = this->transition[this->_state][action];
            break;
        }
    }

    inline States getState() {
        return this->_state;
    }

private:
    States transition[3][2];
    States _state;
};