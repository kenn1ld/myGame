#ifndef ATTACK_H
#define ATTACK_H

#include <string>

class Attack {
    std::string name;
    float damage;

public:
    Attack(std::string n, float d);

    void execute();
    std::string getName() const;
    float getDamage() const;
};

#endif
