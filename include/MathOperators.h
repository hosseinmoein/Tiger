// Hossein Moein
// February 11, 2018
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

#ifndef _INCLUDED_MathOperators_h
#define _INCLUDED_MathOperators_h 0

#include <stdexcept>
#include <iterator>

#include <Matrix.h>

// ----------------------------------------------------------------------------

//
// Matrix-based arithmetic operator functors
//

// ----------------------------------------------------------------------------

namespace hmma
{

struct  MatrixOptBase  {

    enum TYPE { _plus_ = 1, _minus_, _multiply_, _mean_, _transpose_ };

    typedef unsigned int    size_type;
};

// ----------------------------------------------------------------------------

template<class CLASS>
class   MatPlus : public MatrixOptBase  {

    public:

        typedef CLASS   value_type;
        typedef value_type  result_type;

    public:

        inline MatPlus (size_type) noexcept  {   }

        static inline TYPE type () noexcept  { return (_plus_); }

        template<class RHS_ARG_TYPE, class LHS_ARG_TYPE>
        inline result_type operator () (const RHS_ARG_TYPE &lhs,
                                        const LHS_ARG_TYPE &rhs,
                                        size_type,
                                        size_type) const noexcept  {

            return (*lhs + *rhs);
        }
};

// ----------------------------------------------------------------------------

template<class CLASS>
class   MatMinus : public MatrixOptBase  {

    public:

        typedef CLASS   value_type;
        typedef value_type  result_type;

    public:

        inline MatMinus (size_type) noexcept  {   }

        static inline TYPE type () noexcept  { return (_minus_); }

        template<class RHS_ARG_TYPE, class LHS_ARG_TYPE>
        inline result_type operator () (const RHS_ARG_TYPE &lhs,
                                        const LHS_ARG_TYPE &rhs,
                                        size_type,
                                        size_type) const noexcept  {

            return (*lhs - *rhs);
        }
};

// ----------------------------------------------------------------------------

//     +---+---+
//     | A | B |
//     +---+---+
// n = | C | D |
//     +---+---+
//     | E | F |
//     +---+---+
//
//     +---+---+---+---+
//     | Z | Y | X | W |
// m = +---+---+---+---+
//     | V | U | T | S |
//     +---+---+---+---+
//
//       +---------------+---------------+---------------+---------------+
//       | (A*Z) + (B*V) | (A*Y) + (B*U) | (A*X) + (B*T) | (A*W) + (B*S) |
//       +---------------+---------------+---------------+---------------+
// m*n = | (C*Z) + (D*V) | (C*Y) + (D*U) | (C*X) + (D*T) | (C*W) + (D*S) |
//       +---------------+---------------+---------------+---------------+
//       | (E*Z) + (F*V) | (E*Y) + (F*U) | (E*X) + (F*T) | (E*W) + (F*S) |
//       +---------------+---------------+---------------+---------------+
//
template<class CLASS>
class   MatMultiplies : public MatrixOptBase  {

    public:

        typedef CLASS       value_type;
        typedef value_type  result_type;

    public:

        inline MatMultiplies (size_type) noexcept  {   }

        static inline TYPE type () noexcept  { return (_multiply_); }

       // NOTE: It would have been easier/more natural if lhs were a row
       //       iterator. But for compilation reasons it cannot be.
       //       That is why the multiplication may seem strange but it is
       //       correct. 
       //
        template<class RHS_ARG_TYPE, class LHS_ARG_TYPE>
        inline result_type
        operator () (RHS_ARG_TYPE lhs,
                     LHS_ARG_TYPE rhs,
                     size_type rhs_row_size,
                     size_type lhs_row_size) const noexcept  {

            result_type v = 0;

            while (rhs_row_size-- > 0)  {
                v += *lhs * *rhs;
                lhs += lhs_row_size;
                ++rhs;
            }

            return (v);
        }
};

// ----------------------------------------------------------------------------

// Column means
//
template<class CLASS>
class   MatColMean : public MatrixOptBase  {

    public:

        typedef CLASS       value_type;
        typedef value_type  result_type;

    public:

        inline MatColMean (size_type) noexcept  {   }

        static size_type lhs_row_size () noexcept  { return (1); }

        static inline TYPE type () noexcept  { return (_mean_); }

        template<class ARG_TYPE>
        inline result_type
        operator () (ARG_TYPE &rhs, size_type rhs_row_size) const noexcept  {

            result_type v = 0;

            for (size_type i = 0; i < rhs_row_size; ++i, ++rhs)
                v += *rhs;

           // This is an ugly hack so it works with other operators
           //
            rhs -= 1;

            return (v / static_cast<result_type>(rhs_row_size));
        }
};

// ----------------------------------------------------------------------------

// Matrix transpose means
//
template<class CLASS>
class   MatTranspose : public MatrixOptBase  {

    public:

