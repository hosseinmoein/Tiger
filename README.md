[![Build status](https://ci.appveyor.com/api/projects/status/acuqcseoi6y19wp4?svg=true)](https://ci.appveyor.com/project/justinjk007/matrix)
[![Build Status](https://travis-ci.org/justinjk007/Matrix.svg?branch=master)](https://travis-ci.org/justinjk007/Matrix)

# Matrix
This is a high performance matrix math and arithmetic library.
Please see src/materix_tester.cc for how to use this library.
Also see include/Matrix.h for documentation. I know, I will add a documentation file

# Documentation (I know, I will add an explicit documentation file)
[Matrix Documentation](include/Matrix.h)
[Complex Documentation](include/Complex.h)

# Test/Example
[Test File](src/matrix_tester.cc)

# Installing using CMake
This is a header only library but there are many header files, best to
install them in place. The CMake installation works on all platforms,
But we only support Windows 32-bit for the time being.

```
mkdir build
cd build
cmake ..
make install
```

# Uninstalling

```
cd build
make uninstall
```
