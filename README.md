
# GB C++ Property Template Library

This library is a template library for properties in C++.

This library provides `Property` classes for C++.

## Attributes

- Getter and Setter methods for easy access.
- Operator support (`+=`, `-=`, etc.).
- Modern C++ (C++20) support.

## Installation

- C++20 compatible compiler (e.g., GCC, Clang, MSVC).
- Include the `property.h` and `checkoperator.h` files in your project.
- Check compiler flags (e.g., `/std:c++20` for MSVC).

## Example Usage
```cpp
#include "property.h"

class MyClass {
int myProperty;
public:
	MyClass(){}
	void setMyProperty(int value) {
		myProperty = value;
	}
	int getMyProperty() const {
		return myProperty;
	}
	Property<&getMyProperty, &setMyProperty> myProperty=this;
};

int main() {
	MyClass obj;
	obj.myProperty = 10; // Calls setMyProperty(10)
	int value = obj.myProperty; // Calls getMyProperty()
	return 0;
}
```

## License


