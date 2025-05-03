
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
```

## Notes

- Lambdas are not supported in this version for getter and setter functions.
- `Deducing this` is not supported for getter and setter functions.
- only this initialization with `this` is supported.


```cpp
// Example of unsupported feature
class MyClass {
	int myProperty_;
public:
	MyClass(){}
	int& getMyProperty(this self) {//	Error: Deducing this not supported
		return self.myProperty_;
	}
	Property<&getMyProperty> myProperty=this;

};

```


```cpp
// Example of unsupported feature
	MyClass obj;
	Property<&getMyProperty, &setMyProperty> myProperty=&obj;// Error
	Property<&getMyProperty, &setMyProperty> myProperty=this;//	OK
```


## License

This library is licensed under the Apache License, Version 2.0.  
See the LICENSE file for details.