        typedef CLASS       value_type;
        typedef value_type  result_type;

    private:

        const   size_type   lhs_row_size_;

    public:

        inline MatTranspose (size_type lrs) noexcept
            : lhs_row_size_ (lrs)  {   }

        inline size_type lhs_row_size () const noexcept  {

            return (lhs_row_size_);
        }

        static inline TYPE type () noexcept  { return (_transpose_); }

        template<class ARG_TYPE>
        inline result_type
        operator () (const ARG_TYPE &rhs, const size_type &) const noexcept  {

            return (*rhs);
        }
};

// ----------------------------------------------------------------------------

//
// Matrix-based Expressions
//

template<class ITER, class OPT, class TYPE>
class   MatUnaExprOpt  {

    public:

        typedef TYPE                     value_type;
        typedef MatrixOptBase::size_type size_type;

    private:

        mutable ITER        rhs_citer_;
        const   size_type   rhs_row_size_;
        const   size_type   rhs_col_size_;
        const   OPT         opt_;

    public:

        inline MatUnaExprOpt (const ITER &it,
                              size_type rhs_row_size,
                              size_type rhs_col_size) noexcept
            : rhs_citer_ (it),
              rhs_row_size_ (rhs_row_size),
              rhs_col_size_ (rhs_col_size),
              opt_ (rhs_col_size)  {   }

        inline void lhs_col_increment (size_type)  {

            throw std::runtime_error("MatUnaExprOpt::"
                                     "lhs_col_increment(): mustn't be called");
        }
        inline void lhs_col_decrement (size_type)  {

            throw std::runtime_error("MatUnaExprOpt::"
                                     "lhs_col_decrement(): mustn't be called");
        }
        inline void
        rhs_col_increment (size_type i) noexcept  { rhs_citer_ += i; }
        inline void
        rhs_col_decrement (size_type i) noexcept  { rhs_citer_ -= i; }

        inline MatUnaExprOpt &operator ++ () noexcept  {    // ++Prefix

            rhs_col_increment (1);
            return (*this);
        }
        inline MatUnaExprOpt &operator += (size_type i) noexcept  {

            rhs_col_increment (i);
            return (*this);
        }

        inline MatUnaExprOpt &operator -- () noexcept  {    // --Prefix

            rhs_col_decrement (1);
            return (*this);
        }
        inline MatUnaExprOpt &operator -= (size_type i) noexcept  {

            rhs_col_decrement (i);
            return (*this);
        }

        inline size_type
        lhs_row_size () const noexcept  { return (opt_.lhs_row_size ()); }
        inline size_type rhs_row_size () const noexcept  {

            return (opt_.type () == MatrixOptBase::_transpose_
                        ? rhs_col_size_ : rhs_row_size_);
        }
        inline size_type rhs_col_size () const noexcept  {

            return (opt_.type () == MatrixOptBase::_transpose_
                        ? rhs_row_size_ : rhs_col_size_);
        }

        inline value_type operator * () const noexcept  {

            return (opt_ (rhs_citer_, rhs_row_size_));
        }
};

// ----------------------------------------------------------------------------

template<class ITER1, class ITER2, class OPT, class TYPE>
class   MatBinExprOpt  {

    public:

        typedef TYPE                     value_type;
        typedef MatrixOptBase::size_type size_type;

    private:

        ITER1               lhs_citer_;
        ITER2               rhs_citer_;
        const   size_type   lhs_row_size_;
        const   size_type   rhs_row_size_;
        const   size_type   rhs_col_size_;
        const   OPT         opt_;

    public:

        inline MatBinExprOpt (const ITER1 &it1,
                              const ITER2 &it2,
                              size_type lhs_row_size,
                              size_type rhs_row_size,
                              size_type rhs_col_size) noexcept
            : lhs_citer_ (it1),
              rhs_citer_ (it2),
              lhs_row_size_ (lhs_row_size),
              rhs_row_size_ (rhs_row_size),
              rhs_col_size_ (rhs_col_size),
              opt_ (rhs_col_size)  {   }

        inline void
        lhs_col_increment (size_type i) noexcept  { lhs_citer_ += i; }
        inline void
        lhs_col_decrement (size_type i) noexcept  { lhs_citer_ -= i; }
        inline void
        rhs_col_increment (size_type i) noexcept  { rhs_citer_ += i; }
        inline void
        rhs_col_decrement (size_type i) noexcept  { rhs_citer_ -= i; }

        inline MatBinExprOpt &operator ++ () noexcept  {  // ++Prefix

            lhs_col_increment (1);
            rhs_col_increment (1);
            return (*this);
        }
        inline MatBinExprOpt &operator += (size_type i) noexcept  {

            lhs_col_increment (i);
            rhs_col_increment (i);
            return (*this);
        }

