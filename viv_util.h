#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iomanip>


struct VivContainer
{
	std::vector<std::string> guid;
	std::vector<std::string> ordered;
	std::vector<std::string> installed;
	std::vector<std::string> ran;
	std::vector<std::string> pass;
	std::vector<std::string> dp_stat;
	std::vector<std::string> status;
	std::vector<std::string> installs;
};

struct VivReadyFileNames
{
	std::string name;
};

struct VivInput
{
	std::string file_name;
	char separator = ',';

};


int vivGetFileLength(std::ifstream* file);
void vivInitData(VivContainer* data, int num_of_elems);
bool vivMakeFileNames(VivReadyFileNames* names);
void vivSwitchParser(int* num, VivContainer* data, std::string line);
void vivGuidFormat(VivContainer* data);
void vivEmptyFieldsFiller(VivContainer* data);
void vivStatusSetter(VivContainer* data);
void vivInstallsSetter(VivContainer* data);
void vivRemoveDuplicates(VivContainer* data);
void vivFileAssembly(VivContainer* data, std::ofstream* file);
int vivRunDispo();