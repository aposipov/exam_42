
#pragma once
#include <iostream>
#include "ASpell.hpp"
class ASpell;

class ATarget {
public:
    ATarget(std::string type) {
        _type = type;  
    }
    virtual ~ATarget() {}

    std::string getType() { return _type; }

    void getHitBySpell(const ASpell &spell) const;
    virtual ATarget* clone() const = 0;
    
private:
    std::string _type;
};
