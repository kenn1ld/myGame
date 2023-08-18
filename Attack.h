#ifndef ATTACK_H
#define ATTACK_H

#include <string>

class Attack {
	std::string name;
	float damage;

public:
	Attack(const std::string& n, float d); // note the change here

	void execute() const;
	std::string getName() const;
	float getDamage() const;
};

#endif