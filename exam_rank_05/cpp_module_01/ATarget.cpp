
#include "ATarget.hpp"

void ATarget::getHitBySpell(const ASpell &spell) const
{
    std::cout << _type << " has been " << spell.getEffects() << "!" << std::endl;
}
