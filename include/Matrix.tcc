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
* Neither the name of Hossein Moein and/or the DataFrame nor the
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

#include <math.h>
#include <algorithm>    // std::max and min

#include <MathOperators.h>
#include <Matrix.h>

// ----------------------------------------------------------------------------

namespace hmma
{

template<typename T>
inline T abs__ (const T x) noexcept;

template<>
inline int abs__<int> (int x) noexcept  { return (::abs (x)); }

template<>
inline float abs__<float> (float x) noexcept  { return (::fabsf (x)); }

template<>
inline double abs__<double> (double x) noexcept  { return (::fabs (x)); }

template<>
inline long double
abs__<long double> (long double x) noexcept  { return (::fabsl (x)); }

// ----------------------------------------------------------------------------

template<typename T>
inline T sqrt__ (const T x) noexcept;

template<>
inline float sqrt__<float> (float x) noexcept  { return (::sqrtf (x)); }

template<>
inline double sqrt__<double> (double x) noexcept  { return (::sqrt (x)); }

template<>
inline long double
sqrt__<long double> (long double x) noexcept  { return (::sqrtl (x)); }

// ----------------------------------------------------------------------------

template<typename T>
inline T pow__ (T x, T y) noexcept;

template<>
inline float pow__<float> (float x, float y) noexcept  {

    return (::powf (x, y));
}

template<>
inline double pow__<double> (double x, double y) noexcept  {

    return (::pow (x, y));
}

template<>
inline long double
pow__<long double> (long double x, long double y) noexcept  {

    return (::powl (x, y));
}

// ----------------------------------------------------------------------------

template<typename T>
inline T hypot__ (T x, T y) noexcept;

template<>
inline float hypot__<float> (float x, float y) noexcept  {

#ifdef WIN32
    return (_hypotf (x, y));
#else
    return (::hypotf (x, y));
#endif // WIN32
}

template<>
inline double hypot__<double> (double x, double y) noexcept  {

#ifdef WIN32
    return (_hypot (x, y));
#else
    return (::hypot (x, y));
#endif // WIN32
}

template<>
inline long double
hypot__<long double> (long double x, long double y) noexcept  {

#ifdef WIN32
    return (_hypot (x, y));
#else
    return (::hypotl (x, y));
#endif // WIN32
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
const typename Matrix<BASE, TYPE>::value_type Matrix<BASE, TYPE>::EPSILON_ =
    pow__ (value_type(2.0), value_type(-52.0));

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::reference
Matrix<BASE, TYPE>::operator () (size_type r, size_type c) noexcept  {

    return (BaseClass::at (r, c));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::const_reference
Matrix<BASE, TYPE>::operator () (size_type r, size_type c) const noexcept  {

    return (BaseClass::at (r, c));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
Matrix<BASE, TYPE>::transpose (Matrix &that) const noexcept  {

    // Matrix      tmp = *this;
    // size_type   count = 0;

    // for (col_const_iterator cciter = col_begin ();
    //      cciter != col_end (); ++cciter, ++count)
    //     tmp (count / rows (), count % rows ()) = *cciter;
    // that.swap (tmp);

    that = ~ *this;
    return (that);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &Matrix<BASE, TYPE>::transpose () noexcept  {

    Matrix   tmp;

    transpose (tmp);
    BaseClass::swap (tmp);
    return (*this);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &Matrix<BASE, TYPE>::invert ()  {

    if (! is_square ())
        throw NotSquare ();

    Matrix   tmp (BaseClass::rows (), BaseClass::columns ());

    tmp.identity ();
    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        const size_type indx = ppivot_ (r);

        if (indx == BaseClass::_NOPOS)
            throw Singular ();

        if (indx != 0)
            for (size_type c = 0; c < BaseClass::columns (); ++c)
                std::swap (tmp (r, c), tmp (indx, c));

        const value_type    diag = BaseClass::at (r, r);

        for (size_type c = 0; c < BaseClass::columns (); ++c)  {
            BaseClass::at (r, c) /= diag;
            tmp (r, c) /= diag;
        }

        for (size_type rr = 0; rr < BaseClass::rows (); ++rr)
            if (rr != r)  {
                const value_type    off_diag = BaseClass::at (rr, r);

                for (size_type c = 0; c < BaseClass::columns (); ++c)  {
                    BaseClass::at (rr, c) -= off_diag * BaseClass::at (r, c);
                    tmp (rr, c) -= off_diag * tmp (r, c);
                }
            }
    }

    BaseClass::swap (tmp);
    return (*this);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &Matrix<BASE, TYPE>::inverse (Matrix &that) const {

    that = *this;
    return (that.invert ());
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
Matrix<BASE, TYPE>::rref (size_type &rank) noexcept  {

    rank = 0;
    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        if (r >= BaseClass::columns () || ppivot_ (r) == BaseClass::_NOPOS)
            break;

        const value_type    diag = BaseClass::at (r, r);

        for (size_type c = r; c < BaseClass::columns (); ++c)
            BaseClass::at (r, c) /= diag;

        for (size_type rr = r + 1; rr < BaseClass::rows (); ++rr)  {
            const value_type    below_diag = BaseClass::at (rr, r);

            for (size_type c = r; c < BaseClass::columns (); ++c)
                BaseClass::at (rr, c) -= below_diag * BaseClass::at (r, c);
        }
        rank += 1;
    }

    return (*this);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
Matrix<BASE, TYPE>::rref (Matrix &that, size_type &rank) const noexcept  {

    that = *this;
    return (that.rref (rank));
}

// ----------------------------------------------------------------------------

// Static
//
template<template<class T> class BASE, class TYPE>
Matrix<BASE, TYPE> &Matrix<BASE, TYPE>::identity (Matrix &that) {

    if (! that.is_square ())
        throw NotSquare ();

    for (size_type r = 0; r < that.rows (); ++r)
        for (size_type c = 0; c < that.columns (); ++c)
            that (r, c) = r == c ? value_type(1.0) : value_type(0.0);

    return (that);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &Matrix<BASE, TYPE>::identity () {

    const size_type dem = std::min (BaseClass::rows (), BaseClass::columns ());

    BaseClass::resize (dem, dem);
    for (size_type r = 0; r < BaseClass::rows (); ++r)
        BaseClass::at (r, r) = value_type(1.0);

    return (*this);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::size_type
Matrix<BASE, TYPE>::rank () const noexcept  {

    Matrix      tmp (*this);
    size_type   r;

    tmp.rref (r);
    return (r);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::determinant () const {

    if (! is_square ())
        throw NotSquare ();

    Matrix       tmp (*this);
    value_type   result (1.0);

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        const size_type indx = tmp.ppivot_ (r);

        if (indx == BaseClass::_NOPOS)
            return (value_type(0.0));

        if (indx != 0)
            result = -result;

        const value_type    diag = tmp (r, r);

        result *= diag;
        for (size_type rr = r + 1; rr < BaseClass::rows (); ++rr)  {
            const value_type    piv = tmp (rr, r) / diag;

            for (size_type c = r + 1; c < BaseClass::columns (); ++c)
                tmp (rr, c) -= piv * tmp (r, c);
        }
    }

    return (result);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
Matrix<BASE, TYPE>::
get_minor (Matrix &mmatrix, size_type drow, size_type dcol) const noexcept  {

    mmatrix.resize (BaseClass::rows () - 1, BaseClass::columns () - 1);

    size_type   mrow = 0;

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        if (r != drow)  {
            size_type   mcol = 0;

            for (size_type c = 0; c < BaseClass::columns (); ++c)
                if (c != dcol)
                    mmatrix (mrow, mcol++) = BaseClass::at (r, c);

            mrow += 1;  // Increase minor row-count
        }

    return (mmatrix);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::cofactor (size_type row, size_type column) const {

    if (! is_square ())
        throw NotSquare ();

    Matrix   tmp;

    get_minor (tmp, row, column);
    return ((row + column) % 2 ? -tmp.determinant () : tmp.determinant ());
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &Matrix<BASE, TYPE>::adjoint (Matrix &that) const {

    if (! is_square ())
        throw NotSquare ();

    that.resize (BaseClass::rows (), BaseClass::columns ());
    for (size_type r = 0; r < BaseClass::rows (); ++r)
        for (size_type c = 0; c < BaseClass::columns (); ++c)
            that (c, r) = cofactor (r, c);

    return (that);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE>
Matrix<BASE, TYPE>::covariance (bool is_unbiased) const {

    const value_type    denom =
        is_unbiased ? BaseClass::rows () - 1 : BaseClass::rows ();

    if (denom <= value_type(0.0))
        throw NotSolvable ();

    Matrix                              sol (BaseClass::columns (),
                                             BaseClass::columns ());
    typename BaseClass::row_iterator    sol_iter = sol.row_begin ();

   // I don't know how I can take advantage of the fact this is a
   // symmetric matrix by definition
   //
    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        value_type  col_mean (0.0);
        size_type   counter = 0;

        for (typename BaseClass::col_const_iterator cciter =
                 BaseClass::col_begin () + c * BaseClass::rows ();
             counter != BaseClass::rows (); ++cciter, ++counter)
            col_mean += *cciter;

        col_mean /= value_type(BaseClass::rows ());

        for (size_type cc = 0; cc < BaseClass::columns (); ++cc)  {
            value_type  var (0.0);

            for (size_type r = 0; r < BaseClass::rows (); ++r)
                var += (BaseClass::at (r, c) - col_mean) *
                       (BaseClass::at (r, cc) - col_mean);

            *sol_iter++ = var / denom;
        }
    }

    return (sol);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> Matrix<BASE, TYPE>::correlation () const {

    const Matrix    tmp = covariance ();
    Matrix          sol (tmp.rows (), tmp.columns ());

    for (size_type c = 0; c < sol.columns (); ++c)
        for (size_type r = c; r < sol.rows (); ++r)
            sol (r, c) = tmp(r, c) / (sqrt__(tmp (c, c)) * sqrt__(tmp (r, r)));

   // Here I can; Fill up the other half of the, by-definition,
   // symmetric matrix
   //
    for (size_type c = 0; c < sol.columns (); ++c)
        for (size_type r = c; r < sol.rows (); ++r)
            sol (c, r) = sol (r, c);

    return (sol);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE>
Matrix<BASE, TYPE>::solve_se (const Matrix &rhs) const {

    if (! is_square () || BaseClass::columns () != rhs.rows ())
        throw NotSolvable ();

    Matrix  tmp (BaseClass::rows (), BaseClass::columns () + rhs.columns ());

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        for (size_type c = 0; c < BaseClass::columns (); ++c)
            tmp (r, c) = BaseClass::at (r, c);
        for (size_type c = 0; c < rhs.columns (); ++c)
            tmp (r, BaseClass::columns () + c) = rhs (r, c);
    }

    size_type   rank;

    tmp.rref (rank);

    if (rank != BaseClass::rows ())
        throw Singular ();

    Matrix  sol (rhs.rows (), rhs.columns ());

    for (size_type rhsc = 0; rhsc < rhs.columns (); ++rhsc)
        for (int r = static_cast<int>(BaseClass::rows ()) - 1; r >= 0; --r)  {
            sol (r, rhsc) = tmp (r, BaseClass::columns () + rhsc);

            for (size_type c = r + 1; c < BaseClass::columns (); ++c)
                sol (r, rhsc) -= tmp (r, c) * sol (c, rhsc);
        }

    return (sol);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::norm () const noexcept  {

    value_type  result (0.0);

    for (auto citer = BaseClass::col_begin();
         citer != BaseClass::col_end (); ++citer)
        result += *citer * *citer;

    return (sqrt__ (result));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::max_norm () const noexcept  {

    value_type  result (0.0);

    for (auto citer = BaseClass::col_begin();
         citer != BaseClass::col_end (); ++citer)
        if (abs__ (*citer) > result)
            result = abs__ (*citer);

    return (result);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::col_norm () const noexcept  {

    value_type  result (0.0);
    value_type  final_result (0.0);
    size_type   r_counter = 0;

    for (auto citer = BaseClass::col_begin();
         citer != BaseClass::col_end (); ++citer, ++r_counter)  {
        if (r_counter == BaseClass::rows ())  {
            r_counter = 0;
            if (result > final_result)
                final_result = result;
            result = value_type(0.0);
        }
        result += abs__ (*citer);
    }
    if (result > final_result)
        final_result = result;

    return (final_result);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::row_norm () const noexcept  {

    value_type  final_result (0.0);

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        value_type  result (0.0);

        for (size_type c = 0; c < BaseClass::columns (); ++c)
            result += abs__ (BaseClass::at (r, c));

        if (result > final_result)
            final_result = result;
    }

    return (final_result);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::condition () const {

    Matrix  inv;

    inverse (inv);
    return (norm () * inv.norm ());
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::value_type
Matrix<BASE, TYPE>::trace () const {

    if (! is_square ())
        throw NotSquare ();

    value_type  v (0.0);

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        v += at (r, r);

    return (v);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
template<class MAT>
inline void Matrix<BASE, TYPE>::
eigen_space (MAT &eigenvalues, MAT &eigenvectors, bool sort_values) const {

    if (! is_square () || BaseClass::columns () < 2)
        throw NotSolvable ();

    MAT tmp_evecs (BaseClass::rows (), BaseClass::columns ());
    MAT tmp_evals (1, BaseClass::columns ());
    MAT imagi (1, BaseClass::columns ()); // Imaginary part

    if (is_symmetric ())  {
        for (size_type r = 0; r < BaseClass::rows (); ++r)
            for (size_type c = 0; c < BaseClass::columns (); ++c)
                tmp_evecs (r, c) = BaseClass::at (r, c);

        for (size_type c = 0; c < BaseClass::columns (); ++c)
            tmp_evals (0, c) = BaseClass::at (BaseClass::rows () - 1, c);

        tridiagonalize_ (tmp_evecs, tmp_evals, imagi);
        diagonalize_ (tmp_evecs, tmp_evals, imagi);
    }
    else  {
        MAT hess_form (BaseClass::rows (), BaseClass::columns ());

        for (size_type r = 0; r < BaseClass::rows (); ++r)
            for (size_type c = 0; c < BaseClass::columns (); ++c)
                hess_form (r, c) = BaseClass::at (r, c);

        red_to_hessenberg_ (tmp_evecs, hess_form);
        hessenberg_to_schur_ (tmp_evecs, tmp_evals, imagi, hess_form);
    }

    if (sort_values)  {
        for (size_type c = 0; c < BaseClass::columns () - 1; ++c)  {
            size_type   tmp_c = c;
            value_type  p = tmp_evals (0, c);

            for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)
                if (tmp_evals (0, cc) < p)  {
                    tmp_c = cc;
                    p = tmp_evals (0, cc);
                }

            if (tmp_c != c)  {
                tmp_evals (0, tmp_c) = tmp_evals (0, c);
                tmp_evals (0, c) = p;
                for (size_type r = 0; r < BaseClass::rows (); ++r)  {
                    p = tmp_evecs (r, c);
                    tmp_evecs (r, c) = tmp_evecs (r, tmp_c);
                    tmp_evecs (r, tmp_c) = p;
                }
            }
        }
    }

    eigenvalues.swap (tmp_evals);
    eigenvectors.swap (tmp_evecs);
    return;
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
Matrix<BASE, TYPE>::power (Matrix &result, value_type n, bool is_diag) const {

    result = *this;
    result.power (n, is_diag);

    return (result);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline Matrix<BASE, TYPE> &
Matrix<BASE, TYPE>::power (value_type n, bool is_diag) {

    if (is_diag)
        diagonal_power_ (n);
    else  {
        Matrix  eigenvalues;
        Matrix  eigenvectors;

        eigen_space (eigenvalues, eigenvectors, true);

        Matrix   diag_evals (BaseClass::rows (), BaseClass::columns ());

        for (size_type c = 0; c < eigenvalues.columns (); ++c)
            diag_evals (c, c) = pow__ (eigenvalues (0, c), n);

        Matrix  inv;

        *this = eigenvectors * diag_evals * eigenvectors.inverse (inv);
    }

    return (*this);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline void Matrix<BASE, TYPE>::
svd (Matrix &U, Matrix &S, Matrix &V, bool full_size_S) const {

    const size_type min_dem =
        std::min (BaseClass::rows (), BaseClass::columns ());

    if (min_dem < 3)
        throw NotSolvable ();

    Matrix                  self_tmp = *this;
    Matrix                  u_tmp (BaseClass::rows (), min_dem);
    std::vector<value_type> s_tmp (std::min (BaseClass::rows () + 1,
                                             BaseClass::columns ()));
    Matrix                  v_tmp (BaseClass::columns(), BaseClass::columns());
    Matrix                  imagi (1, BaseClass::columns ()); // Imaginary part
    std::vector<value_type> sandbox (BaseClass::rows ());
    const size_type         min_col_cnt =
        std::min (BaseClass::rows () - 1, BaseClass::columns ());
    const size_type         max_row_cnt =
        std::max(0U, std::min(BaseClass::columns () - 2, BaseClass::rows ()));

   // Reduce A to bidiagonal form, storing the diagonal elements
   // in s and the super-diagonal elements in e.
   //
    for (size_type c = 0; c < std::max (min_col_cnt, max_row_cnt); ++c)  {
        if (c < min_col_cnt)  {
          // Compute the transformation for the k-th column and
          // place the k-th diagonal in S (0, c).
          // Compute 2-norm of k-th column without under / overflow.
          //
            s_tmp [c] = value_type(0.0);
            for (size_type r = c; r < BaseClass::rows (); ++r)
                s_tmp [c] = hypot__ (s_tmp [c], self_tmp (r, c));

            if (s_tmp [c] != value_type(0.0))  {
                if (self_tmp (c, c) < value_type(0.0))
                    s_tmp [c] = -s_tmp [c];

                for (size_type r = c; r < BaseClass::rows (); ++r)
                    self_tmp (r, c) /= s_tmp [c];

                self_tmp (c, c) += value_type(1.0);
            }
            s_tmp [c] = -s_tmp [c];
        }
        for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)  {
            if (c < min_col_cnt && s_tmp [c] != value_type(0.0))  {
               // Apply the transformation.
               //
                value_type  t (0.0);

                for (size_type r = c; r < BaseClass::rows (); ++r)
                    t += self_tmp (r, c) * self_tmp (r, cc);

                t /= -self_tmp (c, c);
                for (size_type r = c; r < BaseClass::rows (); ++r)
                    self_tmp (r, cc) += t * self_tmp (r, c);

            }

           // Place the k-th row of A into e for the
           // subsequent calculation of the row transformation.
           //
            imagi (0, cc) = self_tmp (c, cc);
        }
        if (c < min_col_cnt)
           // Place the transformation in U for subsequent back
           // multiplication.
           //
            for (size_type r = c; r < BaseClass::rows (); ++r)
                u_tmp (r, c) = self_tmp (r, c);

        if (c < max_row_cnt)  {

           // Compute the k-th row transformation and place the
           // k-th super-diagonal in imagi (0, c).
           // Compute 2-norm without under / overflow.
           //
            imagi (0, c) = value_type(0.0);
            for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)
                imagi (0, c) = hypot__ (imagi (0, c), imagi (0, cc));

            if (imagi (0, c) != value_type(0.0))  {
                if (imagi (0, c + 1) < value_type(0.0))
                    imagi (0, c) = -imagi (0, c);

                for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)
                    imagi (0, cc) /= imagi (0, c);

                imagi (0, c + 1) += value_type(1.0);
            }
            imagi (0, c) = -imagi (0, c);

            if (c + 1 < BaseClass::rows() && imagi(0, c) != value_type(0.0))  {

               // Apply the transformation.
               //
                for (size_type r = c + 1; r < BaseClass::rows (); ++r)
                    sandbox [r] = value_type(0.0);

                for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)
                    for (size_type r = c + 1; r < BaseClass::rows (); ++r)
                        sandbox [r] += imagi (0, cc) * self_tmp (r, cc);

                for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)  {
                    const value_type    t = -imagi (0, cc) / imagi (0, c + 1);

                    for (size_type r = c + 1; r < BaseClass::rows (); ++r)
                        self_tmp (r, cc) += t * sandbox [r];
                }
            }

           // Place the transformation in V for subsequent
           // back multiplication.
           //
            for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)
                v_tmp (cc, c) = imagi (0, cc);
        }
    }

   // Set up the final bidiagonal matrix of order p.
   //
    int p = static_cast<int>(std::min (BaseClass::columns (),
                                       BaseClass::rows () + 1));

    if (min_col_cnt < BaseClass::columns ())
        s_tmp [min_col_cnt] = self_tmp (min_col_cnt, min_col_cnt);

    if (BaseClass::rows () < p)
        s_tmp [p - 1] = value_type(0.0);

    if (max_row_cnt + 1 < p)
        imagi (0, max_row_cnt) = self_tmp (max_row_cnt, p - 1);

    imagi (0, p - 1) = value_type(0.0);

    for (size_type c = min_col_cnt; c < min_dem; ++c)  {
        for (size_type r = 0; r < BaseClass::rows (); ++r)
            u_tmp (r, c) = value_type(0.0);
        u_tmp (c, c) = value_type(1.0);
    }

    for (int c = min_col_cnt - 1; c >= 0; --c)  {
        if (s_tmp [c] != value_type(0.0))  {
            for (size_type cc = c + 1; cc < min_dem; ++cc)  {
                value_type  t (0.0);

                for (size_type r = c; r < BaseClass::rows (); ++r)
                    t += u_tmp (r, c) * u_tmp (r, cc);

                t /= -u_tmp (c, c);
                for (size_type r = c; r < BaseClass::rows (); ++r)
                    u_tmp (r, cc) += t * u_tmp (r, c);
            }
            for (size_type r = c; r < BaseClass::rows (); ++r )
                u_tmp (r, c) = -u_tmp (r, c);

            u_tmp (c, c) += value_type(1.0);
            for (int r = 0; r < c - 1; ++r)
                u_tmp (r, c) = value_type(0.0);
        }
        else  {
            for (size_type r = 0; r < BaseClass::rows (); ++r)
                u_tmp (r, c) = value_type(0.0);
            u_tmp (c, c) = value_type(1.0);
        }
    }

    for (int c = BaseClass::columns () - 1; c >= 0; --c)  {
        if ((c < max_row_cnt) && (imagi (0, c) != value_type(0.0)))
            for (size_type cc = c + 1; cc < min_dem; ++cc)  {
                value_type  t (0.0);

                for (size_type r = c + 1; r < BaseClass::columns (); ++r)
                    t += v_tmp (r, c) * v_tmp (r, cc);

                t /= -v_tmp (c + 1, c);
                for (size_type r = c + 1; r < BaseClass::columns (); ++r)
                    v_tmp (r, cc) += t * v_tmp (r, c);
            }

        for (size_type r = 0; r < BaseClass::columns (); ++r)
            v_tmp (r, c) = value_type(0.0);
        v_tmp (c, c) = value_type(1.0);
    }

    const size_type pp = p - 1;

   // Main iteration loop for the singular values.
   //
    while (p > value_type(0.0))  {
        int         c;
        size_type   kase;

       // Here is where a test for too many iterations would go.
       //
       // This section of the routine inspects for
       // negligible elements in the s and imagi arrays.  On
       // completion the variables kase and c are set as follows.
       //
       // case == 1 --> if s (p) and imagi (0, c - 1) are negligible and k < p
       // case == 2 --> if s (c) is negligible and c < p
       // case == 3 --> if imagi (0, c - 1) is negligible, c < p, and
       //               s (c), ..., s (p) are not negligible (qr step).
       // case == 4 --> if e (p - 1) is negligible (convergence).
       //
        for (c = p - 2; c >= -1; --c)  {
            if (c == -1)
                break;

            if (abs__ (imagi (0, c)) <=
                    EPSILON_ * (abs__ (s_tmp [c]) + abs__ (s_tmp [c + 1])))  {
                imagi (0, c) = value_type(0.0);
                break;
            }
        }
        if (c == p - 2)
            kase = 4;

        else  {
            int ks;

            for (ks = p - 1; ks >= c; --ks)  {
                if (ks == c)
                    break;

                const value_type    t =
                    ks != p ? abs__ (imagi (0, ks)) : value_type(0.0) +
                    ks != c + value_type(1.0)
                        ? abs__ (imagi (0, ks - 1))
                        : value_type(0.0);

                if (abs__ (s_tmp [ks]) <= EPSILON_ * t)  {
                    s_tmp [ks] = value_type(0.0);
                    break;
                }
            }
            if (ks == c)
                kase = 3;
            else if (ks == p - 1)
                kase = 1;
            else  {
                kase = 2;
                c = ks;
            }
        }
        c += 1;

       // Perform the task indicated by kase.
       //
        switch (kase)  {
           // Deflate negligible s (p).
           //
            case 1:
            {
                value_type  f = imagi (0, p - 2);

                imagi (0, p - 2) = value_type(0.0);
                for (int cc = p - 2; cc >= c; --cc)  {
                    value_type  t = hypot__ (s_tmp [cc], f);
                    value_type  cs = s_tmp [cc] / t;
                    value_type  sn = f / t;

                    s_tmp [cc] = t;
                    if (cc != c)  {
                        f = -sn * imagi (0, cc - 1);
                        imagi (0, cc - 1) *= cs;
                    }

                    for (size_type r = 0; r < BaseClass::columns (); ++r)  {
                        t = cs * v_tmp (r, cc) + sn * v_tmp (r, p - 1);
                        v_tmp (r, p - 1) =
                            -sn * v_tmp (r, cc) + cs * v_tmp (r, p - 1);
                        v_tmp (r, cc) = t;
                    }
                }
            }
            break;

           // Split at negligible s (c).
           //
            case 2:
            {
                value_type  f = imagi (0, c - 1);

                imagi (0, c - 1) = value_type(0.0);
                for (int cc = c; cc < p; ++cc)  {
                    value_type  t = hypot__ (s_tmp [cc], f);
                    value_type  cs = s_tmp [cc] / t;
                    value_type  sn = f / t;

                    s_tmp [cc] = t;
                    f = -sn * imagi (0, cc);
                    imagi (0, cc) *= cs;

                    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
                        t = cs * u_tmp (r, cc) + sn * u_tmp (r, c - 1);
                        u_tmp (r, c - 1) =
                            -sn * u_tmp (r, cc) + cs * u_tmp (r, c - 1);
                        u_tmp (r, cc) = t;
                    }
                }
            }
            break;

           // Perform one qr step.
           //
            case 3:
            {

               // Calculate the shift.
               //
                const value_type    scale =
                    std::max (
                        std::max (
                            std::max (
                                std::max (abs__ (s_tmp [p - 1]),
                                          abs__ (s_tmp [p - 2])),
                                abs__ (imagi (0, p - 2))),
                            abs__ (s_tmp [c])),
                        abs__ (imagi (0, c)));
                const value_type    sp = s_tmp [p - 1] / scale;
                const value_type    spm1 = s_tmp [p - 2] / scale;
                const value_type    epm1 = imagi (0, p - 2) / scale;
                const value_type    sk = s_tmp [c] / scale;
                const value_type    ek = imagi (0, c) / scale;
                const value_type    b =
                    ((spm1 + sp) * (spm1 - sp) + epm1 * epm1) /
                    value_type(2.0);
                const value_type    dd = (sp * epm1) * (sp * epm1);
                value_type          shift (0.0);

                if (b != value_type(0.0) || dd != value_type(0.0))  {
                    shift = b < value_type(0.0)
                                ? -sqrt__ (b * b + dd)
                                :  sqrt__ (b * b + dd);
                    shift = dd / (b + shift);
                }

                value_type  f = (sk + sp) * (sk - sp) + shift;
                value_type  g = sk * ek;

               // Chase zeros.
               //
                for (int cc = c; cc < p - 1; ++cc)  {
                    value_type  t = hypot__ (f, g);
                    value_type  cs = f / t;
                    value_type  sn = g / t;

                    if (cc != c)
                        imagi (0, cc - 1) = t;

                    f = cs * s_tmp [cc] + sn * imagi (0, cc);
                    imagi (0, cc) = cs * imagi (0, cc) - sn * s_tmp [cc];
                    g = sn * s_tmp [cc + 1];
                    s_tmp [cc + 1] *= cs;

                    for (size_type r = 0; r < BaseClass::columns (); ++r)  {
                        t = cs * v_tmp (r, cc) + sn * v_tmp (r, cc + 1);
                        v_tmp (r, cc + 1) =
                            -sn * v_tmp (r, cc) + cs * v_tmp (r, cc + 1);
                        v_tmp (r, cc) = t;
                    }

                    t = hypot__ (f, g);
                    cs = f / t;
                    sn = g / t;
                    s_tmp [cc] = t;
                    f = cs * imagi (0, cc) + sn * s_tmp [cc + 1];
                    s_tmp [cc + 1] = -sn * imagi (0, cc) + cs * s_tmp [cc + 1];
                    g = sn * imagi (0, cc + 1);
                    imagi (0, cc + 1) = cs * imagi (0, cc + 1);

                    if (cc < BaseClass::rows () - 1)
                        for (size_type r = 0; r < BaseClass::rows (); ++r)  {
                            t = cs * u_tmp (r, cc) + sn * u_tmp (r, cc + 1);
                            u_tmp (r, cc + 1) =
                                -sn * u_tmp (r, cc) + cs * u_tmp (r, cc + 1);
                            u_tmp (r, cc) = t;
                        }
                }

                imagi (0, p - 2) = f;
            }
            break;

           // Convergence.
           //
            case 4:
            {
               // Make the singular values positive.
               //
                if (s_tmp [c] <= value_type(0.0))  {
                    s_tmp [c] = s_tmp [c] < value_type(0.0)
                                    ? -s_tmp [c]
                                    : value_type(0.0);

                    for (size_type r = 0; r <= pp; ++r)
                        v_tmp (r, c) = -v_tmp (r, c);
                }

               // Order the singular values.
               //
                while (c < pp)  {
                    if (s_tmp [c] >= s_tmp [c + 1])
                        break;

                    value_type  t = s_tmp [c];

                    s_tmp [c] = s_tmp [c + 1];
                    s_tmp [c + 1] = t;

                    if (c < BaseClass::columns () - 1)
                        for (size_type r = 0; r < BaseClass::columns (); ++r) {
                            t = v_tmp (r, c + 1);
                            v_tmp (r, c + 1) = v_tmp (r, c);
                            v_tmp (r, c) = t;
                        }

                    if (c < BaseClass::rows () - 1)
                        for (size_type r = 0; r < BaseClass::rows (); ++r)  {
                            t = u_tmp (r, c + 1);
                            u_tmp (r, c + 1) = u_tmp (r, c);
                            u_tmp (r, c) = t;
                        }

                    c += 1;
                }

                p -= 1;
            }
            break;
        }
    }

    U.swap (u_tmp);

    S.resize (s_tmp.size (), full_size_S ? s_tmp.size () : 1);

    size_type   row_count = 0;

    for (auto citer = s_tmp.begin(); citer != s_tmp.end(); ++citer, ++row_count)
        S (row_count, full_size_S ? row_count : 0) = *citer;

    V.swap (v_tmp);

    return;
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline void Matrix<BASE, TYPE>::qrd (Matrix &Q, Matrix &R) const noexcept  {

    std::vector<value_type> r_diag (BaseClass::columns ());
    Matrix                  self_tmp = *this;

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        value_type  nrm (0.0);

       // Compute 2-norm of k-th column without under/overflow.
       //
        for (size_type r = c; r < BaseClass::rows (); ++r)
            nrm = hypot__ (nrm, self_tmp (r, c));

        if (nrm != value_type(0.0))  {

           // Form k-th Householder vector.
           //
            if (self_tmp (c, c) < value_type(0.0))
                nrm = -nrm;

            for (size_type r = c; r < BaseClass::rows (); ++r)
                self_tmp (r, c) /= nrm;

            self_tmp (c, c) += value_type(1.0);

           // Apply transformation to remaining columns.
           //
            for (size_type j = c + 1; j < BaseClass::columns (); ++j)  {
                value_type  s (0.0);

                for (size_type r = c; r < BaseClass::rows (); ++r)
                    s += self_tmp (r, c) * self_tmp (r, j);

                s /= -self_tmp (c, c);
                for (size_type r = c; r < BaseClass::rows (); ++r)
                    self_tmp (r, j) += s * self_tmp (r, c);
            }
        }

        r_diag [c] = -nrm;
    }

    Matrix   q_tmp (BaseClass::rows (), BaseClass::columns ());

    for (int c = static_cast<int>(BaseClass::columns ()) - 1; c >= 0; --c)  {
        q_tmp (c, c) = value_type(1.0);

        for (size_type cc = c; cc < BaseClass::columns (); ++cc)
            if (self_tmp (c, c) != value_type(0.0))  {
                value_type  s (0.0);

                for (size_type r = c; r < BaseClass::rows (); ++r)
                    s += self_tmp (r, c) * q_tmp (r, cc);

                s /= -self_tmp (c, c);
                for (size_type r = c; r < BaseClass::rows (); ++r)
                    q_tmp (r, cc) += s * self_tmp (r, c);
            }
    }

    Matrix   r_tmp (BaseClass::columns (), BaseClass::columns ());

    for (size_type c = 0; c < BaseClass::columns (); ++c)
        for (size_type r = 0; r < r_tmp.rows (); ++r)
            if (c < r)
                r_tmp (c, r) = self_tmp (c, r);
            else if (c == r)
                r_tmp (c, r) = r_diag [c];

    Q.swap (q_tmp);
    R.swap (r_tmp);

    return;
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline void Matrix<BASE, TYPE>::lud (Matrix &L, Matrix &U) const {

    if (! is_square ())
        throw NotSquare ();

    Matrix  self_tmp = *this;

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
       // Find pivot.
       //
        size_type   p = c;

        for (size_type r = c + 1; r < BaseClass::rows (); ++r)
            if (abs__ (self_tmp (r, c)) > abs__ (self_tmp (p, c)))
                p = r;

       // Exchange if necessary.
       //
        if (p != c)
            for (size_type cc = 0; cc < BaseClass::columns (); ++cc)
                std::swap (self_tmp (p, cc), self_tmp (c, cc));

       // Compute multipliers and eliminate c-th column.
       //
        if (self_tmp (c, c) != value_type(0.0))
            for (size_type r = c + 1; r < BaseClass::rows (); ++r)  {
                self_tmp (r, c) /= self_tmp (c, c);

                for (size_type cc = c + 1; cc < BaseClass::columns (); ++cc)
                    self_tmp (r, cc) -= self_tmp (r, c) * self_tmp (c, cc);
            }
    }

    Matrix  l_tmp (BaseClass::rows (), BaseClass::columns ());

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        for (size_type c = 0; c < BaseClass::columns (); ++c)
            if (r > c)
                l_tmp (r, c) = self_tmp (r, c);
            else if (r == c)
                l_tmp (r, c) = value_type(1.0);

    Matrix  u_tmp (BaseClass::columns (), BaseClass::columns ());

    for (size_type c = 0; c < BaseClass::columns (); ++c)
        for (size_type r = 0; r < u_tmp.rows (); ++r)
            if (c <= r)
                u_tmp (c,  r) = self_tmp (c, r);

    L.swap (l_tmp);
    U.swap (u_tmp);

    return;
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
template<class MAT>
inline void Matrix<BASE, TYPE>::chod (MAT &R, bool right) const {

    if (! is_square ())
        throw NotSolvable ();

    MAT r_tmp (BaseClass::rows (), BaseClass::columns ());

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        value_type  d (0.0);

        for (size_type r = 0; r < c; ++r)  {
            value_type  s =  right ? BaseClass::at (r, c) : value_type(0.0);

            for (size_type rr = 0; rr < r; ++rr)
                if (right)
                    s -= r_tmp (rr, r) * r_tmp (rr, c);
                else
                    s += r_tmp (r, rr) * r_tmp (c, rr);

            if (right)
                r_tmp (r, c) = s /= r_tmp (r, r);
            else
                r_tmp (c, r) = s = (BaseClass::at (c, r) - s) / r_tmp (r, r);
            d += s * s;

            if (BaseClass::at (r, c) != BaseClass::at (c, r)) // Not symmetric
                throw NotSolvable ();
        }

        d = BaseClass::at (c, c) - d;

        if (d <= value_type(0.0)) // Not positive definite
            throw NotSolvable ();

        r_tmp (c, c) = sqrt__ (std::max (d, value_type(0.0)));
    }

    R.swap (r_tmp);
    return;
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_square () const noexcept  {

    return (BaseClass::rows () == BaseClass::columns ());
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_singular () const noexcept  {

    return (is_square () && determinant () == value_type(0.0));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_diagonal () const noexcept  {

    if (! is_square ())
        return (false);

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        for (size_type c = 0; c < BaseClass::columns (); ++c)
            if (r != c && BaseClass::at (r, c) != value_type(0.0))
                return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_scalar () const noexcept  {

    if (! is_diagonal ())
        return (false);

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        if (BaseClass::at (r, r) != BaseClass::at (0, 0))
            return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_identity () const noexcept  {

    return (is_scalar () && BaseClass::at (0, 0) == value_type(1.0));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_null () const noexcept  {

    for (auto citer = BaseClass::col_begin();
         citer != BaseClass::col_end (); ++citer)
        if (*citer != value_type(0.0))
            return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_symmetric () const noexcept  {

    if (BaseClass::_is_symmetric_matrix ())
        return (true);
    if (! is_square ())
        return (false);

    for (size_type r = 1; r < BaseClass::rows (); ++r)
        for (size_type c = 0; c < r; ++c)
            if (BaseClass::at (r, c) != BaseClass::at (c, r))
                return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_skew_symmetric () const noexcept  {

    if (! is_square ())
        return (false);

    for (size_type r = 1; r < BaseClass::rows (); ++r)
        for (size_type c = 0; c < r; ++c)
            if (BaseClass::at (r, c) != - BaseClass::at (c, r))
                return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_upper_triangular () const noexcept  {

    if (! is_square ())
        return (false);

    for (size_type r = 1; r < BaseClass::rows (); ++r)
        for (size_type c = 0; c < r; ++c)
            if (BaseClass::at (r, c) != value_type(0.0))
                return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_lower_triangular () const noexcept  {

    if (! is_square ())
        return (false);

    for (size_type c = 1; c < BaseClass::columns (); ++c)
        for (size_type r = 0; r < c; ++r)
            if (BaseClass::at (r, c) != value_type(0.0))
                return (false);

    return (true);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_normal () const noexcept  {

    if (! is_square ())
        return (false);

    return (*this * ~ *this == ~ *this * *this);
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_orthogonal () const  {

    if (! is_square ())
        return (false);

    const Matrix    tmp = *this * ~ *this;

    return (tmp.is_identity ());
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline bool Matrix<BASE, TYPE>::is_special_orthogonal () const  {

    return (is_orthogonal () && determinant () == value_type(1.0));
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline void Matrix<BASE, TYPE>::diagonal_power_ (value_type n)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)
        BaseClass::at (c, c) = pow__ (BaseClass::at (c, c), n);

    return;
}

// ----------------------------------------------------------------------------

template<template<class T> class BASE, class TYPE>
inline typename Matrix<BASE, TYPE>::size_type
Matrix<BASE, TYPE>::ppivot_ (size_type the_row) noexcept  {

    size_type   max_row = the_row;
    value_type  max_value (-1.0);

    for (size_type r = the_row; r < BaseClass::rows (); ++r)  {
        const value_type    tmp = abs__ (BaseClass::at (r, the_row));

        if (tmp > max_value && tmp != value_type(0.0))  {
            max_value = tmp;
            max_row = r;
        }
    }

    if (BaseClass::at (max_row, the_row) == value_type(0.0))
        return (BaseClass::_NOPOS);

    if (max_row != the_row)  {
        for (size_type c = 0; c < BaseClass::columns (); ++c)
            std::swap (BaseClass::at (the_row, c), BaseClass::at (max_row, c));

        return (max_row);
    }

    return (0);
}

// ----------------------------------------------------------------------------

// Static
//
template<template<class T> class BASE, class TYPE>
template<class MAT>
inline void Matrix<BASE, TYPE>::
tridiagonalize_ (MAT &e_vecs, MAT &e_vals, MAT &imagi) noexcept  {

    for (size_type r = e_vecs.rows () - 1; r > 0; --r)  {
       // Scale to avoid under/overflow.
       //
        value_type  scale (0.0);

        for (size_type c = 0; c < r; ++c)
            scale += abs__ (e_vals (0, c));

        value_type  h (0.0);

        if (scale == value_type(0.0))  {
            imagi (0, r) = e_vals (0, r - 1);
            for (size_type c = 0; c < r; ++c)  {
                e_vals (0, c) = e_vecs (r - 1, c);
                e_vecs (r, c) = value_type(0.0);
                e_vecs (c, r) = value_type(0.0);
            }
        }
        else  {
           // Generate Householder vector.
           //
            for (size_type c = 0; c < r; ++c)  {
                e_vals (0, c) /= scale;
                h += e_vals (0, c) * e_vals (0, c);
            }

            const value_type    &f = e_vals (0, r - 1);
            value_type          g =
                f > value_type(0.0) ? -sqrt__ (h) : sqrt__ (h);

            imagi (0, r) = scale * g;
            h -= f * g;
            e_vals (0, r - 1) = f - g;

            for (size_type c = 0; c < r; ++c)
                imagi (0, c) = value_type(0.0);

           // Apply similarity transformation to remaining columns.
           //
            for (size_type c = 0; c < r; ++c)  {
                const value_type    &ff = e_vals (0, c);

                e_vecs (c, r) = ff;
                g = imagi (0, c) + e_vecs (c, c) * ff;

                for (size_type cc = c + 1; cc <= r - 1; ++cc)  {
                    g += e_vecs (cc, c) * e_vals (0, cc);
                    imagi (0, cc) += e_vecs (cc, c) * ff;
                }
                imagi (0, c) = g;
            }

            value_type  ff (0.0);

            for (size_type c = 0; c < r; ++c)  {
                imagi (0, c) /= h;
                ff += imagi (0, c) * e_vals (0, c);
            }

            const value_type    hh = ff / (h + h);

            for (size_type c = 0; c < r; ++c)
                imagi (0, c) -= hh * e_vals (0, c);

            for (size_type c = 0; c < r; ++c)  {
                for (size_type cc = c; cc <= r - 1; ++cc)
                    e_vecs (cc, c) -= e_vals (0, c) * imagi (0, cc) +
                                      imagi (0, c) * e_vals (0, cc);

                e_vals (0, c) = e_vecs (r - 1, c);
                e_vecs (r, c) = value_type(0.0);
            }
        }

        e_vals (0, r) = h;
    }

   // Accumulate transformations.
   //
    for (size_type r = 0; r < e_vecs.rows () - 1; ++r)  {
        e_vecs (e_vecs.rows () - 1, r) = e_vecs (r, r);
        e_vecs (r, r) = value_type(1.0);

        const value_type    &h = e_vals (0, r + 1);

        if (h != value_type(0.0))  {
            for (size_type c = 0; c <= r; ++c)
                e_vals (0, c) = e_vecs (c, r + 1) / h;

            for (size_type c = 0; c <= r; ++c)  {
                value_type  g (0.0);

                for (size_type rr = 0; rr <= r; ++rr)
                    g += e_vecs (rr, r + 1) * e_vecs (rr, c);

                for (size_type rr = 0; rr <= r; ++rr)
                    e_vecs (rr, c) -= g * e_vals (0, rr);
            }
        }
        for (size_type rr = 0; rr <= r; ++rr)
            e_vecs (rr, r + 1) = value_type(0.0);
    }

    for (size_type c = 0; c < e_vecs.columns (); ++c)  {
        e_vals (0, c) = e_vecs (e_vecs.rows () - 1, c);
        e_vecs (e_vecs.rows () - 1, c) = value_type(0.0);
    }
    e_vecs (e_vecs.rows () - 1, e_vecs.columns () - 1) = value_type(1.0);
    imagi (0, 0) = value_type(0.0);

    return;
}

// ----------------------------------------------------------------------------

// Static
//
template<template<class T> class BASE, class TYPE>
template<class MAT>
inline void Matrix<BASE, TYPE>::
diagonalize_ (MAT &e_vecs, MAT &e_vals, MAT &imagi) noexcept  {

    for (size_type c = 1; c < e_vecs.columns (); ++c)
        imagi (0, c - 1) = imagi (0, c);
    imagi (0, e_vecs.columns () - 1) = value_type(0.0);

    value_type  f (0.0);
    value_type  tst1 (0.0);

    for (size_type c = 0; c < e_vecs.columns (); ++c)  {
       // Find small subdiagonal element
       //
        tst1 = std::max (tst1, abs__ (e_vals (0, c)) + abs__ (imagi (0, c)));

        size_type   m = c;

        while (m < e_vecs.columns ())  {
            if (abs__ (imagi (0, m)) <= EPSILON_ * tst1)
                break;
            m += 1;
        }

       // If m == l, e_vals (0, c) is an eigenvalue,
       // otherwise, iterate.
       //
        if (m > c)  {
            do  {
               // Compute implicit shift
               //
                value_type  g = e_vals (0, c);
                value_type  p = (e_vals (0, c + 1) - g) /
                                (value_type(2.0) * imagi (0, c));

               // Euclidean distance func
               //
                value_type  dis =
                    p < value_type(0.0) ? -hypot__ (p, value_type(1.0))
                                        :  hypot__ (p, value_type(1.0));

                e_vals (0, c) = imagi (0, c) / (p + dis);
                e_vals (0, c + 1) = imagi (0, c) * (p + dis);

                const value_type    dl1 = e_vals (0, c + 1);
                value_type          h = g - e_vals (0, c);

                for (size_type cc = c + 2; cc < e_vecs.columns (); ++cc)
                    e_vals (0, cc) -= h;

                f += h;

               // Implicit QL transformation.
               //
                p = e_vals (0, m);

                value_type  cunit (1.0);
                value_type  c2 (1.0);
                value_type  c3 (1.0);
                value_type  el1 = imagi (0, c + 1);
                value_type  s (0.0);
                value_type  s2 (0.0);

                for (int cc = m - 1; cc >= static_cast<int>(c); --cc)  {
                    c3 = c2;
                    c2 = cunit;
                    s2 = s;
                    g = cunit * imagi (0, cc);
                    h = cunit * p;
                    dis = hypot__ (p, imagi (0, cc));
                    imagi (0, cc + 1) = s * dis;
                    s = imagi (0, cc) / dis;
                    cunit = p / dis;
                    p = cunit * e_vals (0, cc) - s * g;
                    e_vals (0, cc + 1) =
                        h + s * (cunit * g + s * e_vals (0, cc));

                   // Accumulate transformation.
                   //
                    for (size_type r = 0; r < e_vecs.rows (); ++r)  {
                        h = e_vecs (r, cc + 1);
                        e_vecs (r, cc + 1) = s * e_vecs (r, cc) + cunit * h;
                        e_vecs (r, cc) = cunit * e_vecs (r, cc) - s * h;
                    }
                }

                p = -s * s2 * c3 * el1 * imagi (0, c) / dl1;
                imagi (0, c) = s * p;
                e_vals (0, c) = cunit * p;

              // Check for convergence.
              //
            }  while (abs__ (imagi (0, c)) > EPSILON_ * tst1);
        }

        e_vals (0, c) = e_vals (0, c) + f;
        imagi (0, c) = value_type(0.0);
    }

    return;
}

// ----------------------------------------------------------------------------

// Static
//
template<template<class T> class BASE, class TYPE>
template<class MAT>
inline void Matrix<BASE, TYPE>::
red_to_hessenberg_ (MAT &e_vecs, MAT &hess_form) noexcept  {

    MAT ortho (1, e_vecs.columns ());

    for (size_type c = 1; c <= e_vecs.columns () - 2; ++c)  {
        value_type  scale (0.0);

       // Scale column.
       //
        for (size_type r = c; r <= e_vecs.rows () - 1; ++r)
            scale += abs__ (hess_form (r, c - 1));

        if (scale != value_type(0.0))  {
            value_type  h (0.0);

           // Compute Householder transformation.
           //
            for (size_type cc = e_vecs.columns () - 1; cc >= c; --cc)  {
                ortho (0, cc) = hess_form (cc, c - 1) / scale;
                h += ortho (0, cc) * ortho (0, cc);
            }

            const value_type    g =
                ortho (0, c) > value_type(0.0) ? -sqrt__ (h) : sqrt__ (h);

            h -= ortho (0, c) * g;
            ortho (0, c) -= g;

           // Apply Householder similarity transformation
           // H = (I - u * u' / h) * H * (I - u * u') / h)
           //
            for (size_type cc = c; cc < e_vecs.columns (); ++cc)  {
                value_type  f (0.0);

                for (size_type r = e_vecs.rows () - 1; r >= c; --r)
                    f += ortho (0, r) * hess_form (r, cc);
                f /= h;

                for (size_type r = c; r <= e_vecs.rows () - 1; ++r)
                    hess_form (r, cc) -= f * ortho (0, r);
            }

            for (size_type r = 0; r <= e_vecs.rows () - 1; ++r)  {
                value_type  f (0.0);

                for (size_type cc = e_vecs.columns () - 1; cc >= c; --cc)
                    f += ortho (0, cc) * hess_form (r, cc);
                f /= h;

                for (size_type cc = c; cc <= e_vecs.columns () - 1; ++cc)
                    hess_form (r, cc) -= f * ortho (0, cc);
            }

            ortho (0, c) *= scale;
            hess_form (c, c - 1) = scale * g;
        }
    }

   // Accumulate transformations (Algol's ortran).
   //
    for (size_type r = 0; r < e_vecs.rows (); ++r)
        for (size_type c = 0; c < e_vecs.columns (); ++c)
            e_vecs (r, c) = r == c ? value_type(1.0) : value_type(0.0);

    for (size_type c = e_vecs.columns () - 2; c >= 1; --c)
        if (hess_form (c, c - 1) != value_type(0.0))  {
            for (size_type r = c + 1; r <= e_vecs.rows () - 1; ++r)
                ortho (0, r) = hess_form (r, c - 1);

            for (size_type cc = c; cc <= e_vecs.columns () - 1; ++cc)  {
                value_type  g (0.0);

                for (size_type r = c; r <= e_vecs.rows () - 1; ++r)
                    g += ortho (0, r) * e_vecs (r, cc);

               // Double division avoids possible underflow
               //
                g = (g / ortho (0, c)) / hess_form (c, c - 1);
                for (size_type r = c; r <= e_vecs.rows () - 1; ++r)
                    e_vecs (r, cc) += g * ortho (0, r);
            }
        }

    return;
}

// ----------------------------------------------------------------------------

// Static
//
template<template<class T> class BASE, class TYPE>
template<class MAT>
inline void Matrix<BASE, TYPE>::
hessenberg_to_schur_ (MAT &e_vecs,
                      MAT &e_vals,
                      MAT &imagi,
                      MAT &hess_form) noexcept  {

   // Store roots isolated by balanc and compute matrix norm
   //
    value_type  norm (0.0);

    for (size_type r = 0; r < e_vecs.rows (); ++r)
        for (size_type c = r; c < e_vecs.columns (); ++c)
            norm += abs__ (hess_form (r, c));

    size_type   iter = 0;
    int         n = static_cast<int>(e_vecs.columns () - 1);
    value_type  exshift (0.0);
    value_type  p;
    value_type  q;
    value_type  oo;
    value_type  s;
    value_type  z;
    value_type  w;
    value_type  x;
    value_type  y;

   // Outer loop over eigenvalue index
   //
    while (n >= 0)  {
        int l = n;

       // Look for single small sub-diagonal element
       //
        while (l > 0)  {
            s = abs__ (hess_form (l - 1, l - 1)) + abs__ (hess_form (l, l));

            if (s == value_type(0.0))
                s = norm;
            if (abs__ (hess_form (l, l - 1)) < EPSILON_ * s)
                break;

            l -= 1;
        }

       // Check for convergence
       //
        if (l == n)  {  // One root found
            hess_form (n, n) += exshift;
            e_vals (0, n) = hess_form (n, n);
            imagi (0, n--) = value_type(0.0);
            iter = 0;
        }
        else if (l == n - 1)  {  // Two roots found
            w = hess_form (n, n - 1) * hess_form (n - 1, n);
            p = (hess_form (n - 1, n - 1) - hess_form(n, n)) / value_type(2.0);
            q = p * p + w;
            z = sqrt__ (abs__ (q));

            hess_form (n, n) += exshift;
            hess_form (n - 1, n - 1) += exshift;

            const value_type    &xx = hess_form (n, n);

            if (q >= value_type(0.0))  {  // Real pair
                z = p >= value_type(0.0) ? p + z : p - z;

                e_vals (0, n - 1) = xx + z;
                e_vals (0, n) = z !=
                    value_type(0.0) ? xx - w / z : e_vals (0, n - 1);

                imagi (0, n - 1) = value_type(0.0);
                imagi (0, n) = value_type(0.0);

                const value_type    &cref = hess_form (n, n - 1);

                s = abs__ (cref) + abs__ (z);
                p = cref / s;
                q = z / s;
                oo = sqrt__ (p * p + q * q);
                p /= oo;
                q /= oo;

               // Row modification
               //
                for (size_type c = n - 1; c < e_vecs.columns (); ++c)  {
                    const value_type    &cref = hess_form (n - 1, c);

                    hess_form (n - 1, c) = q * cref + p * hess_form (n, c);
                    hess_form (n, c) = q * hess_form (n, c) - p * cref;
                }

               // Column modification
               //
                for (size_type r = 0; r <= n; ++r)  {
                    const value_type    &cref = hess_form (r, n - 1);

                    hess_form (r, n - 1) = q * cref + p * hess_form (r, n);
                    hess_form (r, n) = q * hess_form (r, n) - p * cref;
                }

               // Accumulate transformations
               //
                for (size_type r = 0; r <= e_vecs.rows () - 1; ++r)  {
                    const value_type    &cref = e_vecs (r, n - 1);

                    e_vecs (r, n - 1) = q * cref + p * e_vecs (r, n);
                    e_vecs (r, n) = q * e_vecs (r, n) - p * cref;
                }
            }
            else  {  // Complex pair
                e_vals (0, n - 1) = xx + p;
                e_vals (0, n) = xx + p;
                imagi (0, n - 1) = z;
                imagi (0, n) = -z;
            }

            n -= 2;
            iter = 0;
        }
        else  {  // No convergence yet

           // Form shift
           //
            x = hess_form (n, n);
            y = value_type(0.0);
            w = value_type(0.0);
            if (l < n)  {
                y = hess_form (n - 1, n - 1);
                w = hess_form (n, n - 1) * hess_form (n - 1, n);
            }

           // Wilkinson's original ad hoc shift
           //
            if (iter == 10)  {
                exshift += x;
                for (size_type r = 0; r <= n; ++r)
                    hess_form (r, r) -= x;

                s = abs__ (hess_form (n, n - 1)) +
                    abs__ (hess_form (n - 1, n - 2));
                x = y = value_type(0.75) * s;
                w = value_type(-0.4375) * s * s;
            }

           // MATLAB's new ad hoc shift
           //
            if (iter == 30)  {
                s = (y - x) / value_type(2.0);
                s = s * s + w;

                if (s > 0)  {
                    s = y < x ? -sqrt__ (s) : sqrt__ (s);
                    s = x - w / ((y - x) / value_type(2.0) + s);

                    for (size_type r = 0; r <= n; ++r)
                        hess_form (r, r) -= s;

                    exshift += s;
                    x = y = w = value_type(0.964);
                }
            }

            iter += 1;  // Could check iteration count here?

           // Look for two consecutive small sub-diagonal elements
           //
            int m = n - 2;

            while (m >= l)  {
                const value_type    &cref = hess_form (m, m);

                oo = x - cref;
                s = y - cref;
                p = (oo * s - w) / hess_form (m + 1, m) + hess_form (m, m + 1);
                q = hess_form (m + 1, m + 1) - cref - oo - s;
                oo = hess_form (m + 2, m + 1);
                s = abs__ (p) + abs__ (q) + abs__ (oo);

                p /= s;
                q /= s;
                oo /= s;

                if (m == l)
                    break;

                if (abs__ (hess_form(m, m - 1)) * (abs__ (q) + abs__ (oo)) <
                        EPSILON_ *
                        (abs__ (p) *
                         (abs__ (hess_form (m - 1, m - 1)) +
                          abs__ (cref) +
                          abs__ (hess_form (m + 1, m + 1)))))
                    break;

                m -= 1;
            }

            for (size_type r = m + 2; r <= n; ++r)  {
                hess_form (r, r - 2) = value_type(0.0);

                if (r > m + 2)
                    hess_form (r, r - 3) = value_type(0.0);
            }

           // Double QR step involving rows l to n and columns m to n
           //
            for (size_type k = m; k <= n - 1; ++k)  {
                const bool  notlast = k != n - 1;

                if (k != m)  {
                    p = hess_form (k, k - 1);
                    q = hess_form (k + 1, k - 1);
                    oo = notlast ? hess_form (k + 2, k - 1) : value_type(0.0);
                    x = abs__ (p) + abs__ (q) + abs__ (oo);

                    if (x != value_type(0.0))  {
                        p /= x;
                        q /= x;
                        oo /= x;
                    }
                }
                if (x == value_type(0.0))
                    break;

                s = p < value_type(0.0) ? -sqrt__ (p * p + q * q + oo * oo)
                                        :  sqrt__ (p * p + q * q + oo * oo);

                if (s != value_type(0.0))  {
                    if (k != m)
                        hess_form (k, k - 1) = -s * x;
                    else if (l != m)
                        hess_form (k, k - 1) = -hess_form (k, k - 1);

                    p += s;
                    x = p / s;
                    y = q / s;
                    z = oo / s;

                    q /= p;
                    oo /= p;

                   // Row modification
                   //
                    for (size_type c = k; c < e_vecs.columns (); ++c)  {
                        p = hess_form (k, c) + q * hess_form (k + 1, c);

                        if (notlast)  {
                            p += oo * hess_form (k + 2, c);
                            hess_form (k + 2, c) -= p * z;
                        }

                        hess_form (k, c) -= p * x;
                        hess_form (k + 1, c) -= p * y;
                    }

                   // Column modification
                   //
                    for (size_type r = 0;
                         r <= std::min (n, static_cast<int>(k + 3)); ++r)  {
                        p = x * hess_form (r, k) + y * hess_form (r, k + 1);

                        if (notlast)  {
                            p += z * hess_form (r, k + 2);
                            hess_form (r, k + 2) -= p * oo;
                         }

                         hess_form (r, k) -= p;
                         hess_form (r, k + 1) -= p * q;
                    }

                   // Accumulate transformations
                   //
                    for (size_type r = 0; r <= e_vecs.rows () - 1; ++r)  {
                        p = x * e_vecs (r, k) + y * e_vecs (r, k + 1);

                        if (notlast)  {
                            p += z * e_vecs (r, k + 2);
                            e_vecs (r, k + 2) -= p * oo;
                        }

                        e_vecs (r, k) -= p;
                        e_vecs (r, k + 1) -= p * q;
                    }
                }
            }
        }
    }

    if (norm == value_type(0.0))
        return;

   // Backsubstitute to find vectors of upper triangular form
   //
    for (int c = static_cast<int>(e_vecs.columns () - 1); c >= 0; --c)  {
        p = e_vals (0, c);
        q = imagi (0, c);

        if (q == value_type(0.0))  {  // Real vector
            int l = c;

            hess_form (c, c) = value_type(1.0);

            for (int r = c - 1; r >= 0; --r)  {
                const value_type    ww = hess_form (r, r) - p;

                oo = value_type(0.0);
                for (int cc = l; cc <= c; ++cc)
                    oo += hess_form (r, cc) * hess_form (cc, c);

                if (imagi (0, r) < value_type(0.0))  {
                    z = ww;
                    s = oo;
                }
                else  {
                    l = r;

                    if (imagi (0, r) == value_type(0.0))
                        hess_form (r, c) = ww != value_type(0.0)
                                               ? -oo / ww
                                               : -oo / (EPSILON_ * norm);
                    else  {  // Solve real equations
                        q = (e_vals (0, r) - p) * (e_vals (0, r) - p) +
                            imagi (0, r) * imagi (0, r);

                        const value_type    &xx = hess_form (r, r + 1);
                        const value_type    tt = (xx * s - z * oo) / q;

                        hess_form (r, c) = tt;

                        hess_form (r + 1, c) =
                            abs__ (xx) > abs__ (z)
                                ? (-oo - ww * tt) / xx
                                : (-s - hess_form (r + 1, r) * tt) / z;
                    }

                   // Overflow control
                   //
                    const value_type    t = abs__ (hess_form (r, c));

                    if (EPSILON_ * t * t > value_type(1.0))
                        for (size_type rr = r; rr <= c; ++rr)
                            hess_form (rr, c) /= t;
                }
            }
        }
        else if (q < value_type(0.0))  {  // Complex vector
            size_type   l = c - 1;
            value_type  cdivr;
            value_type  cdivi;

           // Last vector component imaginary so matrix is triangular
           //
            if (abs__ (hess_form(c, c - 1)) > abs__ (hess_form(c - 1, c)))  {
                hess_form (c - 1, c - 1) = q / hess_form (c, c - 1);
                hess_form (c - 1, c) =
                    -(hess_form (c, c) - p) / hess_form (c, c - 1);
            }
            else  {
                cdiv_ (value_type(0.0),
                       -hess_form (c - 1, c),
                       hess_form (c - 1, c - 1) - p,
                       q,
                       cdivr,
                       cdivi);
                hess_form (c - 1, c - 1) = cdivr;
                hess_form (c - 1, c) = cdivi;
            }

            hess_form (c, c - 1) = value_type(0.0);
            hess_form (c, c) = value_type(1.0);

            for (int r = c - 2; r >= 0; --r)  {
                value_type  ra (0.0);
                value_type  sa (0.0);

                for (int cc = l; cc <= c; ++cc)  {
                    ra += hess_form (r, cc) * hess_form (cc, c - 1);
                    sa += hess_form (r, cc) * hess_form (cc, c);
                }

                const value_type    ww = hess_form (r, r) - p;

                if (imagi (0, r) < value_type(0.0))  {
                    z = ww;
                    oo = ra;
                    s = sa;
                }
                else  {
                    l = r;

                    if (imagi (0, r) == value_type(0.0))  {
                        cdiv_ (-ra, -sa, ww, q, cdivr, cdivi);
                        hess_form (r, c - 1) = cdivr;
                        hess_form (r, c) = cdivi;
                    }
                    else  {  // Solve complex equations
                        const value_type    &xx = hess_form (r, r + 1);
                        const value_type    &yy = hess_form (r + 1, r);
                        const value_type    vi =
                            (e_vals (0, r) - p) * value_type(2.0) * q;
                        value_type          vr =
                            (e_vals (0, r) - p) * (e_vals (0, r) - p) +
                            imagi (0, r) * imagi (0, r) -
                            q * q;

                        if (vr == value_type(0.0) && vi == value_type(0.0))
                            vr = EPSILON_ *
                                 norm *
                                 (abs__ (ww) +
                                  abs__ (q) +
                                  abs__ (xx) +
                                  abs__ (yy) +
                                  abs__ (z));

                        cdiv_ (xx * oo - z * ra + q * sa,
                               xx * s - z * sa - q * ra,
                               vr,
                               vi,
                               cdivr, cdivi);
                        hess_form (r, c - 1) = cdivr;
                        hess_form (r, c) = cdivi;

                        if (abs__ (xx) > abs__ (z) + abs__ (q))  {
                            hess_form (r + 1, c - 1) =
                                (-ra -
                                 ww * hess_form (r, c - 1) +
                                 q * hess_form (r, c)) /
                                xx;
                            hess_form (r + 1, c) =
                                (-sa - ww * hess_form (r, c) -
                                 q * hess_form (r, c - 1)) /
                                xx;
                        }
                        else  {
                            cdiv_ (-oo - yy * hess_form (r, c - 1),
                                   -s - yy * hess_form (r, c),
                                   z,
                                   q,
                                   cdivr, cdivi);
                            hess_form (r + 1, c - 1) = cdivr;
                            hess_form (r + 1, c) = cdivi;
                        }
                    }

                   // Overflow control
                   //
                    const value_type    t =
                        std::max (abs__ (hess_form (r, c - 1)),
                                  abs__ (hess_form (r, c)));

                    if (EPSILON_ * t * t > 1)
                        for (size_type rr = r; rr <= c; ++rr)  {
                            hess_form (rr, c - 1) /= t;
                            hess_form (rr, c) /= t;
                        }
                }
            }
        }
    }

   // Back transformation to get eigenvectors of original matrix
   //
    for (int c = static_cast<int>(e_vecs.columns () - 1); c >= 0; --c)
        for (size_type r = 0; r <= e_vecs.rows () - 1; ++r)  {
            z = value_type(0.0);

            for (size_type k = 0;
                 k <= std::min (c, static_cast<int>(e_vecs.columns() - 1));
                 ++k)
                z += e_vecs (r, k) * hess_form (k, c);

            e_vecs (r, c) = z;
        }

   return;
}

// ----------------------------------------------------------------------------

// Static
//
template<template<class T> class BASE, class TYPE>
inline void Matrix<BASE, TYPE>::
cdiv_ (value_type xr,
       value_type xi,
       value_type yr,
       value_type yi,
       value_type &cdivr,
       value_type &cdivi) noexcept  {

    if (abs__ (yr) > abs__ (yi))  {
        const value_type    r = yi / yr;
        const value_type    d = yr + r * yi;

        cdivr = (xr + r * xi) / d;
        cdivi = (xi - r * xr) / d;
    }
    else  {
        const value_type    r = yr / yi;
        const value_type    d = yi + r * yr;

        cdivr = (r * xr + xi) / d;
        cdivi = (r * xi - xr) / d;
    }

    return;
}

} // namespace hmma

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
