// Hossein Moein
// July 17 2009

#ifndef _INCLUDED_DMScu_MathOperators_h
#define _INCLUDED_DMScu_MathOperators_h 0

#include <cstdlib>
#include <math.h>
#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

// Some cute stuff, not very usefull though.
//
struct  DMScu_NL  {

    struct  NullType  {   };

    template<int anu_NUM, class anu_TYPE>
    struct  NumList  {

        enum { value = anu_NUM };

        typedef anu_TYPE    tail;
    };

    template<class anu_TYPE, int anu_NUM>
    struct  Append  {

        typedef DMScu_NL::NumList<anu_NUM, anu_TYPE>   Result;
    };

    // template<class anu_TYPE, class anu_TYPE_LIST>
    // struct  AppendList  {

    //     typedef typename DMScu_NL::Append<anu_TYPE_LIST,
    //                                       anu_TYPE::value>::Result  Result;
    // };

    template<class anu_TYPE>
    struct  Length  {

        enum { value = DMScu_NL::Length<typename anu_TYPE::tail>::value + 1 };
    };

    template<unsigned int anu_LEN, int anu_DEF_NUM>
    struct  InitNumList  {

        typedef DMScu_NL::NumList
                    <anu_DEF_NUM,
                     typename DMScu_NL::InitNumList
                         <anu_LEN - 1, anu_DEF_NUM>::Result>    Result;
    };

    template<class anu_TYPE, unsigned int anu_IDX>
    struct  NumAt  {

        enum { value = DMScu_NL::NumAt<typename anu_TYPE::tail,
                                       anu_IDX - 1>::value };
    };

    template<class anu_TYPE, unsigned int anu_IDX>
    struct  Erase  {

        typedef typename DMScu_NL::Erase<typename anu_TYPE::tail,
                                         anu_IDX - 1>::Result   Result;
    };
};

template<>
struct  DMScu_NL::Length<DMScu_NL::NullType>  {

    enum { value = 0 };
};

template<int anu_DEF_NUM>
struct  DMScu_NL::InitNumList<0, anu_DEF_NUM>  {

    typedef DMScu_NL::NullType  Result;
};

template<class anu_TYPE>
struct  DMScu_NL::Erase<anu_TYPE, 0>  {

    typedef anu_TYPE    Result;
};

template<class anu_TYPE>
struct  DMScu_NL::NumAt<anu_TYPE, 0>  {

    enum { value = anu_TYPE::value };
};

// ----------------------------------------------------------------------------

//
// Arithmetic Operator Functors
//

template<class cu_CLASS1, class cu_CLASS2 = cu_CLASS1,
         class cu_RESULT = cu_CLASS1>
class   DMScu_Plus :
    public std::binary_function<cu_CLASS1, cu_CLASS2, cu_RESULT>  {

    public:

        typedef typename std::binary_function<cu_CLASS1,
                                              cu_CLASS2,
                                              cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type          result_type;
        typedef typename BaseClass::first_argument_type  first_argument_type;
        typedef typename BaseClass::second_argument_type second_argument_type;

        inline DMScu_Plus () throw ()  {   }
        inline result_type operator () (const first_argument_type &lhs,
                                        const second_argument_type &rhs)
            const throw ()  { return (lhs + rhs); }
};

// ----------------------------------------------------------------------------

template<class cu_CLASS1, class cu_CLASS2 = cu_CLASS1,
         class cu_RESULT = cu_CLASS1>
class   DMScu_Minus :
    public std::binary_function<cu_CLASS1, cu_CLASS2, cu_RESULT>  {

    public:

        typedef typename std::binary_function<cu_CLASS1,
                                              cu_CLASS2,
                                              cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type          result_type;
        typedef typename BaseClass::first_argument_type  first_argument_type;
        typedef typename BaseClass::second_argument_type second_argument_type;

        inline DMScu_Minus () throw ()  {   }
        inline result_type operator () (const first_argument_type &lhs,
                                        const second_argument_type &rhs)
            const throw ()  { return (lhs - rhs); }
};

