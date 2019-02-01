[![Build status](https://ci.appveyor.com/api/projects/status/acuqcseoi6y19wp4?svg=true)](https://ci.appveyor.com/project/hosseinmoein/matrix)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/std/the-standard )
[![Build Status](https://travis-ci.org/hosseinmoein/Matrix.svg?branch=master)](https://travis-ci.org/hosseinmoein/Matrix)

![Alt text](docs/Matrix.jpeg "Matrix")

# Matrix
This is a math and arithmetic library. It has stood many years of performing in mission critical production for financial systems. It has been optimized for correctness and performance.
Most of the math routines are straight translation from Fortran. Arithmetic operators are implemented using expression template techniques.<BR>
This is a header-only library

# Documentation
[Matrix Documentation](docs/MatrixDoc.pdf)<BR>
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

# Design's thought process
Have you ever needed a good matrix math library, in C++? If so, continue reading. It is not that big of a deal, if you put some thoughts to it. But most importantly it is fun.

If you want to do this the right way, so it could handle a large spectrum of problems, you should start specifying the bits and pieces before getting to your main matrix object.

One of the pieces that need its own construct is whether you need a dense or sparse matrix. So you should have some types/classes/structs that separate these two concepts.

Then you should ask yourself where do I want to store all the data in the matrix and how do I want to lay them out (e.g. in memory vector, mmaped vector, hash map, etc.). That, of course, depends on the previous construct that you layed down for dense/sparse choice. But let’s make this a template argument so you will have a freedom of choosing down the line.

Another separation you need is symmetric vs. non-symmetric matrices. You have to choose whether you want to bake this into the matrix class hierarchy or make it a flag, set at runtime, in the final matrix class. There are pros and cons to both approaches. I am not going into the pros and cons, but I chose to bake it in.

Now, you thought about most of the basic things you need for a library that can solve a wide range of problems. So let’s put them together succinctly:

1) `template<T> MatrixBase`. T is the type of data (e.g. double, imaginary, etc.) you want to store. MatrixBase defines basics such as types like size_type, value_type, etc. Also it defines one-liner exceptions such as NotSquare, Singular, etc. That's about it for this.
2) `template<T, S = std::vector> MatrixStorage` which is derived from `MatrixBase`. T is the data type as above, S is where you want to store the data, defaulted to the venerable STL vector. This class contains the data storage. It tells you how many rows and columns you have. It can swap data, etc.
3) `template<T, S> DenseMatrixBase` which is derived from `MatrixStorage` with the corresponding template arguments. This class, basically, implements how the data is laid out given the storage type. For example; is it a vector of vectors, is it a flat vector with column or row major layout, etc. In my case, it is a flat vector with column major layout. Since this is the class that has the knowledge about the data layout, it provides the data access methods including row and column iterators. There is, similarly, a class for sparse matrix. I am omitting that line of derivation here.
4) `template<T, S> SymmMatrixBase` which is derived from `DenseMatrixBase` with the corresponding template arguments. This class implements the shortcuts that we could apply to symmetric matrices. There is no non-symmetric class. A Matrix that doesn’t have SymmMatrixBase in its derivation line, by default, is non-symmetric. Note that a non-symmetric class could still be symmetric based on its run-time data content, but it would not benefit from the shortcuts.
5) And finally we have the head matrix that draws from all of the above classes.


OK, so we have a working matrix class now. So far, most of the things we have done are boilerplate with some thoughts given to class design.

But now comes the exciting part;

Let’s assume we have a bunch of these matrices in our system, server, analytical engine and what not. What are the most frequent and therefore time/CPU consuming operations we usually do with these matrices. I submit, it is arithmetic (not Eigen vector, transformations, etc.), at least in the systems I am familiar with.

If you have noticed, I did not talk, at all, about any arithmetic operators (e.g. +, *, ^, etc.). In the traditional C++ you would redefine arithmetic operators as either friend or independent methods. Let's assume you have a bunch of compatibly sized matrices and you have an expression like `A = B + C * D`. This simple expression creates 2 temporary matrices. If these matrices are large, and/or you do this frequently, and/or you have complicated expressions then you will have a nasty bottleneck in your system.

Enter the expression template programming;

This technique allows you to substitute temporary matrices with simple (one, two pointer size) iterators. You calculate the very final result of the arithmetic, when you assign the final value to A by going through the iterators in the right order.
I am not going through the details here. It requires its own article and more. 

Isn’t C++ wonderful? It allows you to do so much funky stuff. And it also let you blow your leg off. What else do you want?
