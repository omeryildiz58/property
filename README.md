
# C++ Property Template Library

This library is a template library for properties in C++.

This library provides `Property` classes for C++.

## Attributes

- Getter and Setter methods for easy access.
- Operator support (`+=`, `-=`, etc.).
- Modern C++ (C++23) support.

## Installation

- C++20 compatible compiler (e.g., GCC, Clang, MSVC).
- Include the `property.h` and `checkoperator.h` files in your project.
- Check compiler flags (e.g., `/std:c++23` for MSVC).

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
- Only this initialization with `this` is supported.
- If you want to use `Property` with read only, only getter must be defined.


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


## Build & Test (CMake + CTest)

```bash
cmake --preset linux-release
cmake --build out/build/linux-release
ctest --test-dir out/build/linux-release --output-on-failure
```

## CI/CD & Automated Releases

- **Submodule CMake Version Rule:** All repositories in the same submodule/release chain (`YILDIZ-ERP` -> `musFramework` -> `musProperty`) must keep the same `cmake_minimum_required` version (`VERSION 3.8`) so parent and child CMake policy levels match cleanly.
- **CI (`.github/workflows/ci.yml`):** Verifies C++23 compilation and `test_property` across Linux (GCC & Clang), Windows (MSVC x64), and macOS (Universal `arm64;x86_64`).
- **Release Please (`.github/workflows/release-please.yml`):** Merges to `master` using Conventional Commits automatically maintain a Release PR, create version tags/GitHub Releases, and attach Windows/Linux/macOS SDK packages with SHA-256 checksums.

## License

This library is licensed under the Apache License, Version 2.0.  
See the LICENSE file for details.



