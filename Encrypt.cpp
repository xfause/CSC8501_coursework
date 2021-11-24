#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<vector>
#include<map>
#include"Menu.h"

std::string ListToString(const std::list<int>& lstValue)
{
	if (lstValue.empty()) return "";
	std::string ss;
	int i = 0;
	for (auto it = lstValue.cbegin(); it != lstValue.cend(); it++)
	{
		ss = ss + std::to_string(*it);
	}
	return ss;
}

int calcSteps(int a) {
	if (a == 0) {
		return 0;
	}
	int steps = 0;
	while (a != 1) {
		a = (a % 2) ? (3 * a) + 1 : (a / 2);
		steps++;
	}
	return steps;
}

double searchTimeLimit = 3;

void dfs4Compare(
	std::vector<int>& plainPwdAscVec,
	int length,
	int v,
	std::vector<int> path,
	int target,
	int nodeCount,
	int** edges,
	int encryptPwdLength,
	int& plainLength,
	std::map<int, int>& nodeCountMapReverse,
	std::map<int, std::map<int, std::vector<int>>>& nodeMap,
	bool& compareFlag,
	clock_t& startSearchTime
)
{
	clock_t curTime = clock();
	if ((double)(curTime - startSearchTime) / CLOCKS_PER_SEC > searchTimeLimit) {
		return;
	}
	if (compareFlag == true) {
		return;
	}
	if (length < 0) {
		return;
	}
	if (path.size() > plainLength) {
		return;
	}
	if (length == 0 && v == target && path.size() == plainLength) {
		//std::map<int, int> childSln;
		std::vector<std::pair<int, int>> childSln;
		for (int i = 0; i < path.size(); i++) {
			childSln.push_back(std::make_pair(i + 1, path[i]));
		}

		int correctDigits = 0;
		for (int i = 0; i < childSln.size(); i++) {
			int lengthBetweenChar = 0;
			int nodeCode = nodeCountMapReverse[childSln[i].second];
			if (i == childSln.size() - 1) {
				lengthBetweenChar = encryptPwdLength - nodeCode;
			}
			else {
				int before = nodeCountMapReverse[childSln[i].second];
				int after = nodeCountMapReverse[childSln[i + 1].second];
				lengthBetweenChar = after - before;
			}
			std::vector<int> asciiOfChar = nodeMap[nodeCode][lengthBetweenChar];

			bool isFind = false;
			for (int j = 0; j < asciiOfChar.size(); j++) {
				if (asciiOfChar[j] == plainPwdAscVec[i]) {
					isFind = true;
				}
			}
			if (isFind == true) {
				correctDigits++;
			}
		}
		if (correctDigits == plainLength) {
			compareFlag = true;
		}
		return;
	}
	if (length == 0) {
		return;
	}
	for (int j = v; j < nodeCount; j++) {
		if (edges[v][j] != 0) {
			path.push_back(v);
			dfs4Compare(
				plainPwdAscVec,
				length - edges[v][j],
				j,
				path,
				target,
				nodeCount,
				edges,
				encryptPwdLength,
				plainLength,
				nodeCountMapReverse,
				nodeMap,
				compareFlag,
				startSearchTime
			);
			path.pop_back();
		}
	}
}

void dfs(
	int length,
	int v,
	std::vector<int> path,
	int target,
	int nodeCount,
	int** edges,
	int encryptPwdLength,
	int& solutionNumbers,
	std::map<int, int>& solutionGroup,
	std::map<int, int>& nodeCountMapReverse,
	std::map<int, std::map<int, std::vector<int>>>& nodeMap
) {
	if (length == 0 && v == target) {
		solutionNumbers++;
		std::map<int, int> childSln;
		for (int i = 0; i < path.size(); i++) {
			childSln[i + 1] = path[i];
		}
		solutionGroup[childSln.size()] = solutionGroup[childSln.size()] + 1;

		std::ofstream outputDecryptSolutionsFile;
		outputDecryptSolutionsFile.open("./Result/solutions.txt", std::ios::app);
		//solution->push_back(childSln);
		outputDecryptSolutionsFile << "solution " << solutionNumbers << " (length=" << childSln.size() << ")" << " :===================== " << "\n";
		for (auto iter = childSln.begin(); iter != childSln.end(); iter++) {
			int lengthBetweenChar = 0;
			int nodeCode = nodeCountMapReverse[iter->second];
			if (std::next(iter) == childSln.end()) {
				lengthBetweenChar = encryptPwdLength - nodeCode;
			}
			else {
				int before = nodeCountMapReverse[iter->second];
				int after = nodeCountMapReverse[(++iter)->second];
				lengthBetweenChar = after - before;
				iter--;
			}

			std::vector<int> asciiOfChar = nodeMap[nodeCode][lengthBetweenChar];
			for (int i = 0; i < asciiOfChar.size(); i++) {
				outputDecryptSolutionsFile << asciiOfChar[i] << " ";
			}
			outputDecryptSolutionsFile << "\n";
		}
		outputDecryptSolutionsFile.close();
		return;
	}
	if (length == 0) {
		return;
	}
	for (int j = v; j < nodeCount; j++) {
		if (edges[v][j] != 0) {
			path.push_back(v);
			dfs(length - edges[v][j], j, path, target, nodeCount, edges,
				encryptPwdLength, solutionNumbers, solutionGroup, nodeCountMapReverse, nodeMap
			);
			path.pop_back();
		}
	}
}

