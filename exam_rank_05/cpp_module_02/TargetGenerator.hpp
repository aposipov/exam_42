
#pragma once
#include <iostream>
#include <map>
#include "ATarget.hpp"

class TargetGenerator {
public:
    TargetGenerator() {}
    ~TargetGenerator() {}

    void learnTargetType(ATarget* target) {
        tar_map[target->getType()] = target;
    }

    void forgetTargetType(std::string const &targetType) {
        tar_map.erase(targetType);
    }

    ATarget* createTarget(std::string const &targetType) {
        if (tar_map[targetType] != 0)
            return tar_map[targetType];
        return 0;
    }
private:
    std::map<std::string, ATarget*> tar_map;
};
