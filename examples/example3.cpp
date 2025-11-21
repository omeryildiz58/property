#include <iostream>
#include <musProperty.h>
using namespace std;

class MyClass {
	int myProperty_;
public:
	MyClass(){}
	void setMyProperty(const int& value) {
		myProperty_ = value;
	}
	int& getMyProperty() {
		return myProperty_;
	}
	Property<&getMyProperty, &setMyProperty> myProperty=this;

};

int main() {
	MyClass obj;

	obj.myProperty = 10; // Calls setMyProperty(10)
	int value = obj.myProperty; // Calls getMyProperty()
	return 0;
}