        inline MatBinExprOpt &operator -- () noexcept  {  // --Prefix

            lhs_col_decrement (1);
            rhs_col_decrement (1);
            return (*this);
        }
        inline MatBinExprOpt &operator -= (size_type i) noexcept  {

            lhs_col_decrement (i);
            rhs_col_decrement (i);
            return (*this);
        }

        inline size_type
        lhs_row_size () const noexcept  { return (lhs_row_size_); }
        inline size_type
        rhs_row_size () const noexcept  { return (rhs_row_size_); }
        inline size_type
        rhs_col_size () const noexcept  { return (rhs_col_size_); }

        inline value_type operator * () const noexcept  {

            return (opt_ (lhs_citer_,
                          rhs_citer_,
                          rhs_row_size_,
                          lhs_row_size_));
        }
};

// ----------------------------------------------------------------------------

// Matrix-based Expression
//
template<class ITER, template<class T> class BASE, class TYPE>
class   MatrixExpr  {

    public:

        typedef TYPE                     value_type;
        typedef MatrixOptBase::size_type size_type;

    private:

        typedef Matrix<BASE, TYPE>   MatrixType;

        size_type                   multi_count_;
        ITER                        expr_opt_;
        const   unsigned    char    opt_type_;

    public:

        inline MatrixExpr (const ITER &eo,
                           unsigned char opt_type) noexcept
            : multi_count_ (0), expr_opt_ (eo), opt_type_ (opt_type)  {   }

        inline MatrixExpr &operator ++ () noexcept  {  // ++Prefix

            if  (opt_type_ == static_cast<unsigned char>
                                  (MatrixOptBase::_multiply_))  {
                expr_opt_.lhs_col_increment (1);
                if (! (++multi_count_ % lhs_row_size ()))  {
                    expr_opt_.lhs_col_decrement (lhs_row_size ());
                    expr_opt_.rhs_col_increment (rhs_row_size ());
                }
            }
            else
                ++expr_opt_;

            return (*this);
        }
        inline MatrixExpr &operator += (size_type i) noexcept  {

            if  (opt_type_ == static_cast<unsigned char>
                                  (MatrixOptBase::_multiply_))  {
                expr_opt_.lhs_col_increment (i);
                while (i-- > 0)
                    if (! (++multi_count_ % lhs_row_size ()))  {
                        expr_opt_.lhs_col_decrement (lhs_row_size ());
                        expr_opt_.rhs_col_increment (rhs_row_size ());
                        break;
                    }
            }
            else
                expr_opt_ += i;

            return (*this);
        }

        inline MatrixExpr &operator -- () noexcept  {

            if  (opt_type_ == static_cast<unsigned char>
                                  (MatrixOptBase::_multiply_))  {
                expr_opt_.lhs_col_decrement (1);
                if (! (--multi_count_ % lhs_row_size ()))  {
                    expr_opt_.lhs_col_increment (lhs_row_size ());
                    expr_opt_.rhs_col_decrement (rhs_row_size ());
                }
            }
            else
                --expr_opt_;

            return (*this);
        }
        inline MatrixExpr &operator -= (size_type i) noexcept  {

            if  (opt_type_ == static_cast<unsigned char>
                                  (MatrixOptBase::_multiply_))  {
                expr_opt_.lhs_col_decrement (i);
                while (i-- > 0)
                    if (! (--multi_count_ % lhs_row_size ()))  {
                        expr_opt_.lhs_col_increment (lhs_row_size ());
                        expr_opt_.rhs_col_decrement (rhs_row_size ());
                        break;
                    }
            }
            else
                expr_opt_ -= i;

            return (*this);
        }

        inline value_type
        operator * () const noexcept  { return (*expr_opt_); }

        inline size_type
        lhs_row_size () const noexcept  { return (expr_opt_.lhs_row_size ()); }
        inline size_type
        rhs_row_size () const noexcept  { return (expr_opt_.rhs_row_size ()); }
        inline size_type
        rhs_col_size () const noexcept  { return (expr_opt_.rhs_col_size ()); }

        size_type result_row_size () const noexcept  {

            return (opt_type_ == static_cast<unsigned char>
                                     (MatrixOptBase::_multiply_) ||
                    opt_type_ == static_cast<unsigned char>
                                     (MatrixOptBase::_plus_)
                        ? lhs_row_size ()
                        : opt_type_ == static_cast<unsigned char>
                                           (MatrixOptBase::_mean_)
                              ? 1
                              : rhs_row_size ()); // transpose
        }

        size_type result_col_size () const noexcept  {

            return (rhs_col_size ());
        }

        void assign (MatrixType &lhs) const  {

            const_iterator  rhs_citer = begin ();

            lhs.resize (result_row_size (), result_col_size ());
            for (typename MatrixType::col_iterator lhs_iter = lhs.col_begin ();
                 lhs_iter != lhs.col_end (); ++lhs_iter, ++rhs_citer)
                *lhs_iter = *rhs_citer;

            return;
        }

