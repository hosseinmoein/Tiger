// Hossein Moein
/*
Copyright (c) 2019-2022, Hossein Moein
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Hossein Moein and/or the Matrix nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <cstdlib>
#include <math.h>
#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

namespace hmma
{

// Some cute stuff, not very usefull though.
//
struct  NL  {

    struct  NullType  {   };

    template<int N, class T>
    struct  NumList  {

        enum { value = N };

        typedef T    tail;
    };

    template<class T, int N>
    struct  Append  {

        typedef NL::NumList<N, T>   Result;
    };

    // template<class T, class T_LIST>
    // struct  AppendList  {

    //     typedef typename NL::Append<T_LIST,
    //                                       T::value>::Result  Result;
    // };

    template<class T>
    struct  Length  {

        enum { value = NL::Length<typename T::tail>::value + 1 };
    };

    template<unsigned int L, int DN>
    struct  InitNumList  {

        typedef NL::NumList<
            DN, typename NL::InitNumList<L - 1, DN>::Result>    Result;
    };

    template<class T, unsigned int I>
    struct  NumAt  {

        enum { value = NL::NumAt<typename T::tail, I - 1>::value };
    };

    template<class T, unsigned int I>
    struct  Erase  {

        typedef typename NL::Erase<typename T::tail, I - 1>::Result   Result;
    };
};

// ----------------------------------------------------------------------------

template<>
struct  NL::Length<NL::NullType>  { enum { value = 0 }; };

template<int DN>
struct  NL::InitNumList<0, DN>  { typedef NL::NullType  Result; };

template<class T>
struct  NL::Erase<T, 0>  { typedef T    Result; };

template<class T>
struct  NL::NumAt<T, 0>  { enum { value = T::value }; };

// ----------------------------------------------------------------------------

//
// Arithmetic Operator Functors
//

template<class C1, class C2 = C1, class R = C1>
class   Plus : public std::binary_function<C1, C2, R>  {

public:

    typedef typename std::binary_function<C1, C2, R> BaseClass;
    typedef typename BaseClass::result_type          result_type;
    typedef typename BaseClass::first_argument_type  first_argument_type;
    typedef typename BaseClass::second_argument_type second_argument_type;

    inline Plus () noexcept  {   }
    inline result_type operator () (const first_argument_type &lhs,
                                    const second_argument_type &rhs)
        const noexcept  { return (lhs + rhs); }
};

// ----------------------------------------------------------------------------

template<class C1, class C2 = C1, class R = C1>
class   Minus : public std::binary_function<C1, C2, R>  {

public:

    typedef typename std::binary_function<C1, C2, R> BaseClass;
    typedef typename BaseClass::result_type          result_type;
    typedef typename BaseClass::first_argument_type  first_argument_type;
    typedef typename BaseClass::second_argument_type second_argument_type;

    inline Minus () noexcept  {   }
    inline result_type operator () (const first_argument_type &lhs,
                                    const second_argument_type &rhs)
        const noexcept  { return (lhs - rhs); }
};

// ----------------------------------------------------------------------------

template<class C1, class C2 = C1, class R = C1>
class   Multiplies : public std::binary_function<C1, C2, R>  {

public:

    typedef typename std::binary_function<C1, C2, R> BaseClass;
    typedef typename BaseClass::result_type          result_type;
    typedef typename BaseClass::first_argument_type  first_argument_type;
    typedef typename BaseClass::second_argument_type second_argument_type;

    inline Multiplies () noexcept  {   }
    inline result_type operator () (const first_argument_type &lhs,
                                    const second_argument_type &rhs)
        const noexcept  { return (lhs * rhs); }
};

// ----------------------------------------------------------------------------

template<class C1, class C2 = C1, class R = C1>
class   Divides : public std::binary_function<C1, C2, R>  {

public:

    typedef typename std::binary_function<C1, C2, R> BaseClass;
    typedef typename BaseClass::result_type          result_type;
    typedef typename BaseClass::first_argument_type  first_argument_type;
    typedef typename BaseClass::second_argument_type second_argument_type;

    inline Divides () noexcept  {   }
    inline result_type operator () (const first_argument_type &lhs,
                                    const second_argument_type &rhs)
        const noexcept  { return (lhs / rhs); }
};

// ----------------------------------------------------------------------------

template<class C1, class C2 = C1, class R = C1>
class   Modulus : public std::binary_function<C1, C2, R>  {

public:

    typedef typename std::binary_function<C1, C2, R> BaseClass;
    typedef typename BaseClass::result_type          result_type;
    typedef typename BaseClass::first_argument_type  first_argument_type;
    typedef typename BaseClass::second_argument_type second_argument_type;

    inline Modulus () noexcept  {   }
    inline result_type operator () (const first_argument_type &lhs,
                                    const second_argument_type &rhs)
        const noexcept  { return (lhs % rhs); }
};

// ----------------------------------------------------------------------------

template<class C, class R = C>
class   Negate : public std::unary_function<C, R>  {

public:

    typedef typename std::unary_function<C, R>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline Negate () noexcept  {   }
    inline result_type operator () (const argument_type &lhs)
        const noexcept  { return (-lhs); }
};

// ----------------------------------------------------------------------------

//
// Scaler Expressions
//

template<class C, class R = C>
class   Identity : public std::unary_function<C, R>  {

public:

    typedef typename std::unary_function<C, R>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline Identity () noexcept  {   }

    inline result_type operator () (const argument_type &lhs)
        const noexcept  { return (static_cast<result_type>(lhs)); }
};

// ----------------------------------------------------------------------------

template<class C, class R = C>
class   Literal : public std::unary_function<C, R>  {

private:

    const C value_;

public:

    typedef typename std::unary_function<C, R>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline Literal () noexcept : value_ ()  {   }
    inline explicit Literal (const C &v) noexcept : value_ (v)  {   }

    inline result_type operator () (const argument_type &)
        const noexcept  { return (static_cast<result_type>(value_)); }
};

// ----------------------------------------------------------------------------

template<class E, class A>
class   Expression : public std::unary_function<A, A>  {

private:

    const E exp_;

public:

    typedef typename std::unary_function<A, A>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline Expression () noexcept : exp_ ()  {   }
    inline explicit Expression (const E &e) noexcept : exp_ (e)  {   }

    inline result_type operator () (const argument_type &x)
        const noexcept  { return (exp_ (x)); }
};

// ----------------------------------------------------------------------------

template<class E1, class E2, class O, class A>
class   BinExprOpt : public std::unary_function<A, A>  {

private:

    const E1    expr1_;
    const E2    expr2_;
    const O     opt_;

public:

    typedef typename std::unary_function<A, A>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline BinExprOpt () noexcept : expr1_ (), expr2_ (), opt_ ()  {   }
    inline BinExprOpt (const E1 &e1, const E2 &e2) noexcept
        : expr1_ (e1), expr2_ (e2), opt_ ()  {   }

    inline result_type operator () (argument_type x) const noexcept  {

        return (opt_ (expr1_ (x), expr2_ (x)));
    }
};

// ----------------------------------------------------------------------------

template<class E, class O, class A>
class   UnaExprOpt : public std::unary_function<A, A>  {

private:

    const E expr_;
    const O opt_;

public:

    typedef typename std::unary_function<A, A>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline UnaExprOpt () noexcept : expr_ (), opt_ ()  {   }
    inline UnaExprOpt (const E &e) noexcept : expr_ (e), opt_ ()  {   }

    inline result_type operator () (argument_type x) const noexcept  {

        return (opt_ (expr_ (x)));
    }
};

// ----------------------------------------------------------------------------

template<class T, class R = T>
class   Power : public std::binary_function<T, T, R>  {

public:

    typedef typename std::binary_function<T, T, R>   BaseClass;
    typedef typename BaseClass::result_type          result_type;
    typedef typename BaseClass::first_argument_type  first_argument_type;
    typedef typename BaseClass::second_argument_type second_argument_type;

    inline Power () noexcept  {   }
    inline result_type operator () (const first_argument_type &lhs,
                                    const second_argument_type &rhs)
        const noexcept  { return (::pow (lhs, rhs)); }

   // ::pow (Literal, Expression)
   //
    template<class E>
    inline
    Expression<BinExprOpt<Literal<T>, Expression<E, T>, Power<T>, T>, T>
    operator () (const first_argument_type &lhs, const Expression<E, T> &rhs)
        const noexcept  {

        typedef BinExprOpt<Literal<T>,
                           Expression<E, T>,
                           Power<T>,
                           T>   expr_type;

        return (Expression<expr_type, T>(expr_type (Literal<T>(lhs), rhs)));
    }

   // ::pow (Expression, Literal)
   //
    template<class E>
    inline
    Expression<BinExprOpt<Expression<E, T>, Literal<T>, Power<T>, T>, T>
    operator () (const Expression<E, T> &lhs,
                 const second_argument_type &rhs) const noexcept  {

        typedef BinExprOpt<Expression<E, T>,
                           Literal<T>,
                           Power<T>,
                           T>   expr_type;

        return (Expression<expr_type, T>(expr_type (lhs, Literal<T>(rhs))));
    }

   // ::pow (Expression, Expression)
   //
    template<class E>
    inline
    Expression<BinExprOpt<Expression<E, T>, Expression<E, T>, Power<T>, T>, T>
    operator () (const Expression<E, T> &lhs, const Expression<E, T> &rhs)
        const noexcept  {

        typedef BinExprOpt<Expression<E, T>,
                           Expression<E, T>,
                           Power<T>,
                           T>   expr_type;

        return (Expression<expr_type, T>(expr_type (Literal<T>(lhs),
                                                    Literal<T>(rhs))));
    }
};

// ----------------------------------------------------------------------------

template<class T, class R = T>
class   NLog : public std::unary_function<T, R>  {

public:

    typedef typename std::unary_function<T, R>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline NLog () noexcept  {   }
    inline result_type operator () (const argument_type &lhs)
        const noexcept  { return (::log (lhs)); }

   // ::log (Expression)
   //
    template<class E>
    inline
    Expression<UnaExprOpt<Expression<E, T>, NLog<T>, T>, T>
    operator () (const Expression<E, T> &x) const noexcept  {

        typedef UnaExprOpt<Expression<E, T>, NLog<T>, T>   expr_type;

        return (Expression<expr_type, T>(expr_type (Literal<T>(x))));
    }
};

// ----------------------------------------------------------------------------

template<class T, class R = T>
class   EExpo : public std::unary_function<T, R>  {

public:

    typedef typename std::unary_function<T, R>  BaseClass;
    typedef typename BaseClass::result_type     result_type;
    typedef typename BaseClass::argument_type   argument_type;

    inline EExpo () noexcept  {   }
    inline result_type operator () (const argument_type &lhs)
        const noexcept  { return (::exp (lhs)); }

   // ::exp (Expression)
   //
    template<class E>
    inline
    Expression<UnaExprOpt<Expression<E, T>, NLog<T>, T>, T>
    operator () (const Expression<E, T> &x) const noexcept  {

        typedef UnaExprOpt<Expression<E, T>, NLog<T>, T>   expr_type;

        return (Expression<expr_type, T>(expr_type (Literal<T>(x))));
    }
};

// ----------------------------------------------------------------------------

//
// Literal Opt Expression
//

template<class T, class E>
inline
Expression<BinExprOpt<Literal<T>, Expression<E, T>, Plus<T>, T>, T>
operator + (const T &lhs, const Expression<E, T> &rhs) noexcept  {

    typedef BinExprOpt<Literal<T>, Expression<E, T>, Plus<T>, T>   expr_type;

    return (Expression<expr_type, T>(expr_type (Literal<T>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

template<class T, class E>
inline
Expression<BinExprOpt<Literal<T>, Expression<E, T>, Minus<T>, T>, T>
operator - (const T &lhs, const Expression<E, T> &rhs) noexcept  {

    typedef BinExprOpt<Literal<T>, Expression<E, T>, Minus<T>, T>   expr_type;

    return (Expression<expr_type, T>(expr_type (Literal<T>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

template<class T, class E>
inline
Expression<BinExprOpt<Literal<T>, Expression<E, T>, Multiplies<T>, T>, T>
operator * (const T &lhs, const Expression<E, T> &rhs) noexcept  {

    typedef BinExprOpt<Literal<T>,
                       Expression<E, T>,
                       Multiplies<T>,
                       T>   expr_type;

    return (Expression<expr_type, T>(expr_type (Literal<T>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

template<class T, class E>
inline
Expression<BinExprOpt<Literal<T>, Expression<E, T>, Divides<T>, T>, T>
operator / (const T &lhs, const Expression<E, T> &rhs) noexcept  {

    typedef BinExprOpt<Literal<T>,
                       Expression<E, T>,
                       Divides<T>,
                       T>   expr_type;

    return (Expression<expr_type, T>(expr_type (Literal<T>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

//
// Expression Opt Literal
//

template<class E, class T>
inline
Expression<BinExprOpt<Expression<E, T>, Literal<T>, Plus<T>, T>, T>
operator + (const Expression<E, T> &lhs, const T &rhs) noexcept  {

    typedef BinExprOpt<Expression<E, T>, Literal<T>, Plus<T>, T>   expr_type;

    return (Expression<expr_type, T>(expr_type (lhs, Literal<T>(rhs))));
}

// ----------------------------------------------------------------------------

template<class E, class T>
inline
Expression<BinExprOpt<Expression<E, T>, Literal<T>, Minus<T>, T>, T>
operator - (const Expression<E, T> &lhs, const T &rhs) noexcept  {

    typedef BinExprOpt<Expression<E, T>, Literal<T>, Minus<T>, T>   expr_type;

    return (Expression<expr_type, T>(expr_type (lhs, Literal<T>(rhs))));
}

// ----------------------------------------------------------------------------

template<class E, class T>
inline
Expression<BinExprOpt<Expression<E, T>, Literal<T>, Multiplies<T>, T>, T>
operator * (const Expression<E, T> &lhs, const T &rhs) noexcept  {

    typedef BinExprOpt<Expression<E, T>,
                       Literal<T>,
                       Multiplies<T>,
                       T>   expr_type;

    return (Expression<expr_type, T>(expr_type (lhs, Literal<T>(rhs))));
}

// ----------------------------------------------------------------------------

template<class E, class T>
inline
Expression<BinExprOpt<Expression<E, T>, Literal<T>, Divides<T>, T>, T>
operator / (const Expression<E, T> &lhs, const T &rhs) noexcept  {

    typedef BinExprOpt<Expression<E, T>,
                       Literal<T>,
                       Divides<T>,
                       T>   expr_type;

    return (Expression<expr_type, T>(expr_type (lhs, Literal<T>(rhs))));
}

// ----------------------------------------------------------------------------

//
// Expression Opt Expression
//

template<class E1, class E2, class T>
inline
Expression<BinExprOpt<Expression<E1, T>, Expression<E2, T>, Plus<T>, T>, T>
operator + (const Expression<E1, T> &lhs,
            const Expression<E2, T> &rhs) noexcept  {

    typedef BinExprOpt<Expression<E1, T>,
                       Expression<E2, T>,
                       Plus<T>,
                       T>   expr_type;

    return (Expression<expr_type, T> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class E1, class E2, class T>
inline
Expression<BinExprOpt<Expression<E1, T>, Expression<E2, T>, Minus<T>, T>, T>
operator - (const Expression<E1, T> &lhs,
            const Expression<E2, T> &rhs) noexcept  {

    typedef BinExprOpt<Expression<E1, T>,
                       Expression<E2, T>,
                       Minus<T>,
                       T>   expr_type;

    return (Expression<expr_type, T> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class E1, class E2, class T>
inline
Expression<BinExprOpt<Expression<E1, T>, Expression<E2, T>, Multiplies<T>, T>,
           T>
operator * (const Expression<E1, T> &lhs,
            const Expression<E2, T> &rhs) noexcept  {

    typedef BinExprOpt<Expression<E1, T>,
                       Expression<E2, T>,
                       Multiplies<T>,
                       T>   expr_type;

    return (Expression<expr_type, T>(expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class E1, class E2, class T>
inline
Expression<BinExprOpt<Expression<E1, T>, Expression<E2, T>, Divides<T>, T>, T>
operator / (const Expression<E1, T> &lhs,
            const Expression<E2, T> &rhs) noexcept  {

    typedef BinExprOpt<Expression<E1, T>,
                       Expression<E2, T>,
                       Divides<T>,
                       T>   expr_type;

    return (Expression<expr_type, T>(expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

//
// Vector-based constructs
//

// The only reason for this class is the need for the assignment operator
// for Expressions, unfortunately.
//
template<class T>
class   Vector : public std::vector<T>  {

public:

    typedef std::vector<T>                  BaseClass;
    typedef typename BaseClass::size_type   size_type;
    typedef typename BaseClass::value_type  value_type;

    Vector () noexcept : BaseClass ()  {   }
    Vector (size_type n) noexcept : BaseClass (n)  {   }
    Vector (size_type n, const value_type &v) noexcept
        : BaseClass (n, v)  {   }

    inline Vector &operator = (const Vector &rhs) noexcept  {

        BaseClass::operator = (rhs);
        return (*this);
    }

    template<class E>
    inline Vector &operator = (const E &rhs) noexcept  {

        rhs.assign (*this);
        return (*this);
    }
};

// ----------------------------------------------------------------------------

//
// Vector-based Expressions
//

template<class I, class T>
class   VecExpression  {

private:

    I   citer_;

public:

    typedef T   value_type;

    inline explicit VecExpression (const I &it) noexcept : citer_ (it)  {   }

    inline VecExpression &operator ++ () noexcept  {    // ++Prefix

        ++citer_;
        return (*this);
    }
    inline VecExpression operator ++ (int) noexcept  { // Postfix++

        VecExpression slug (citer_);

        ++citer_;
        return (slug);
    }

    inline value_type operator * () const noexcept  { return (*citer_); }
    inline value_type operator [] (unsigned int i) const noexcept  {

        return (*(citer_ + i));
    }

    inline void assign (Vector<T> &lhs) const noexcept  {

        I   slug = citer_;

        for (typename Vector<T>::iterator iter = lhs.begin ();
             iter != lhs.end (); ++iter, ++slug)
            *iter = *slug;
    }
};

// ----------------------------------------------------------------------------

template<class I1, class I2, class O, class T>
class   VecBinExprOpt  {

private:

    I1      citer1_;
    I2      citer2_;
    const O opt_;

public:

    typedef T   value_type;

    inline VecBinExprOpt (const I1 &it1, const I2 &it2) noexcept
        : citer1_ (it1), citer2_ (it2), opt_ ()  {   }

    inline VecBinExprOpt &operator ++ () noexcept  {    // ++Prefix

        ++citer1_;
        ++citer2_;
        return (*this);
    }
    inline VecBinExprOpt operator ++ (int) noexcept  { // Postfix++

        VecBinExprOpt slug (citer1_, citer2_);

        ++citer1_;
        ++citer2_;
        return (slug);
    }

    inline value_type operator * () const noexcept  {

        return (opt_ (*citer1_, *citer2_));
    }
    inline value_type operator [] (unsigned int i) const noexcept  {

        return (opt_ (*(citer1_ + i), *(citer2_ + i)));
    }
};

// ----------------------------------------------------------------------------

//
// Vector-based: Vector Opt Vector
//

template<class T>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            typename Vector<T>::const_iterator,
                            Plus<T>,
                            T>,
              T>
operator + (const Vector<T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          typename Vector<T>::const_iterator,
                          Plus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs.begin(), rhs.begin())));
}

// ----------------------------------------------------------------------------

template<class T>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            typename Vector<T>::const_iterator,
                            Minus<T>,
                            T>,
              T>
operator - (const Vector<T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                           typename Vector<T>::const_iterator,
                           Minus<T>,
                           T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type(lhs.begin(), rhs.begin())));
}

// ----------------------------------------------------------------------------

template<class T>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            typename Vector<T>::const_iterator,
                            Multiplies<T>,
                            T>,
              T>
operator * (const Vector<T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          typename Vector<T>::const_iterator,
                          Multiplies<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type(lhs.begin(), rhs.begin())));
}

// ----------------------------------------------------------------------------

template<class T>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            typename Vector<T>::const_iterator,
                            Divides<T>,
                            T>,
              T>
operator / (const Vector<T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          typename Vector<T>::const_iterator,
                          Divides<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type(lhs.begin(), rhs.begin())));
}

// ----------------------------------------------------------------------------

//
// Vector-based: Expression Opt Vector
//

template<class I, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I, T>,
                            typename Vector<T>::const_iterator,
                            Plus<T>,
                            T>,
              T>
operator + (const VecExpression<I, T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I, T>,
                          typename Vector<T>::const_iterator,
                          Plus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class I, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I, T>,
                            typename Vector<T>::const_iterator,
                            Minus<T>,
                            T>,
              T>
operator - (const VecExpression<I, T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I, T>,
                          typename Vector<T>::const_iterator,
                          Minus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class I, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I, T>,
                            typename Vector<T>::const_iterator,
                            Multiplies<T>,
                            T>,
              T>
operator * (const VecExpression<I, T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I, T>,
                          typename Vector<T>::const_iterator,
                          Multiplies<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class I, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I, T>,
                            typename Vector<T>::const_iterator,
                            Divides<T>,
                            T>,
              T>
operator / (const VecExpression<I, T> &lhs, const Vector<T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I, T>,
                          typename Vector<T>::const_iterator,
                          Divides<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

//
// Vector-based: Vector Opt Expression
//

template<class T, class I>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            VecExpression<I, T>,
                            Plus<T>,
                            T>,
              T>
operator + (const Vector<T> &lhs, const VecExpression<I, T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          VecExpression<I, T>,
                          Plus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

template<class T, class I>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            VecExpression<I, T>,
                            Minus<T>,
                            T>,
              T>
operator - (const Vector<T> &lhs, const VecExpression<I, T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          VecExpression<I, T>,
                          Minus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

template<class T, class I>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            VecExpression<I, T>,
                            Multiplies<T>,
                            T>,
              T>
operator * (const Vector<T> &lhs, const VecExpression<I, T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          VecExpression<I, T>,
                          Multiplies<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

template<class T, class I>
inline
VecExpression<VecBinExprOpt<typename Vector<T>::const_iterator,
                            VecExpression<I, T>,
                            Divides<T>,
                            T>,
              T>
operator / (const Vector<T> &lhs, const VecExpression<I, T> &rhs) noexcept  {

    typedef VecBinExprOpt<typename Vector<T>::const_iterator,
                          VecExpression<I, T>,
                          Divides<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

//
// Vector-based: Expression Opt Expression
//

template<class I1, class I2, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I1, T>,
                            VecExpression<I2, T>,
                            Plus<T>,
                            T>,
              T>
operator + (const VecExpression<I1, T> &lhs,
            const VecExpression<I2, T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I1, T>,
                          VecExpression<I2, T>,
                          Plus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class I1, class I2, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I1, T>,
                            VecExpression<I2, T>,
                            Minus<T>,
                            T>,
               T>
operator - (const VecExpression<I1, T> &lhs,
            const VecExpression<I2, T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I1, T>,
                          VecExpression<I2, T>,
                          Minus<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class I1, class I2, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I1, T>,
                            VecExpression<I2, T>,
                            Multiplies<T>,
                            T>,
              T>
operator * (const VecExpression<I1, T> &lhs,
            const VecExpression<I2, T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I1, T>,
                          VecExpression<I2, T>,
                          Multiplies<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class I1, class I2, class T>
inline
VecExpression<VecBinExprOpt<VecExpression<I1, T>,
                            VecExpression<I2, T>,
                            Divides<T>,
                            T>,
              T>
operator / (const VecExpression<I1, T> &lhs,
            const VecExpression<I2, T> &rhs) noexcept  {

    typedef VecBinExprOpt<VecExpression<I1, T>,
                          VecExpression<I2, T>,
                          Divides<T>,
                          T>    expr_type;

    return (VecExpression<expr_type, T>(expr_type (lhs, rhs)));
}

} // namespace hmma

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
