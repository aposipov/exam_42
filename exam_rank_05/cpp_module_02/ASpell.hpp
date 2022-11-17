
#pragma once
#include <iostream>
#include "ATarget.hpp"
class ATarget;

class ASpell {
public:
    ASpell(std::string name, std::string effects) {
        _name = name;
        _effects = effects;    
    }
    virtual ~ASpell() {}

    std::string getName() { return _name; }
    std::string getEffects() const { return _effects; }

    void launch(const ATarget &target);
    virtual ASpell* clone() const = 0;

private:
    std::string _name;
    std::string _effects;
};