        inline ITER get_expr_opt () const noexcept  { return (expr_opt_); }
        inline unsigned char get_expr_opt_type () const noexcept  {

            return (opt_type_);
        }

    public:

        class   const_iterator
            : public std::iterator<std::forward_iterator_tag,
                                   value_type, long int>  {

            public:

                typedef std::forward_iterator_tag   iterator_category;

            private:

                const   MatrixExpr   *expr_node_;
                ITER                 expr_opt_;
                size_type            multi_count_;

            public:

               // Defualt constructor initializes the iterator into an
               // _undefied_ state
               //
                inline const_iterator () noexcept
                    : expr_node_ (NULL), expr_opt_ (), multi_count_ (0)  {   }

                inline const_iterator (const MatrixExpr *en) noexcept
                    : expr_node_ (en),
                      expr_opt_ (expr_node_->get_expr_opt ()),
                      multi_count_ (0)  {   }

                inline bool
                operator == (const const_iterator &rhs) const noexcept  {

                    return (expr_node_ == rhs.expr_node_ &&
                            multi_count_ == rhs.multi_count_);
                }
                inline bool
                operator != (const const_iterator &rhs) const noexcept  {

                    return (expr_node_ != rhs.expr_node_ ||
                            multi_count_ != rhs.multi_count_);
                } 

                inline value_type operator * () const noexcept  {
    
                    return (*expr_opt_);
                }
                inline operator value_type () const noexcept  {

                    return (*expr_opt_);
                }

               // ++Prefix
               //
                inline const_iterator &operator ++ () noexcept  {

                    if  (expr_node_->get_expr_opt_type () ==
                             static_cast<unsigned char>
                                 (MatrixOptBase::_multiply_))  {
                        expr_opt_.lhs_col_increment (1);
                        if (! (++multi_count_ % expr_node_->lhs_row_size()))  {
                            expr_opt_.lhs_col_decrement (
                                expr_node_->lhs_row_size ());
                            expr_opt_.rhs_col_increment (
                                expr_node_->rhs_row_size ());
                        }
                    }
                    else
                        ++expr_opt_;

                    return (*this);
                }
        };