// ----------------------------------------------------------------------------

template<class cu_CLASS1, class cu_CLASS2 = cu_CLASS1,
         class cu_RESULT = cu_CLASS1>
class   DMScu_Multiplies :
    public std::binary_function<cu_CLASS1, cu_CLASS2, cu_RESULT>  {

    public:

        typedef typename std::binary_function<cu_CLASS1,
                                              cu_CLASS2,
                                              cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type          result_type;
        typedef typename BaseClass::first_argument_type  first_argument_type;
        typedef typename BaseClass::second_argument_type second_argument_type;

        inline DMScu_Multiplies () throw ()  {   }
        inline result_type operator () (const first_argument_type &lhs,
                                        const second_argument_type &rhs)
            const throw ()  { return (lhs * rhs); }
};

// ----------------------------------------------------------------------------

template<class cu_CLASS1, class cu_CLASS2 = cu_CLASS1,
         class cu_RESULT = cu_CLASS1>
class   DMScu_Divides :
    public std::binary_function<cu_CLASS1, cu_CLASS2, cu_RESULT>  {

    public:

        typedef typename std::binary_function<cu_CLASS1,
                                              cu_CLASS2,
                                              cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type          result_type;
        typedef typename BaseClass::first_argument_type  first_argument_type;
        typedef typename BaseClass::second_argument_type second_argument_type;

        inline DMScu_Divides () throw ()  {   }
        inline result_type operator () (const first_argument_type &lhs,
                                        const second_argument_type &rhs)
            const throw ()  { return (lhs / rhs); }
};

// ----------------------------------------------------------------------------

template<class cu_CLASS1, class cu_CLASS2 = cu_CLASS1,
         class cu_RESULT = cu_CLASS1>
class   DMScu_Modulus :
    public std::binary_function<cu_CLASS1, cu_CLASS2, cu_RESULT>  {

    public:

        typedef typename std::binary_function<cu_CLASS1,
                                              cu_CLASS2,
                                              cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type          result_type;
        typedef typename BaseClass::first_argument_type  first_argument_type;
        typedef typename BaseClass::second_argument_type second_argument_type;

        inline DMScu_Modulus () throw ()  {   }
        inline result_type operator () (const first_argument_type &lhs,
                                        const second_argument_type &rhs)
            const throw ()  { return (lhs % rhs); }
};

// ----------------------------------------------------------------------------

template<class cu_CLASS, class cu_RESULT = cu_CLASS>
class   DMScu_Negate : public std::unary_function<cu_CLASS, cu_RESULT>  {

    public:

        typedef typename std::unary_function<cu_CLASS, cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type                  result_type;
        typedef typename BaseClass::argument_type                argument_type;

        inline DMScu_Negate () throw ()  {   }
        inline result_type operator () (const argument_type &lhs)
            const throw ()  { return (-lhs); }
};

// ----------------------------------------------------------------------------

//
// Scaler Expressions
//

template<class cu_CLASS, class cu_RESULT = cu_CLASS>
class   DMScu_Identity : public std::unary_function<cu_CLASS, cu_RESULT>  {

    public:

        typedef typename std::unary_function<cu_CLASS, cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type                  result_type;
        typedef typename BaseClass::argument_type                argument_type;

        inline DMScu_Identity () throw ()  {   }

        inline result_type operator () (const argument_type &lhs)
            const throw ()  { return (static_cast<result_type>(lhs)); }
};

// ----------------------------------------------------------------------------

template<class cu_CLASS, class cu_RESULT = cu_CLASS>
class   DMScu_Literal : public std::unary_function<cu_CLASS, cu_RESULT>  {

    private:

        const   cu_CLASS    value_;

    public:

        typedef typename std::unary_function<cu_CLASS, cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type                  result_type;
        typedef typename BaseClass::argument_type                argument_type;

        inline DMScu_Literal () throw () : value_ ()  {   }
        inline explicit DMScu_Literal (const cu_CLASS &v) throw ()
            : value_ (v)  {   }

