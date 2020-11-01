#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <iomanip>



struct MqContainer
{
	std::vector<std::string> id;
	std::vector<std::string> sms_valid;
	std::vector<std::string> email_valid;
	std::vector<std::string> sms_click;
	std::vector<std::string> email_click;
	std::vector<std::string> buy;
	std::vector<std::string> out;
	std::vector<std::string> delivered;
	std::vector<std::string> status;
};

struct MqReadyFileNames
{
	std::string name;
};

struct MqInput
{
	std::string file_name;
	char separator = ',';
};

void mqInputs(MqInput* input_data);
bool mqMakeFileNames(MqReadyFileNames* names);
int mqGetFileLength(std::ifstream* file);
void mqInitData(MqContainer* data, int num_of_elems);
void mqSwitchParser(int* num, MqContainer* data, std::string line);
void mqIdSetter(MqContainer* data);
void mqSmsClickSetter(MqContainer* data);
void mqDeliverSetter(MqContainer* data);
void mqStatusSetter(MqContainer* data);
void mqRemoveDuplicates(MqContainer* data);
void protectFileAssembly(MqContainer* data, std::ofstream* file);
int mqRunDispo();