        const_iterator begin () const noexcept  { return (this); }
};

// ----------------------------------------------------------------------------

//
// Matrix-based global math operators
//

// ----------------------------------------------------------------------------

// Unary Inverse operator
//
template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE>
operator ! (const Matrix<BASE, TYPE> &rhs)  {

    Matrix<BASE, TYPE>   tmp;

    rhs.inverse (tmp);
    return (tmp);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline Matrix<BASE, TYPE>
operator ! (const MatrixExpr<ITER, BASE, TYPE> &rhs)  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    MatrixType  tmp = rhs;

    tmp.invert ();
    return (tmp);
}

// ----------------------------------------------------------------------------

// Unary Transpose operator
//
template<template<class T> class BASE, class TYPE>
inline
MatrixExpr
    <MatUnaExprOpt
         <typename Matrix<BASE, TYPE>::row_const_iterator,
          MatTranspose<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator ~ (const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatUnaExprOpt<typename MatrixType::row_const_iterator,
                          MatTranspose<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (rhs.row_begin (), rhs.rows (), rhs.columns ()),
                 static_cast<unsigned char>
                     (MatrixOptBase::_transpose_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline
MatrixExpr
    <MatUnaExprOpt<MatrixExpr<ITER, BASE, TYPE>,
                   MatTranspose<TYPE>,
                   TYPE>,
     BASE,
     TYPE>
operator ~ (const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef MatUnaExprOpt<MatrixExpr<ITER, BASE, TYPE>, MatTranspose<TYPE>,
                          TYPE>  expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (rhs, rhs.rhs_row_size (). rhs.rhs_col_size ()),
                 static_cast<unsigned char>
                     (MatrixOptBase::_transpose_)));
}

// ----------------------------------------------------------------------------

// Unary Mean operator
//
template<template<class T> class BASE, class TYPE>
inline
MatrixExpr
    <MatUnaExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          MatColMean<TYPE>,
          TYPE>,
     BASE,
     TYPE>
mat_col_mean (const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatUnaExprOpt<typename MatrixType::col_const_iterator,
                          MatColMean<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (rhs.col_begin (), rhs.rows (), rhs.columns ()),
                 static_cast<unsigned char>(MatrixOptBase::_mean_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline
MatrixExpr
    <MatUnaExprOpt<MatrixExpr<ITER, BASE, TYPE>, MatColMean<TYPE>, TYPE>,
                   BASE, TYPE>
mat_col_mean (const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef MatUnaExprOpt<MatrixExpr<ITER, BASE, TYPE>, MatColMean<TYPE>,
                          TYPE>  expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (rhs, rhs.rhs_row_size (). rhs.rhs_col_size ()),
                 static_cast<unsigned char>(MatrixOptBase::_mean_)));
}

// ----------------------------------------------------------------------------

//
// Matrix-based global arithmetic operators
//

// ----------------------------------------------------------------------------

// Matrix-based: Matrix Opt Matrix
//
template<template<class T> class BASE, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatPlus<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator + (const Matrix<BASE, TYPE> &lhs,
            const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<typename MatrixType::col_const_iterator,
                          typename MatrixType::col_const_iterator,
                          MatPlus<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs.col_begin (),
                            rhs.col_begin (),
                            lhs.rows (),
                            rhs.rows (),
                            rhs.columns ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatMinus<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator - (const Matrix<BASE, TYPE> &lhs,
            const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<typename MatrixType::col_const_iterator,
                          typename MatrixType::col_const_iterator,
                          MatMinus<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs.col_begin (),
                            rhs.col_begin (),
                            lhs.rows (),
                            rhs.rows (),
                            rhs.columns ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatMultiplies<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator * (const Matrix<BASE, TYPE> &lhs,
            const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<typename MatrixType::col_const_iterator,
                          typename MatrixType::col_const_iterator,
                          MatMultiplies<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
               (expr_type (lhs.col_begin (),
                           rhs.col_begin (),
                           lhs.rows (),
                           rhs.rows (),
                           rhs.columns ()),
                static_cast<unsigned char>(MatrixOptBase::_multiply_)));
}

// ----------------------------------------------------------------------------

// NOTE: The rhs _is_ _not_ a const reference. It will be altered and coverted
//       to its inverse.
//
template<template<class T> class BASE, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatMultiplies<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator / (const Matrix<BASE, TYPE> &lhs,
            Matrix<BASE, TYPE> &rhs) noexcept  {

    return (lhs * rhs.invert ());
}

// ----------------------------------------------------------------------------

// Matrix-based: Expression Opt Matrix
//
template<template<class T> class BASE, class ITER, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <MatrixExpr<ITER, BASE, TYPE>,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatPlus<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator + (const MatrixExpr<ITER, BASE, TYPE> &lhs,
            const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<MatrixExpr<ITER,
                                     BASE,
                                     TYPE>,
                          typename MatrixType::col_const_iterator,
                          MatPlus<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs,
                            rhs.col_begin (),
                            lhs.lhs_row_size (),
                            rhs.rows (),
                            rhs.columns ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <MatrixExpr<ITER, BASE, TYPE>,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatMinus<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator - (const MatrixExpr<ITER, BASE, TYPE> &lhs,
            const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<MatrixExpr<ITER,
                                     BASE,
                                     TYPE>,
                          typename MatrixType::col_const_iterator,
                          MatMinus<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs,
                            rhs.col_begin (),
                            lhs.lhs_row_size (),
                            rhs.rows (),
                            rhs.columns ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <MatrixExpr<ITER, BASE, TYPE>,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatMultiplies<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator * (const MatrixExpr<ITER, BASE, TYPE> &lhs,
            const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<MatrixExpr<ITER,
                                     BASE,
                                     TYPE>,
                          typename MatrixType::col_const_iterator,
                          MatMultiplies<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
               (expr_type (lhs,
                           rhs.col_begin (),
                           lhs.lhs_row_size (),
                           rhs.rows (),
                           rhs.columns ()),
                static_cast<unsigned char>(MatrixOptBase::_multiply_)));
}

// ----------------------------------------------------------------------------

// NOTE: The rhs _is_ _not_ a const reference. It will be altered and coverted
//       to its inverse.
//
template<template<class T> class BASE, class ITER, class TYPE>
inline
MatrixExpr
    <MatBinExprOpt
         <MatrixExpr<ITER, BASE, TYPE>,
          typename Matrix<BASE, TYPE>::col_const_iterator,
          MatMultiplies<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator / (const MatrixExpr<ITER, BASE, TYPE> &lhs,
            Matrix<BASE, TYPE> &rhs) noexcept  {

    return (lhs * rhs.invert ());
}

// ----------------------------------------------------------------------------

// Matrix-based: Matrix Opt Expression
//
template<template<class T> class BASE, class TYPE, class ITER>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          MatrixExpr<ITER, BASE, TYPE>,
          MatPlus<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator + (const Matrix<BASE, TYPE> &lhs,
            const MatrixExpr<ITER, BASE, TYPE> &rhs)
    noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<typename MatrixType::col_const_iterator,
                          MatrixExpr<ITER,
                                     BASE,
                                     TYPE>,
                          MatPlus<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs.col_begin (),
                            rhs,
                            lhs.rows (),
                            rhs.rhs_row_size (),
                            rhs.rhs_col_size ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE, class ITER>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          MatrixExpr<ITER, BASE, TYPE>,
          MatMinus<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator - (const Matrix<BASE, TYPE> &lhs,
            const MatrixExpr<ITER, BASE, TYPE> &rhs)
    noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<typename MatrixType::col_const_iterator,
                          MatrixExpr<ITER,
                                     BASE,
                                     TYPE>,
                          MatMinus<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs.col_begin (),
                            rhs,
                            lhs.rows (),
                            rhs.rhs_row_size (),
                            rhs.rhs_col_size ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE, class ITER>
inline
MatrixExpr
    <MatBinExprOpt
         <typename Matrix<BASE, TYPE>::col_const_iterator,
          MatrixExpr<ITER, BASE, TYPE>,
          MatMultiplies<TYPE>,
          TYPE>,
     BASE,
     TYPE>
operator * (const Matrix<BASE, TYPE> &lhs,
            const MatrixExpr<ITER, BASE, TYPE> &rhs)
    noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;
    typedef MatBinExprOpt<typename MatrixType::col_const_iterator,
                          MatrixExpr<ITER,
                                     BASE,
                                     TYPE>,
                          MatMultiplies<TYPE>,
                          TYPE>      expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs.col_begin (),
                            rhs,
                            lhs.rows (),
                            rhs.rhs_row_size (),
                            rhs.rhs_col_size ()),
                static_cast<unsigned char>(MatrixOptBase::_multiply_)));
}