        inline result_type operator () (const argument_type &)
            const throw ()  { return (static_cast<result_type>(value_)); }
};

// ----------------------------------------------------------------------------

template<class cu_EXP, class cu_ARG>
class   DMScu_Expression : public std::unary_function<cu_ARG, cu_ARG>  {

    private:

        const   cu_EXP  exp_;

    public:

        typedef typename std::unary_function<cu_ARG, cu_ARG>    BaseClass;
        typedef typename BaseClass::result_type                 result_type;
        typedef typename BaseClass::argument_type               argument_type;

        inline DMScu_Expression () throw () : exp_ ()  {   }
        inline explicit DMScu_Expression (const cu_EXP &e) throw ()
            : exp_ (e)  {   }

        inline result_type operator () (const argument_type &x)
            const throw ()  { return (exp_ (x)); }
};

// ----------------------------------------------------------------------------

template<class cu_EXPR1, class cu_EXPR2, class cu_OPT, class cu_ARG>
class   DMScu_BinExprOpt : public std::unary_function<cu_ARG, cu_ARG>  {

    private:

        const   cu_EXPR1    expr1_;
        const   cu_EXPR2    expr2_;
        const   cu_OPT      opt_;

    public:

        typedef typename std::unary_function<cu_ARG, cu_ARG>    BaseClass;
        typedef typename BaseClass::result_type                 result_type;
        typedef typename BaseClass::argument_type               argument_type;

        inline DMScu_BinExprOpt () throw ()
            : expr1_ (), expr2_ (), opt_ ()  {   }
        inline DMScu_BinExprOpt (const cu_EXPR1 &e1,
                                 const cu_EXPR2 &e2) throw ()
            : expr1_ (e1), expr2_ (e2), opt_ ()  {   }

        inline result_type operator () (argument_type x) const throw ()  {

            return (opt_ (expr1_ (x), expr2_ (x)));
        }
};

// ----------------------------------------------------------------------------

template<class cu_EXPR, class cu_OPT, class cu_ARG>
class   DMScu_UnaExprOpt : public std::unary_function<cu_ARG, cu_ARG>  {

    private:

        const   cu_EXPR expr_;
        const   cu_OPT  opt_;

    public:

        typedef typename std::unary_function<cu_ARG, cu_ARG>    BaseClass;
        typedef typename BaseClass::result_type                 result_type;
        typedef typename BaseClass::argument_type               argument_type;

        inline DMScu_UnaExprOpt () throw () : expr_ (), opt_ ()  {   }
        inline DMScu_UnaExprOpt (const cu_EXPR &e) throw ()
            : expr_ (e), opt_ ()  {   }

