#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

int getIntInput(std::istream& in);
std::string getStrInput(std::istream& in);
bool fileValidity(std::ifstream* file, std::string checker_line);