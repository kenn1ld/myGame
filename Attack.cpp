#include "Attack.h"
#include <iostream>
#include "Logger.h"
Attack::Attack(const std::string& n, float d) : name(n), damage(d) {}

void Attack::execute() const{
	// Example: just print the attack for now
	Logger::console->info("Executing attack: " + name + " with damage: " + std::to_string(damage));
}

std::string Attack::getName() const {
	return name;
}

float Attack::getDamage() const {
	return damage;
}