// ----------------------------------------------------------------------------

// template<template<class T> class BASE, class TYPE, class ITER>
// inline
// MatrixExpr
//     <MatBinExprOpt
//          <typename Matrix<BASE, TYPE>::col_const_iterator,
//           typename Matrix<BASE, TYPE>::col_const_iterator,
//           MatMultiplies<TYPE>,
//           TYPE>,
//      BASE,
//      TYPE>
// operator / (const Matrix<BASE, TYPE> &lhs,
//             const MatrixExpr<ITER, BASE, TYPE> &rhs)
//     noexcept  {
// 
//     typedef Matrix<BASE, TYPE>   MatrixType;
// 
//     const   MatrixType  rhs_tmp = rhs;
// 
//     return (lhs * ! rhs_tmp);
// }

// ----------------------------------------------------------------------------

// Matrix-based: Expression Opt Expression
//
template<template<class T> class BASE,
         class ITER1,
         class ITER2,
         class TYPE>
inline
MatrixExpr
    <MatBinExprOpt<MatrixExpr<ITER1, BASE, TYPE>,
                   MatrixExpr<ITER2, BASE, TYPE>,
                   MatPlus<TYPE>,
                   TYPE>,
     BASE,
     TYPE>
operator + (const MatrixExpr<ITER1, BASE, TYPE> &lhs,
            const MatrixExpr<ITER2, BASE, TYPE> &rhs)
    noexcept  {

    typedef MatBinExprOpt<MatrixExpr<ITER1,
                                     BASE,
                                     TYPE>,
                          MatrixExpr<ITER2,
                                     BASE,
                                     TYPE>,
                          MatPlus<TYPE>,
                          TYPE>  expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs,
                            rhs,
                            lhs.lhs_row_size (),
                            rhs.rhs_row_size (),
                            rhs.rhs_col_size ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE,
         class ITER1,
         class ITER2,
         class TYPE>
inline
MatrixExpr
    <MatBinExprOpt<MatrixExpr<ITER1, BASE, TYPE>,
                   MatrixExpr<ITER2, BASE, TYPE>,
                   MatMinus<TYPE>,
                   TYPE>,
     BASE,
     TYPE>
operator - (const MatrixExpr<ITER1, BASE, TYPE> &lhs,
            const MatrixExpr<ITER2, BASE, TYPE> &rhs)
    noexcept  {

    typedef MatBinExprOpt<MatrixExpr<ITER1,
                                     BASE,
                                     TYPE>,
                          MatrixExpr<ITER2,
                                     BASE,
                                     TYPE>,
                          MatMinus<TYPE>,
                          TYPE>  expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
                (expr_type (lhs,
                            rhs,
                            lhs.lhs_row_size (),
                            rhs.rhs_row_size (),
                            rhs.rhs_col_size ()),
                 static_cast<unsigned char>(MatrixOptBase::_plus_)));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE,
         class ITER1,
         class ITER2,
         class TYPE>
inline
MatrixExpr
    <MatBinExprOpt<MatrixExpr<ITER1, BASE, TYPE>,
                   MatrixExpr<ITER2, BASE, TYPE>,
                   MatMultiplies<TYPE>,
                   TYPE>,
     BASE,
     TYPE>
operator * (const MatrixExpr<ITER1, BASE, TYPE> &lhs,
            const MatrixExpr<ITER2, BASE, TYPE> &rhs)
    noexcept  {

    typedef MatBinExprOpt<MatrixExpr<ITER1,
                                     BASE,
                                     TYPE>,
                          MatrixExpr<ITER2,
                                     BASE,
                                     TYPE>,
                          MatMultiplies<TYPE>,
                          TYPE>  expr_type;

    return (MatrixExpr<expr_type, BASE, TYPE>
               (expr_type (lhs,
                           rhs,
                           lhs.lhs_row_size (),
                           rhs.rhs_row_size (),
                           rhs.rhs_col_size ()),
                static_cast<unsigned char>(MatrixOptBase::_multiply_)));
}

// ----------------------------------------------------------------------------

// template<template<class T> class BASE,
//          class ITER1,
//          class ITER2,
//          class TYPE>
// inline
// MatrixExpr
//     <MatBinExprOpt
//          <MatrixExpr<ITER1, BASE, TYPE>,
//           typename Matrix<BASE, TYPE>::col_const_iterator,
//           MatMultiplies<TYPE>,
//           TYPE>,
//      BASE,
//      TYPE>
// operator / (const MatrixExpr<ITER1, BASE, TYPE> &lhs,
//             const MatrixExpr<ITER2, BASE, TYPE> &rhs)
//     noexcept  {
// 
//     typedef Matrix<BASE, TYPE>   MatrixType;
// 
//     const   MatrixType  rhs_tmp = rhs;
// 
//     return (lhs * ! rhs_tmp);
// }

// ----------------------------------------------------------------------------

//
// Matrix-based global arithmetic self-operators
//

// ----------------------------------------------------------------------------

// Matrix += Matrix
//
template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
operator += (Matrix<BASE, TYPE> &lhs,
             const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    typename MatrixType::col_const_iterator rhs_citer = rhs.col_begin ();

    for (typename MatrixType::col_iterator lhs_iter = lhs.col_begin ();
         lhs_iter != lhs.col_end (); ++lhs_iter, ++rhs_citer)
        *lhs_iter += *rhs_citer;

    return (lhs);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
operator -= (Matrix<BASE, TYPE> &lhs,
             const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    typename MatrixType::col_const_iterator rhs_citer = rhs.col_begin ();

    for (typename MatrixType::col_iterator lhs_iter = lhs.col_begin ();
         lhs_iter != lhs.col_end (); ++lhs_iter, ++rhs_citer)
        *lhs_iter -= *rhs_citer;

    return (lhs);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
operator *= (Matrix<BASE, TYPE> &lhs,
             const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    const   MatrixType  tmp_lhs = lhs;

    lhs.resize (lhs.rows (), rhs.columns ());

    typename MatrixType::row_const_iterator tlhs_rciter = tmp_lhs.row_begin ();
    typename MatrixType::col_const_iterator rhs_cciter = rhs.col_begin ();

    for (typename MatrixType::col_iterator lhs_citer = lhs.col_begin ();
         lhs_citer != lhs.col_end (); ++lhs_citer)  {
         if (tlhs_rciter == tmp_lhs.row_end ())  {
             tlhs_rciter = tmp_lhs.row_begin ();
             rhs_cciter += rhs.rows ();
         }

         for (typename MatrixType::size_type c = 0; c < rhs.rows (); ++c)
            *lhs_citer += *tlhs_rciter++ * *rhs_cciter++;

         rhs_cciter -= rhs.rows ();
    }

    return (lhs);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
operator /= (Matrix<BASE, TYPE> &lhs,
             const Matrix<BASE, TYPE> &rhs) noexcept  {

    return (lhs *= ! rhs);
}

// ----------------------------------------------------------------------------

// Matrix += Expression
//
template<template<class T> class BASE, class ITER, class TYPE>
inline Matrix<BASE, TYPE> &
operator += (Matrix<BASE, TYPE> &lhs,
             const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>           MatrixType;
    typedef MatrixExpr<ITER, BASE, TYPE> ExprType;

    typename ExprType::const_iterator   rhs_citer = rhs.begin ();

    for (typename MatrixType::col_iterator lhs_iter = lhs.col_begin ();
         lhs_iter != lhs.col_end (); ++lhs_iter, ++rhs_citer)
        *lhs_iter += *rhs_citer;

    return (lhs);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline Matrix<BASE, TYPE> &
operator -= (Matrix<BASE, TYPE> &lhs,
             const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>           MatrixType;
    typedef MatrixExpr<ITER, BASE, TYPE> ExprType;

    typename ExprType::const_iterator   rhs_citer = rhs.begin ();

    for (typename MatrixType::col_iterator lhs_iter = lhs.col_begin ();
         lhs_iter != lhs.col_end (); ++lhs_iter, ++rhs_citer)
        *lhs_iter -= *rhs_citer;

    return (lhs);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline Matrix<BASE, TYPE> &
operator *= (Matrix<BASE, TYPE> &lhs,
             const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    const   MatrixType  tmp_rhs = rhs;

    return (lhs *= rhs);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline Matrix<BASE, TYPE> &
operator /= (Matrix<BASE, TYPE> &lhs,
             const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    const   MatrixType  tmp_rhs = rhs;

    return (lhs *= ! rhs);
}

// ----------------------------------------------------------------------------

//
// Matrix-based global comparison operators
//

// ----------------------------------------------------------------------------

// Matrix == Matrix
//
template<template<class T> class BASE, class TYPE>
inline bool operator == (const Matrix<BASE, TYPE> &lhs,
                         const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>   MatrixType;

    if (lhs.rows () != rhs.rows () || lhs.columns () != rhs.columns ())
        return (false);

    typename MatrixType::col_const_iterator rhs_citer = rhs.col_begin ();

    for (typename MatrixType::col_const_iterator lhs_citer = lhs.col_begin ();
         lhs_citer != lhs.col_end (); ++lhs_citer, ++rhs_citer)
        if (*lhs_citer != *rhs_citer)
            return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool operator != (const Matrix<BASE, TYPE> &lhs,
                         const Matrix<BASE, TYPE> &rhs) noexcept  {

    return (! (lhs == rhs));
}

// ----------------------------------------------------------------------------

// Matrix == Expression
//
template<template<class T> class BASE, class ITER, class TYPE>
inline bool
operator == (const Matrix<BASE, TYPE> &lhs,
             const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>           MatrixType;
    typedef MatrixExpr<ITER, BASE, TYPE> ExprType;

    if (lhs.rows () != rhs.result_row_size () ||
        lhs.columns () != rhs.result_col_size ())
        return (false);

    typename ExprType::const_iterator   rhs_citer = rhs.begin ();

    for (typename MatrixType::col_const_iterator lhs_citer = lhs.col_begin ();
         lhs_citer != lhs.col_end (); ++lhs_citer, ++rhs_citer)
        if (*lhs_citer != *rhs_citer)
            return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline bool
operator != (const Matrix<BASE, TYPE> &lhs,
             const MatrixExpr<ITER, BASE, TYPE> &rhs) noexcept  {

    return (! (lhs == rhs));
}

// ----------------------------------------------------------------------------

// Expression == Matrix
//
template<template<class T> class BASE, class ITER, class TYPE>
inline bool
operator == (const MatrixExpr<ITER, BASE, TYPE> &lhs,
             const Matrix<BASE, TYPE> &rhs) noexcept  {

    typedef Matrix<BASE, TYPE>           MatrixType;
    typedef MatrixExpr<ITER, BASE, TYPE> ExprType;

    if (lhs.result_row_size () != rhs.rows () ||
        lhs.result_col_size () != rhs.columns ())
        return (false);

    typename ExprType::const_iterator   lhs_citer = lhs.begin ();

    for (typename MatrixType::col_const_iterator rhs_citer = rhs.col_begin ();
         rhs_citer != rhs.col_end (); ++rhs_citer, ++lhs_citer)
        if (*lhs_citer != *rhs_citer)
            return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class ITER, class TYPE>
inline bool
operator != (const MatrixExpr<ITER, BASE, TYPE> &lhs,
             const Matrix<BASE, TYPE> &rhs) noexcept  {

    return (! (lhs == rhs));
}

// ----------------------------------------------------------------------------

// Expression == Expression
//
template<template<class T> class BASE,
         class ITER1,
         class ITER2,
         class TYPE>
inline bool
operator == (const MatrixExpr<ITER1, BASE, TYPE> &lhs,
             const MatrixExpr<ITER2, BASE, TYPE> &rhs) noexcept  {

    typedef MatrixExpr<ITER1, BASE, TYPE> LhsExprType;
    typedef MatrixExpr<ITER2, BASE, TYPE> RhsExprType;

    if (lhs.result_row_size () != rhs.result_row_size () ||
        lhs.result_col_size () != rhs.result_col_size ())
        return (false);

    typename LhsExprType::const_iterator    lhs_citer = lhs.begin ();
    typename RhsExprType::const_iterator    rhs_citer = rhs.begin ();
    const   typename LhsExprType::size_type max_count =
        lhs.result_row_size () * lhs.result_col_size ();

    for (typename LhsExprType::size_type count = 0; count < max_count;
         ++count, ++rhs_citer, ++lhs_citer)
        if (*lhs_citer != *rhs_citer)
            return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE,
         class ITER1,
         class ITER2,
         class TYPE>
inline bool
operator != (const MatrixExpr<ITER1, BASE, TYPE> &lhs,
             const MatrixExpr<ITER2, BASE, TYPE> &rhs)
    noexcept  {

    return (! (lhs == rhs));
}

} // namespace hmma

// ----------------------------------------------------------------------------

#undef _INCLUDED_MathOperators_h
#define _INCLUDED_MathOperators_h 1
#endif  // _INCLUDED_MathOperators_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
