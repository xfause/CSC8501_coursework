#include <iostream>
#include <any>
#include <thread>
#include "Menu.h"
#include "Encrypt.h"
#include "BranchOne.cpp"
#include "BranchTwo.cpp"
#include "BranchThree.cpp"
#include "BranchFour.cpp"


int generateStepSize = 100;
int generateGroupSize = 10000;

inline void inlineFunc() {
	std::cout << std::endl;
}

int main()
{
	//A friend to all is a friend to none;
	// abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghij
	//27322810313331033910211452912207344136146925461033281533271031012815108114101
	//clock_t processStart, procesEnd;
	//processStart = clock();
	//std::string testStr = "934";
	//std::string plainASCII = "84|30";
	////std::string str = EncryptPassword(testStr);
	//DecryptPassword(testStr, plainASCII, 2, 3, true);
	//procesEnd = clock();
	//std::cout << (double)(procesEnd - processStart) / CLOCKS_PER_SEC<<"s" << std::endl;
	//return 0;

	std::thread t(inlineFunc);
	t.detach();


	auto bm = BaseMenu();
	auto om1 = OptionMenu_1();
	auto om2 = OptionMenu_2();
	auto om3 = OptionMenu_3();
	auto om4 = OptionMenu_4();

	MenuDisplay(bm);
	while (1) {
		std::any opCode;
		int tmp;
		std::cin >> tmp;
		opCode = tmp;

		if (std::any_cast<int>(opCode) == 1) {
			MenuDisplay(om1);
			if (SaveUsernameAndPassword(om1, bm)) {
				MenuDisplay(om1, "success");
				MenuDisplay(bm);
			}
			else {
				MenuDisplay(om1, "fail");
				MenuDisplay(bm);
			}
		}
		else if (std::any_cast<int>(opCode) == 2) {
			Op2CheckPasswordOfUser(om2, bm);
		}
		else if (std::any_cast<int>(opCode) == 3) {
			MenuDisplay(om3);
			Op3GeneratePasswordFile(om3, generateStepSize, generateGroupSize);
			MenuDisplay(bm);
		}
		else if (std::any_cast<int>(opCode) == 4) {
			Op4DecryptPasswordFile(om4, bm, generateStepSize, generateGroupSize);
		}
		else if (std::any_cast<int>(opCode) == 5) {
			// A friend to all is a friend to none
			// 27322810313331033910211452912207344136146925461033281533271031012815108114101
			std::string tp;
			std::cout << "Please Input Encrypted Password:" << std::endl;
			std::cin >> tp;
			std::cout << "Specfic password is : " << tp << std::endl;
			DecryptPassword(tp, "" , 0, 3, false);
			std::cout << "Decrypt Success."<< std::endl;
			MenuDisplay(bm);
		}
		else if (std::any_cast<int>(opCode) == 6) {
			break;
		}
		else {
			std::cout << "Code Error, Please Input Again.\n";
			std::cout << "Input Option Code:";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}
