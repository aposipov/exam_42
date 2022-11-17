
#pragma once
#include <iostream>
// #include <map>
#include "ASpell.hpp"
#include "ATarget.hpp"
#include "SpellBook.hpp"

class Warlock {
public:
    Warlock(std::string name, std::string title) {
        _name = name;
        _title = title;
        std::cout << _name << ": This looks like another boring day." << std::endl;
    }

    ~Warlock() {
        std::cout << _name << ": My job here is done!" << std::endl;
    }

    const std::string &getName() const { return _name; }
    const std::string &getTitle() const { return _title; }
    void setTitle(const std::string &title) { _title = title ;}

    void introduce() const {
        std::cout << _name << ": I am " << _name << ", " << _title << "!" << std::endl;
    }

    void learnSpell(ASpell* spell) {
        // map[spell->getName()] = spell;
        book.learnSpell(spell);
    }

    void forgetSpell(std::string spellName) {
        // map.erase(spellName);
        book.forgetSpell(spellName);
    }

    void launchSpell(std::string spellName, ATarget &target) {
        // if (map[spellName] != 0)
            // map[spellName]->launch(target);
        ASpell* tmp = book.createSpell(spellName);
        if (tmp)
            tmp->launch(target);
    }

private:
    std::string _name;
    std::string _title;
    // std::map<std::string, ASpell*> map;
    SpellBook book;
};