        inline result_type operator () (argument_type x) const throw ()  {

            return (opt_ (expr_ (x)));
        }
};

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_RESULT = cu_TYPE>
class   DMScu_Power :
    public std::binary_function<cu_TYPE, cu_TYPE, cu_RESULT>  {

    public:

        typedef typename std::binary_function<cu_TYPE,
                                              cu_TYPE,
                                              cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type          result_type;
        typedef typename BaseClass::first_argument_type  first_argument_type;
        typedef typename BaseClass::second_argument_type second_argument_type;

        inline DMScu_Power () throw ()  {   }
        inline result_type operator () (const first_argument_type &lhs,
                                        const second_argument_type &rhs)
            const throw ()  { return (::pow (lhs, rhs)); }

       // ::pow (Literal, Expression)
       //
        template<class cu_EXPR>
        inline
        DMScu_Expression<DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                                          DMScu_Expression<cu_EXPR, cu_TYPE>,
                                          DMScu_Power<cu_TYPE>,
                                          cu_TYPE>,
                         cu_TYPE>
        operator () (const first_argument_type &lhs,
                     const DMScu_Expression<cu_EXPR, cu_TYPE> &rhs)
            const throw ()  {

            typedef DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                                     DMScu_Expression<cu_EXPR, cu_TYPE>,
                                     DMScu_Power<cu_TYPE>,
                                     cu_TYPE>   expr_type;

            return (DMScu_Expression<expr_type, cu_TYPE>
                        (expr_type (DMScu_Literal<cu_TYPE>(lhs), rhs)));
        }

       // ::pow (Expression, Literal)
       //
        template<class cu_EXPR>
        inline
        DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                          DMScu_Literal<cu_TYPE>,
                                          DMScu_Power<cu_TYPE>,
                                          cu_TYPE>,
                         cu_TYPE>
        operator () (const DMScu_Expression<cu_EXPR, cu_TYPE> &lhs,
                     const second_argument_type &rhs) const throw ()  {

            typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                     DMScu_Literal<cu_TYPE>,
                                     DMScu_Power<cu_TYPE>,
                                     cu_TYPE>   expr_type;

            return (DMScu_Expression<expr_type, cu_TYPE>
                        (expr_type (lhs, DMScu_Literal<cu_TYPE>(rhs))));
        }

       // ::pow (Expression, Expression)
       //
        template<class cu_EXPR>
        inline
        DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                          DMScu_Expression<cu_EXPR, cu_TYPE>,
                                          DMScu_Power<cu_TYPE>,
                                          cu_TYPE>,
                         cu_TYPE>
        operator () (const DMScu_Expression<cu_EXPR, cu_TYPE> &lhs,
                     const DMScu_Expression<cu_EXPR, cu_TYPE> &rhs)
            const throw ()  {

            typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                     DMScu_Expression<cu_EXPR, cu_TYPE>,
                                     DMScu_Power<cu_TYPE>,
                                     cu_TYPE>   expr_type;

            return (DMScu_Expression<expr_type, cu_TYPE>
                        (expr_type (DMScu_Literal<cu_TYPE>(lhs),
                                    DMScu_Literal<cu_TYPE>(rhs))));
        }
};

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_RESULT = cu_TYPE>
class   DMScu_NLog : public std::unary_function<cu_TYPE, cu_RESULT>  {

    public:

        typedef typename std::unary_function<cu_TYPE, cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type                  result_type;
        typedef typename BaseClass::argument_type                argument_type;

        inline DMScu_NLog () throw ()  {   }
        inline result_type operator () (const argument_type &lhs)
            const throw ()  { return (::log (lhs)); }

       // ::log (Expression)
       //
        template<class cu_EXPR>
        inline
        DMScu_Expression<DMScu_UnaExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                          DMScu_NLog<cu_TYPE>,
                                          cu_TYPE>,
                         cu_TYPE>
        operator () (const DMScu_Expression<cu_EXPR, cu_TYPE> &x)
            const throw ()  {

            typedef DMScu_UnaExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                     DMScu_NLog<cu_TYPE>,
                                     cu_TYPE>   expr_type;

            return (DMScu_Expression<expr_type, cu_TYPE>
                        (expr_type (DMScu_Literal<cu_TYPE>(x))));
        }
};

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_RESULT = cu_TYPE>
class   DMScu_EExpo : public std::unary_function<cu_TYPE, cu_RESULT>  {

    public:

        typedef typename std::unary_function<cu_TYPE, cu_RESULT> BaseClass;
        typedef typename BaseClass::result_type                  result_type;
        typedef typename BaseClass::argument_type                argument_type;

        inline DMScu_EExpo () throw ()  {   }
        inline result_type operator () (const argument_type &lhs)
            const throw ()  { return (::exp (lhs)); }

       // ::exp (Expression)
       //
        template<class cu_EXPR>
        inline
        DMScu_Expression<DMScu_UnaExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                          DMScu_NLog<cu_TYPE>,
                                          cu_TYPE>,
                         cu_TYPE>
        operator () (const DMScu_Expression<cu_EXPR, cu_TYPE> &x)
            const throw ()  {

            typedef DMScu_UnaExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                     DMScu_NLog<cu_TYPE>,
                                     cu_TYPE>   expr_type;

            return (DMScu_Expression<expr_type, cu_TYPE>
                        (expr_type (DMScu_Literal<cu_TYPE>(x))));
        }
};

// ----------------------------------------------------------------------------

