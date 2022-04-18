# Installation instructions

This library has no outside dependencies and is configured to produce a compiled shared library (.so or .dylib) file. Use of the shared library file depends on your framework. In Ngen, you would point to the compiled library in your realization config.

To build the library:

1. Clone the repository
2. From the root of your working copy, initialize git submotules with
```
git submodule update --init --recursive
```
3. Compile the library and tests with
```
cmake -S . -B cmake_build
cmake --build cmake_build --target all
```
(The compiled shared library will be in `cmake_build` as `libslothmodel.so` or `libslothmodel.dylib`)

4. (Optional) Run the tests at `cmake_built/test/sloth_tests`