std::vector<int> splitASCIIStr(std::string str, std::string token) {
	std::vector<int>result;
	while (str.size()) {
		int index = str.find(token);
		if (index != std::string::npos) {
			result.push_back(stoi(str.substr(0, index)));
			str = str.substr(index + token.size());
			if (str.size() == 0)result.push_back(stoi(str));
		}
		else {
			result.push_back(stoi(str));
			str = "";
		}
	}
	return result;
}

std::string DecryptPassword(std::string encrypted, std::string decryptedASCII, int plainPwdLength, int tag, bool campare = true) {
	// tag means ascii range
	// tag 1 97-122
	// tag 2 0-256
	// tag 3 32-127
	// 
	//i: 1 to str.len - 3
	//	j : 0 to 143 (g(127 + 256)max)(127是g(0 - 256)的最大值)
	//		k : 0 to 256
	//		if str[n + 1] == g(j + k) :
	//			f[n + 1][g(j + k)] or = f[n][j]
	//		if str[n + 1:n + 2] == g(j + k) :
	//			f[n + 2][g(j + k)] or = f[n][j]
	//		if str[n + 1:n + 3] == g(j + k) :
	//			f[n + 3][g(j + k)] or = f[n][j]
	//

	int left, right = 0;
	if (tag == 1) {
		left = 97;
		right = 122;
	}
	else if (tag == 2) {
		left = 0;
		right = 255;
	}
	else if (tag == 3) {
		left = 32;
		right = 127;
	}

	int encryptPwdLength = encrypted.length();

	int** f = new int* [encryptPwdLength + 1];
	for (int i = 0; i < encryptPwdLength + 1; i++)
	{
		f[i] = new int[4]{ 0 };
	}
	f[0][0] = 1;

	std::vector<std::pair<int, std::pair<int, std::pair<int, int>>>> testVec;

	for (int i = 0; i <= encryptPwdLength; i++) {
		for (int j = 0; j < 3; j++) {
			int prevJValue = -1;
			std::string preJStr = "";
			if (i == 0) {
				prevJValue = 0;
			}
			else {
				if (j < i) {
					preJStr = encrypted.substr(i - j - 1, j + 1);
					if (preJStr[0] != '0') {
						prevJValue = std::stoi(preJStr);
					}
				}
			}
			if (prevJValue >= 0) {
				for (int k = left; k <= right; k++) {
					int newSteps = calcSteps(prevJValue + k);
					std::string newStepStr = std::to_string(newSteps);
					int newStepLength = newStepStr.length();
					if (i < encryptPwdLength) {
						if (encrypted.substr(i, 1) == newStepStr && encrypted.substr(i + 1, 1) != "0") {
							f[i + 1][newStepLength - 1] |= f[i][j];
							if (f[i + 1][newStepLength - 1] == 1) {
								testVec.push_back(std::make_pair(i, std::make_pair(1, std::make_pair(prevJValue, k))));
							}
						}
					}
					if (i < encryptPwdLength - 1) {
						if (encrypted.substr(i, 2) == newStepStr && encrypted.substr(i + 2, 1) != "0") {
							f[i + 2][newStepLength - 1] |= f[i][j];
							if (f[i + 2][newStepLength - 1] == 1) {
								testVec.push_back(std::make_pair(i, std::make_pair(2, std::make_pair(prevJValue, k))));
							}
						}
					}
					if (i < encryptPwdLength - 2) {
						if (encrypted.substr(i, 3) == newStepStr && encrypted.substr(i + 3, 1) != "0") {
							f[i + 3][newStepLength - 1] |= f[i][j];
							if (f[i + 3][newStepLength - 1] == 1) {
								testVec.push_back(std::make_pair(i, std::make_pair(3, std::make_pair(prevJValue, k))));
							}
						}
					}
				}
			}
		}
	}
	//std::cout << "=====================  " << std::endl;
	std::map<int, std::map<int, std::vector<int>>> nodeMap;

	for (int i = 0; i < testVec.size(); i++) {
		int t_i = testVec[i].first;
		int t_l = testVec[i].second.first;
		int t_v = testVec[i].second.second.second;
		if (f[testVec[i].first][std::to_string(testVec[i].second.second.first).length() - 1] == 1) {

			if (nodeMap.count(t_i) == 0) {
				std::vector<int> tt_v = { t_v };
				std::map<int, std::vector<int>> t_m;
				t_m[t_l] = tt_v;
				nodeMap[t_i] = t_m;
			}
			else {
				if (nodeMap[t_i].count(t_l) == 0) {
					std::vector<int> tt_v = { t_v };
					nodeMap[t_i][t_l] = tt_v;
				}
				else {
					if (std::find(nodeMap[t_i][t_l].begin(), nodeMap[t_i][t_l].end(), t_v) != nodeMap[t_i][t_l].end()) {
						// do nothing
					}
					else {
						nodeMap[t_i][t_l].push_back(t_v);
					}
				}
			}
		}
	}

	std::map<int, int> nodeCountMap;
	int nodeCount = 0;
	for (auto& t : nodeMap) {
		nodeCountMap[t.first] = nodeCount;
		nodeCount++;
	}
	nodeCountMap[encryptPwdLength] = nodeCount++;
	/*for (auto& t : nodeCountMap) {
		std::cout << t.first << " " << t.second << std::endl;
	}
	std::cout << "=====================  " << std::endl;*/

	std::map<int, int> nodeCountMapReverse;
	nodeCount = 0;
	for (auto& t : nodeMap) {
		nodeCountMapReverse[nodeCount] = t.first;
		nodeCount++;
	}
	nodeCountMapReverse[nodeCount++] = encryptPwdLength;
	/*for (auto& t : nodeCountMapReverse) {
		std::cout << t.first << " " << t.second << std::endl;
	}
	std::cout << "=====================  " << std::endl;*/


	int nodeSize = nodeCount;
	int** nodeMaxtrix = new int* [nodeSize];
	for (int i = 0; i < nodeSize; i++)
	{
		nodeMaxtrix[i] = new int[nodeSize] {0};
	}

	for (auto& t : nodeMap) {
		for (auto& t2 : t.second) {
			nodeMaxtrix[nodeCountMap[t.first]][nodeCountMap[t.first + t2.first]] = t2.first;
		}
	}

	// dp[i][j][k]
	// from start to node j is have a path which length=j and weight=k
	// dp[nodeSize][plainPwdLength][encryptPwdLength] = 1 means have solution
	//
		//int*** dp = new int** [nodeSize];
		//for (int i = 0; i < nodeSize; i++) {
		//	dp[i] = new int* [plainPwdLength + 1];
		//}
		//for (int i = 0; i < nodeSize; i++) {
		//	for (int j = 0; j <= plainPwdLength; j++) {
		//		dp[i][j] = new int[encryptPwdLength + 3]{ 0 };
		//	}
		//}

		//dp[0][0][0] = 1;
		//for (auto iter = nodeMap[0].begin(); iter != nodeMap[0].end(); ++iter) {
		//	dp[nodeCountMap[(iter->first)]][1][(iter->first)] = 1;
		//}

		//for (int i = 0; i < nodeSize; i++) {
		//	for (int j = 0; j < plainPwdLength; j++) {
		//		for (int k = 0; k <= encryptPwdLength; k++) {
		//			if (nodeMap[nodeCountMapReverse[i]].size()) {
		//				for (auto iter = nodeMap[nodeCountMapReverse[i]].begin(); iter != nodeMap[nodeCountMapReverse[i]].end(); ++iter) {
		//					/*std::cout << i << " " << j << " " << k << std::endl;
		//					std::cout << nodeCountMapReverse[i] + (iter->first) << " " << j + 1 << " " << k + (iter->first) << std::endl;*/
		//					dp[nodeCountMap[nodeCountMapReverse[i] + (iter->first)]][j + 1][k + (iter->first)] |= dp[i][j][k];
		//					if (dp[nodeCount - 1][plainPwdLength][encryptPwdLength] == 1) {
		//						break;
		//					}
		//				}
		//			}
		//			if (dp[nodeCount - 1][plainPwdLength][encryptPwdLength] == 1) {
		//				break;
		//			}
		//		}
		//		if (dp[nodeCount - 1][plainPwdLength][encryptPwdLength] == 1) {
		//			break;
		//		}
		//	}
		//	if (dp[nodeCount - 1][plainPwdLength][encryptPwdLength] == 1) {
		//		break;
		//	}
		//}
		//if (dp[nodeCount - 1][plainPwdLength][encryptPwdLength] == 1) {
		//	for (int i = 0; i < nodeSize; i++) {
		//		for (int j = 0; j <= plainPwdLength; j++) {
		//			delete[] dp[i][j];
		//		}
		//	}
		//	for (int i = 0; i < nodeSize; i++) {
		//		delete[] dp[i];
		//	}
		//	delete[] dp;
		//	return "true";
		//}
		//else {
		//	for (int i = 0; i < nodeSize; i++) {
		//		for (int j = 0; j <= plainPwdLength; j++) {
		//			delete[] dp[i][j];
		//		}
		//	}
		//	for (int i = 0; i < nodeSize; i++) {
		//		delete[] dp[i];
		//	}
		//	delete[] dp;
		//	return "false";
		//}

	if (campare == true) {

		std::vector<int> asciiOfPlain = splitASCIIStr(decryptedASCII, "|");
		std::vector<int> pathVec;
		int solutionNumbers = 0;
		bool haveSolution = false;
		clock_t startSearchTime = clock();
		dfs4Compare(
			asciiOfPlain,
			encryptPwdLength,
			0,
			pathVec,
			nodeCount - 1,
			nodeCount,
			nodeMaxtrix,
			encryptPwdLength,
			plainPwdLength,
			nodeCountMapReverse,
			nodeMap,
			haveSolution,
			startSearchTime
		);

		for (int i = 0; i < nodeSize; i++) {
			delete[] nodeMaxtrix[i];
		}
		delete[] nodeMaxtrix;
		delete[] f;
		if (haveSolution) {
			return "true";
		}
		else {
			return "false";
		}
	}
	else {
		// get the path of length = encrypt.length()
		// check the path go through nums of node = plainPwd length
		std::ofstream emptyFile;
		emptyFile.open("./Result/solutions.txt");

		std::vector<int> pathVec;
		std::map<int, int> solutionGroup;
		int solutionNumbers = 0;
		dfs(encryptPwdLength,
			0,
			pathVec,
			nodeCount - 1,
			nodeCount,
			nodeMaxtrix,
			encryptPwdLength,
			solutionNumbers,
			solutionGroup,
			nodeCountMapReverse,
			nodeMap
		);

		if (solutionNumbers > 0) {

			std::ofstream outputDecryptSolutionsFile;
			outputDecryptSolutionsFile.open("./Result/solutions.txt", std::ios::app);

			for (auto iter = solutionGroup.begin(); iter != solutionGroup.end(); iter++) {
				outputDecryptSolutionsFile << "plain text length: " << iter->first << " " << "solution numbers: " << iter->second << "\n";
			}
			outputDecryptSolutionsFile << "total solutions: " << solutionNumbers << "\n";
			outputDecryptSolutionsFile.close();

			for (int i = 0; i < nodeSize; i++) {
				delete[] nodeMaxtrix[i];
			}
			delete[] nodeMaxtrix;
			delete[] f;
			return "true";
		}
		else {
			for (int i = 0; i < nodeSize; i++) {
				delete[] nodeMaxtrix[i];
			}
			delete[] nodeMaxtrix;
			delete[] f;
			return "false";
		}
	}
}

std::string EncryptPassword(std::string origin) {
	std::list<int> final;
	int offset = 0;
	int temp = 0;
	for (int i = 0; i < origin.length(); i++) {
		int steps = calcSteps((int)origin[i] + offset);
		offset = steps;
		final.push_back(steps);
	}

	std::string s = ListToString(final);
	return s;
}

