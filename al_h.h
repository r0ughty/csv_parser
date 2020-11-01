#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iomanip>



struct AldContainer
{
	std::vector<std::string> id;
	std::vector<std::string> progress;
	std::vector<std::string> tier;
	std::vector<std::string> phone;
	std::vector<std::string> c250;
	std::vector<std::string> c500;
	std::vector<std::string> c700;
};

struct AldReadyFileNames
{
	std::string guid_name;
	std::string phone_name;
};

struct AldInput
{
	std::string file_name;
	char separator = ',';
	int separator_choice = 0;
	int file_type = 0;
};


void aldInputs(AldInput* input_data);
bool aldMakeFileNames(AldReadyFileNames* names);
int aldGetFileLength(std::ifstream* file);
void aldInitData(AldContainer* data, int num_of_elems);
void aldSwitchParser(int* num, AldContainer* data, std::string line);
void aldPhoneFiller(AldContainer* data);
void aldTierSetter(AldContainer* data);
void aldRemoveDuplicates(AldContainer* data);
void aldFileAssembly(AldContainer* data, std::ofstream* emailFile, std::ofstream* guidFile);
int aldRunDispo();

