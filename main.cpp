#include "protect_h.h"
#include "viv_h.h"
#include "utility.h"
#include "al_h.h"
#include "mq_h.h"

void menu()
{
	std::cout << "=======BEST MENU YOU'VE EVER SEEN=======\n" << std::endl;
	std::cout << "1. Alder" << std::endl;
	std::cout << "2. Magic Quote" << std::endl;
	std::cout << "3. Protect" << std::endl;
	std::cout << "4. Vivint" << std::endl;
	std::cout << "0. Exit" << std::endl;
}



int main()
{
	int choice;
	int loop = 1;


	while (loop)
	{ 
		menu();
		std::cout << "Input: ";
		choice = getIntInput(std::cin);
		while (choice < 0 || choice > 4)
		{
			std::cout << "Wrong input." << std::endl;
			std::cout << "Input: ";
			choice = getIntInput(std::cin);
		}

		switch (choice)
		{
		case 1:
			aldRunDispo();
			break;
		case 2:
			mqRunDispo();;
			break;
		case 3:
			protectRunDispo();			
			break;
		case 4:
			vivRunDispo();
			break;
		case 0:
			loop = 0;
			break;
		default:
			break;
		}	
	}

	


	return 0;
}