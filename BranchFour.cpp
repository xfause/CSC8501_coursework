#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "Menu.h"
#include "Encrypt.h"

void Op4DecryptPasswordFile(OptionMenu_4 om4, BaseMenu bm, int stepSize, int groupSize) {
	std::vector<std::string> pwdVec;
	std::vector<std::string> plainPwdVec;

	try {
		std::string str;
		std::ifstream passwordFile;
		passwordFile.open("./Result/passwordtest.txt");
		if (passwordFile.fail()) {
			throw std::invalid_argument("Unable To Open File: passwordtest.txt");
		}
		while (getline(passwordFile, str))
		{
			pwdVec.push_back(str);
		}
		if (pwdVec.size() == 0) {
			throw std::invalid_argument("Password List Is Empty");
		}
		if (passwordFile.eof()) {
			passwordFile.close();
		}
		else {
			throw std::invalid_argument("Unable To Read File: passwordtest.txt");
		}

		std::ifstream plainPwdFile;
		plainPwdFile.open("./Result/passwordtest_plain.txt");
		if (plainPwdFile.fail()) {
			throw std::invalid_argument("Unable To Open File: passwordtest_plain.txt");
		}
		while (getline(plainPwdFile, str))
		{
			plainPwdVec.push_back(str);
		}
		if (plainPwdVec.size() == 0) {
			throw std::invalid_argument("Plain Password List Is Empty");
		}
		if (plainPwdFile.eof()) {
			plainPwdFile.close();
		}
		else {
			throw std::invalid_argument("Unable To Read File: passwordtest_plain.txt");
		}

		MenuDisplay(om4);

		std::ofstream analysFile;
		analysFile.open("./Result/analysis.txt");
		clock_t processStart, procesEnd;
		for (int c = 0; c < 2; c++) {
			int cateCount = 0;
			while (cateCount < groupSize) {
				int stepCount = 0;
				int processTempSuccNum = 0;
				double processTempTime = 0;
				int length = (cateCount / stepSize) + 1;
				while (stepCount < stepSize) {
					/*if (c == 1 && cateCount > 2600) {
						std::cout << pwdVec[cateCount + c * groupSize] << " " <<
							plainPwdVec[cateCount + c * groupSize] << " " <<
							length << std::endl;*/
						if (processTempTime < 5 * 60) {
							processStart = clock();
							std::string plainText = DecryptPassword(
								pwdVec[cateCount + c * groupSize],
								plainPwdVec[cateCount + c * groupSize],
								length,
								c == 0 ? 1 : 2,
								true
							);

							if (plainText == "true") {
								processTempSuccNum++;
							}
							procesEnd = clock();
							processTempTime += (double)(procesEnd - processStart) / CLOCKS_PER_SEC;
						}
					//}
					
					stepCount++;
					cateCount++;
				}
				float percentage = float(processTempSuccNum) / float(stepSize);
				std::cout << "Categoty " << c + 1 << " Analys Success Rate Of Length " << length << " : " << percentage << std::endl;
				std::cout << "Categoty " << c + 1 << " Analys Averange Time Of Length " << length << " : " << processTempTime / processTempSuccNum << std::endl;
				analysFile << "Categoty " << c + 1 << " Analys Success Rate Of Length " << length << " : " << percentage << "\n";
				analysFile << "Categoty " << c + 1 << " Analys Averange Time Of Length " << length << " : " << processTempTime / processTempSuccNum << "\n";
			}
		}
		analysFile.close();

		MenuDisplay(om4, "success");
		MenuDisplay(bm);
		return;
	}
	catch (std::exception const& e) {
		std::cout << "There Is An Error: " << e.what() << std::endl;
		MenuDisplay(bm);
	}
}