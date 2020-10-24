#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iomanip>



struct ProtectContainer
{
	std::vector<std::string> affiliate_code;
	std::vector<std::string> finish_code;
	std::vector<std::string> is_lead;
	std::vector<std::string> is_sale;
	std::vector<std::string> submitted_email;
	std::vector<std::string> beacon_range;
	std::vector<std::string> status;
};

struct ProtectReadyFileNames
{
	std::string email_name;
	std::string guid_name;
};

struct ProtectInput
{
	std::string file_name;
	char separator = ',';
	int separator_choice;
	int file_type;
};

int getIntInput(std::istream& in);
void protectInputs(ProtectInput* input_data);
bool protectMakeFileNames(ProtectReadyFileNames* names, int choice);
int protectGetFileLength(std::ifstream* file);
void protectInitData(ProtectContainer* data, int num_of_elems);
void protectSwitchParser(int* num, ProtectContainer* data, std::string line);
void protectStatusFiller(ProtectContainer* data);
void protectBeaconRangeFormatting(ProtectContainer* data);
void protectAffiliateCodeFormatting(ProtectContainer* data);
void protectStatusSetter(ProtectContainer* data);
void protectRemoveDuplicates(ProtectContainer* data);
void protectFileAssembly(ProtectContainer* data, std::ofstream* emailFile, std::ofstream* guidFile);
int protectRunDispo();