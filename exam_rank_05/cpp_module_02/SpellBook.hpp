
#include <iostream>
#include <map>
#include "ASpell.hpp"

class SpellBook {
public:
    SpellBook() {}
    ~SpellBook() {}

    void learnSpell(ASpell* spell) {
        sp_map[spell->getName()] = spell;
    }

    void forgetSpell(std::string const &spellName) {
        sp_map.erase(spellName);
    }

    ASpell* createSpell(std::string const &spellName) {
        if (sp_map[spellName] != 0)
            return sp_map[spellName];
        return 0;
    }
private:
    std::map<std::string, ASpell*> sp_map;
};
