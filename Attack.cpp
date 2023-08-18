#include "Attack.h"
#include <iostream>

Attack::Attack(std::string n, float d) : name(n), damage(d) {}

void Attack::execute() {
    // Example: just print the attack for now
    std::cout << "Executing attack: " << name << " with damage: " << damage << std::endl;
}

std::string Attack::getName() const {
    return name;
}

float Attack::getDamage() const {
    return damage;
}
