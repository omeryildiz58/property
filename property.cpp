#include <iostream>
#include "property.h"

struct dene{
	//dene& operator+=(const dene& d) {
	//	return *this;
	//}

	//const dene& operator+(const dene&) {
	//	return *this;
	//}
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

	inline const int& getValue() const {
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
	const dene ddd = {};
	MyClass	obj1;
	obj1.valuePropertyaaa += 5;
	return 0;
}
