#pragma once

class BaseMenu {

public:
	BaseMenu();
	~BaseMenu();
	void display(std::string value);
};

class OptionMenu_1 :public BaseMenu {

public:
	OptionMenu_1();
	~OptionMenu_1();
	void display(std::string value);
};

class OptionMenu_2 :public BaseMenu {

public:
	OptionMenu_2();
	~OptionMenu_2();
	void display(std::string value);
};
class OptionMenu_3 :public BaseMenu {

public:
	OptionMenu_3();
	~OptionMenu_3();
	void display(std::string value);
};

class OptionMenu_4 :public BaseMenu {

public:
	OptionMenu_4();
	~OptionMenu_4();
	void display(std::string value);
};

template<typename T>
void MenuDisplay(T& p, std::string value = "") {
	p.display(value);
}
