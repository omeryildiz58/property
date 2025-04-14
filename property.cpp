#include <iostream>
#include "property.h"

struct dene{
	dene& operator+=(const dene& d) {
		return *this;
	}

	dene& operator+=(dene& d) {
		return *this;
	}

	double	operator[](const int a) {
		return 5.0;
	}

	int operator+(const dene&) {
		return 5;
	}

	//bool operator==(const dene&) {
	//	return true;
	//}

	bool operator==(const int&) {
		return true;
	}

	//bool operator==(bool a) {
	//	return a;
	//}

	//bool operator==(int a) {
	//	return a==3;
	//}

	bool operator==(const dene&) const{
		return true;
	}

	dene() {}
	dene(const dene&) {}
};

class MyClass {
	inline dene& getaaa() {
		return aaa;
	}

	inline void setaaa(const dene& value) {
		aaa = value;
	}
public:
	MyClass() : value_(0) {}

	inline int& getValue() {
		return value_;
	}

	inline void setValue(const int& value) {
		value_ = value;
	}

	Property<&getValue, &setValue > valueProperty = *this;
	Property<&getaaa, &MyClass::setaaa> valuePropertyaaa = *this;
private:
	int value_;
	dene aaa;
};

class MyClass1 {
public:
	MyClass1() {}

	inline const int& getValue() const {
		return 9;
	}

	Property<&MyClass1::getValue> valueProperty = *this;



};

 
int main() {
	MyClass1 obj;
	//obj.valueProperty2 = 5;
	//obj.valueProperty1 += 5;
	std::cout << "Value: " << (obj.valueProperty + 5) << std::endl;
	dene ddd = {};
	MyClass	obj1;
	MyClass	obj2;
	obj1.valueProperty += 5;

	obj1.valuePropertyaaa.operator dene &() += ddd;
	obj1.valuePropertyaaa += ddd;

	auto aaa = obj1.valuePropertyaaa + ddd;
	auto aaa1 = obj1.valuePropertyaaa + obj2.valuePropertyaaa;

	//obj1.valuePropertyaaa == 3;// obj2.valuePropertyaaa;
	//int aa = 5;
	//auto afdaa = obj1.valuePropertyaaa[5];

	return 0;
}
