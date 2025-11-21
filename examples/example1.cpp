#include <iostream>
#include <musProperty.h>
using namespace std;

class MyClass {
    int value_;
public:
    int& getValue() { return value_; }
    void setValue(const int& value) { value_ = value; }

    Property<&MyClass::getValue, &MyClass::setValue> valueProperty=this;
};

int main() {
    MyClass obj;
    obj.valueProperty = 42;
    std::cout << obj.valueProperty << std::endl;
    std::cout << "Input a integer:";
				std::cin >> obj.valueProperty;
    std::cout << obj.valueProperty << std::endl;
    return 0;
}
