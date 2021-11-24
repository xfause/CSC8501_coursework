#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include"Encrypt.h"
#include"Menu.h"

std::string Op3RandomPlainTextCateOne(int length) {
	std::string allLowCaseCharSet = "abcdefghijklmnopqrstuvwxyz";
	std::string randomLowCaseCharSet = "";
	std::string outputPlainPwd = "";
	for (int i = 0; i < 10; i++) {
		int r = rand() % (25 - 0 + 1) + 0;
		randomLowCaseCharSet += allLowCaseCharSet[r];
	}

	for (int i = 0; i < length; i++) {
		int r = rand() % ((randomLowCaseCharSet.length()-1) - 0 + 1) + 0;
		outputPlainPwd += randomLowCaseCharSet[r];
	}
	return outputPlainPwd;
}

std::vector<int> Op3RandomPlainTextCateTwo(int length) {
	//std::string allCharSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	std::string outputPlainPwd = "";
	std::vector<int> outputVec;

	std::vector<int> allCharASCII;
	for (int i = 0; i < 256; i++) {
		allCharASCII.push_back(i);
	}

	for (int i = 0; i < length; i++) {
		int r = rand() % ((allCharASCII.size() - 1) - 0 + 1) + 0;
		outputVec.push_back(allCharASCII[r]);
		allCharASCII.erase(std::remove(allCharASCII.begin(), allCharASCII.end(), allCharASCII[r]), allCharASCII.end());
	}

	/*for (int i = 0; i < length; i++) {
		int r = rand() % ((allCharSet.length() - 1) - 0 + 1) + 0;
		outputPlainPwd += allCharSet[r];
		allCharSet.erase(r, 1);
	}*/
	return outputVec;
}

void Op3GeneratePasswordFile(OptionMenu_3 om3, int stepSize, int groupSize) {
	std::string (*generatePlainForCateOne)(int a) = Op3RandomPlainTextCateOne;
	std::vector<int> (*generatePlainForCateTwo)(int a) = Op3RandomPlainTextCateTwo;

	std::srand(std::time(NULL));

	std::ofstream pwdTestFile;
	pwdTestFile.open("./Result/passwordtest.txt");
	std::ofstream pwdPlainFile;
	pwdPlainFile.open("./Result/passwordtest_plain.txt");
	for (int c = 0; c < 2; c++) {
		int groupCount = 0;
		while (groupCount < groupSize) {
			std::string tmpPwd;
			auto calcPasswordLength = [=]() {return (groupCount / stepSize) + 1; };
			int pwdLength = calcPasswordLength();
			if (c == 0) {
				tmpPwd = generatePlainForCateOne(pwdLength);
				//std::cout << tmpPwd << std::endl;
				pwdTestFile << EncryptPassword(tmpPwd) << "\n";
				std::string t = "";
				for (int i = 0; i < tmpPwd.length(); i++) {
					if (i == tmpPwd.length() - 1) {
						t += std::to_string((int)tmpPwd[i]);
					}
					else {
						t += std::to_string((int)tmpPwd[i]);
						t += "|";
					}
				}
				pwdPlainFile << t << "\n";
			}
			else {
				tmpPwd = "";
				std::vector<int> tmpVec = generatePlainForCateTwo(pwdLength);
				int offset = 0;
				int tempNum = 0;
				for (int i = 0; i < tmpVec.size(); i++) {
					int steps = 0;
					tempNum = tmpVec[i] + offset;
					steps = offset = calcSteps(tempNum);
					tmpPwd += std::to_string(steps);
				}
				pwdTestFile << tmpPwd << "\n";
				std::string t = "";
				for (int i = 0; i < tmpVec.size(); i++) {
					if (i == tmpVec.size() - 1) {
						t += std::to_string(tmpVec[i]);
					}
					else {
						t += std::to_string(tmpVec[i]);
						t += "|";
					}
				}
				pwdPlainFile << t << "\n";
			}
			groupCount++;
		}

	}
	pwdTestFile.close();
	MenuDisplay(om3, "success");
}