//
// Literal Opt Expression
//

template<class cu_TYPE, class cu_EXPR>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                                  DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Plus<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator + (const cu_TYPE &lhs,
            const DMScu_Expression<cu_EXPR, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                             DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Plus<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (DMScu_Literal<cu_TYPE>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_EXPR>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                                  DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Minus<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator - (const cu_TYPE &lhs,
            const DMScu_Expression<cu_EXPR, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                             DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Minus<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (DMScu_Literal<cu_TYPE>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_EXPR>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                                  DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Multiplies<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator * (const cu_TYPE &lhs,
            const DMScu_Expression<cu_EXPR, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                             DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Multiplies<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (DMScu_Literal<cu_TYPE>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_EXPR>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                                  DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Divides<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator / (const cu_TYPE &lhs,
            const DMScu_Expression<cu_EXPR, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Literal<cu_TYPE>,
                             DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Divides<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (DMScu_Literal<cu_TYPE>(lhs), rhs)));
}

// ----------------------------------------------------------------------------

//
// Expression Opt Literal
//

template<class cu_EXPR, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Literal<cu_TYPE>,
                                  DMScu_Plus<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator + (const DMScu_Expression<cu_EXPR, cu_TYPE> &lhs,
            const cu_TYPE &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Literal<cu_TYPE>,
                             DMScu_Plus<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (lhs, DMScu_Literal<cu_TYPE>(rhs))));
}

// ----------------------------------------------------------------------------

template<class cu_EXPR, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Literal<cu_TYPE>,
                                  DMScu_Minus<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator - (const DMScu_Expression<cu_EXPR, cu_TYPE> &lhs,
            const cu_TYPE &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Literal<cu_TYPE>,
                             DMScu_Minus<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (lhs, DMScu_Literal<cu_TYPE>(rhs))));
}

// ----------------------------------------------------------------------------

template<class cu_EXPR, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Literal<cu_TYPE>,
                                  DMScu_Multiplies<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator * (const DMScu_Expression<cu_EXPR, cu_TYPE> &lhs,
            const cu_TYPE &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Literal<cu_TYPE>,
                             DMScu_Multiplies<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (lhs, DMScu_Literal<cu_TYPE>(rhs))));
}

// ----------------------------------------------------------------------------

template<class cu_EXPR, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                                  DMScu_Literal<cu_TYPE>,
                                  DMScu_Divides<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator / (const DMScu_Expression<cu_EXPR, cu_TYPE> &lhs,
            const cu_TYPE &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR, cu_TYPE>,
                             DMScu_Literal<cu_TYPE>,
                             DMScu_Divides<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE>
                (expr_type (lhs, DMScu_Literal<cu_TYPE>(rhs))));
}

// ----------------------------------------------------------------------------

//
// Expression Opt Expression
//

