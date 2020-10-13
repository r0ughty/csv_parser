#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iomanip>



struct Container
{
	std::vector<std::string> affiliate_code;
	std::vector<std::string> finish_code;
	std::vector<std::string> is_lead;
	std::vector<std::string> is_sale;
	std::vector<std::string> submitted_email;
	std::vector<std::string> beacon_range;
	std::vector<std::string> status;
};

struct ReadyFileNames
{
	std::string email_name;
	std::string guid_name;
};

struct Input
{
	std::string file_name;
	char separator = ',';
	int separator_choice = 0;
	int file_type = 0;
};

void Inputs(Input* input);
bool makeFileNames(ReadyFileNames* names, int choice);
int getFileLength(std::ifstream* file);
void initData(Container* data, int num_of_elems);
void switchParser(int* num, Container* data, std::string line);
void statusFiller(Container* data);
void beaconRangeFormatting(Container* data);
void affiliateCodeFormatting(Container* data);
void statusSetter(Container* data);
void removeDuplicates(Container* data);
void fileAssembly(Container* data, std::ofstream* emailFile, std::ofstream* guidFile);