template<class cu_EXPR1, class cu_EXPR2, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                                  DMScu_Expression<cu_EXPR2, cu_TYPE>,
                                  DMScu_Plus<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator + (const DMScu_Expression<cu_EXPR1, cu_TYPE> &lhs,
            const DMScu_Expression<cu_EXPR2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                             DMScu_Expression<cu_EXPR2, cu_TYPE>,
                             DMScu_Plus<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_EXPR1, class cu_EXPR2, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                                  DMScu_Expression<cu_EXPR2, cu_TYPE>,
                                  DMScu_Minus<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator - (const DMScu_Expression<cu_EXPR1, cu_TYPE> &lhs,
            const DMScu_Expression<cu_EXPR2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                             DMScu_Expression<cu_EXPR2, cu_TYPE>,
                             DMScu_Minus<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_EXPR1, class cu_EXPR2, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                                  DMScu_Expression<cu_EXPR2, cu_TYPE>,
                                  DMScu_Multiplies<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator * (const DMScu_Expression<cu_EXPR1, cu_TYPE> &lhs,
            const DMScu_Expression<cu_EXPR2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                             DMScu_Expression<cu_EXPR2, cu_TYPE>,
                             DMScu_Multiplies<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_EXPR1, class cu_EXPR2, class cu_TYPE>
inline
DMScu_Expression<DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                                  DMScu_Expression<cu_EXPR2, cu_TYPE>,
                                  DMScu_Divides<cu_TYPE>,
                                  cu_TYPE>,
                 cu_TYPE>
operator / (const DMScu_Expression<cu_EXPR1, cu_TYPE> &lhs,
            const DMScu_Expression<cu_EXPR2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_BinExprOpt<DMScu_Expression<cu_EXPR1, cu_TYPE>,
                             DMScu_Expression<cu_EXPR2, cu_TYPE>,
                             DMScu_Divides<cu_TYPE>,
                             cu_TYPE>   expr_type;

    return (DMScu_Expression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

//
// Vector-based constructs
//

// The only reason for this class is the need for the assignment operator
// for Expressions, unfortunately.
//
template<class cu_TYPE>
class   DMScu_Vector : public std::vector<cu_TYPE>  {

    public:

        typedef std::vector<cu_TYPE>            BaseClass;
        typedef typename BaseClass::size_type   size_type;
        typedef typename BaseClass::value_type  value_type;

        DMScu_Vector () throw () : BaseClass ()  {   }
        DMScu_Vector (size_type n) throw () : BaseClass (n)  {   }
        DMScu_Vector (size_type n, const value_type &v) throw ()
            : BaseClass (n, v)  {   }

        inline DMScu_Vector &operator = (const DMScu_Vector &rhs) throw ()  {

            BaseClass::operator = (rhs);
            return (*this);
        }

        template<class cu_EXPR>
        inline DMScu_Vector &operator = (const cu_EXPR &rhs) throw ()  {

            rhs.assign (*this);
            return (*this);
        }
};

// ----------------------------------------------------------------------------

//
// Vector-based Expressions
//

template<class cu_ITER, class cu_TYPE>
class   DMScu_VecExpression  {

    private:

        cu_ITER citer_;

    public:

        typedef cu_TYPE value_type;

        inline explicit DMScu_VecExpression (const cu_ITER &it) throw ()
            : citer_ (it)  {   }

        inline DMScu_VecExpression &operator ++ () throw ()  {    // ++Prefix

            ++citer_;
            return (*this);
        }
        inline DMScu_VecExpression operator ++ (int) throw ()  { // Postfix++

            DMScu_VecExpression slug (citer_);

            ++citer_;
            return (slug);
        }

        inline value_type operator * () const throw ()  { return (*citer_); }
        inline value_type operator [] (unsigned int i) const throw ()  {

            return (*(citer_ + i));
        }

        inline void assign (DMScu_Vector<cu_TYPE> &lhs) const throw ()  {

            cu_ITER slug = citer_;

            for (typename DMScu_Vector<cu_TYPE>::iterator iter = lhs.begin ();
                 iter != lhs.end (); ++iter, ++slug)
                *iter = *slug;
        }
};

// ----------------------------------------------------------------------------

template<class cu_ITER1, class cu_ITER2, class cu_OPT, class cu_TYPE>
class   DMScu_VecBinExprOpt  {

    private:

        cu_ITER1        citer1_;
        cu_ITER2        citer2_;
        const   cu_OPT  opt_;

    public:

        typedef cu_TYPE value_type;

        inline DMScu_VecBinExprOpt (const cu_ITER1 &it1,
                                    const cu_ITER2 &it2) throw ()
            : citer1_ (it1), citer2_ (it2), opt_ ()  {   }

        inline DMScu_VecBinExprOpt &operator ++ () throw ()  {    // ++Prefix

            ++citer1_;
            ++citer2_;
            return (*this);
        }
        inline DMScu_VecBinExprOpt operator ++ (int) throw ()  { // Postfix++

            DMScu_VecBinExprOpt slug (citer1_, citer2_);

            ++citer1_;
            ++citer2_;
            return (slug);
        }

        inline value_type operator * () const throw ()  {

            return (opt_ (*citer1_, *citer2_));
        }
        inline value_type operator [] (unsigned int i) const throw ()  {

            return (opt_ (*(citer1_ + i), *(citer2_ + i)));
        }
};

// ----------------------------------------------------------------------------

//
// Vector-based: Vector Opt Vector
//

template<class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Plus<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator + (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Plus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Minus<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator - (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Minus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Multiplies<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator * (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Multiplies<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Divides<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator / (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Divides<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs.begin ())));
}

// ----------------------------------------------------------------------------

//
// Vector-based: Expression Opt Vector
//

template<class cu_ITER, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Plus<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator + (const DMScu_VecExpression<cu_ITER, cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Plus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class cu_ITER, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Minus<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator - (const DMScu_VecExpression<cu_ITER, cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Minus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class cu_ITER, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Multiplies<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator * (const DMScu_VecExpression<cu_ITER, cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Multiplies<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

template<class cu_ITER, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_Divides<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator / (const DMScu_VecExpression<cu_ITER, cu_TYPE> &lhs,
            const DMScu_Vector<cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_Divides<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs, rhs.begin ())));
}

// ----------------------------------------------------------------------------

//
// Vector-based: Vector Opt Expression
//

template<class cu_TYPE, class cu_ITER>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        DMScu_Plus<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator + (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                DMScu_Plus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_ITER>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        DMScu_Minus<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator - (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                DMScu_Minus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_ITER>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        DMScu_Multiplies<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator * (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                DMScu_Multiplies<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_TYPE, class cu_ITER>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<
                        typename DMScu_Vector<cu_TYPE>::const_iterator,
                        DMScu_VecExpression<cu_ITER, cu_TYPE>,
                        DMScu_Divides<cu_TYPE>,
                        cu_TYPE>,
                    cu_TYPE>
operator / (const DMScu_Vector<cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<typename DMScu_Vector<cu_TYPE>::const_iterator,
                                DMScu_VecExpression<cu_ITER, cu_TYPE>,
                                DMScu_Divides<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE>
                (expr_type (lhs.begin (), rhs)));
}

// ----------------------------------------------------------------------------

//
// Vector-based: Expression Opt Expression
//

template<class cu_ITER1, class cu_ITER2, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                        DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                        DMScu_Plus<cu_TYPE>,
                                        cu_TYPE>,
                    cu_TYPE>
operator + (const DMScu_VecExpression<cu_ITER1, cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                DMScu_Plus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_ITER1, class cu_ITER2, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                        DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                        DMScu_Minus<cu_TYPE>,
                                        cu_TYPE>,
                    cu_TYPE>
operator - (const DMScu_VecExpression<cu_ITER1, cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                DMScu_Minus<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_ITER1, class cu_ITER2, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                        DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                        DMScu_Multiplies<cu_TYPE>,
                                        cu_TYPE>,
                    cu_TYPE>
operator * (const DMScu_VecExpression<cu_ITER1, cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                DMScu_Multiplies<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

template<class cu_ITER1, class cu_ITER2, class cu_TYPE>
inline
DMScu_VecExpression<DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                        DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                        DMScu_Divides<cu_TYPE>,
                                        cu_TYPE>,
                    cu_TYPE>
operator / (const DMScu_VecExpression<cu_ITER1, cu_TYPE> &lhs,
            const DMScu_VecExpression<cu_ITER2, cu_TYPE> &rhs) throw ()  {

    typedef DMScu_VecBinExprOpt<DMScu_VecExpression<cu_ITER1, cu_TYPE>,
                                DMScu_VecExpression<cu_ITER2, cu_TYPE>,
                                DMScu_Divides<cu_TYPE>,
                                cu_TYPE>    expr_type;

    return (DMScu_VecExpression<expr_type, cu_TYPE> (expr_type (lhs, rhs)));
}

// ----------------------------------------------------------------------------

#undef _INCLUDED_DMScu_MathOperators_h
#define _INCLUDED_DMScu_MathOperators_h 1
#endif  // _INCLUDED_DMScu_MathOperators_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
