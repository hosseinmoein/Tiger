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
* Neither the name of Hossein Moein and/or the Tiger nor the
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

#include <Tiger/DenseMatrixBase.h>

// ----------------------------------------------------------------------------

namespace hmma
{

// This is a column major matrix. if it were row major, we would have
// return (*(data_.begin () + (r * columns () + c)));
//
template<class T>
inline typename DenseMatrixBase<T>::reference
DenseMatrixBase<T>::at (size_type r, size_type c) noexcept  {

    return (BaseClass::_get_data () [c * BaseClass::rows () + r]);
}

// ----------------------------------------------------------------------------

template<class T>
inline typename DenseMatrixBase<T>::const_reference
DenseMatrixBase<T>::
at (size_type r, size_type c) const noexcept  {

    return (BaseClass::_get_data () [c * BaseClass::rows () + r]);
}

// ----------------------------------------------------------------------------

template<class T>
inline typename DenseMatrixBase<T>::ColumnVector
DenseMatrixBase<T>::get_column (size_type c) noexcept  {

    return (
        ColumnVector (
            &(BaseClass::_get_data () [c * BaseClass::rows ()]),
            &(BaseClass::_get_data () [((c + 1) * BaseClass::rows ()) - 1])));
}

// ----------------------------------------------------------------------------

template<class T>
inline typename DenseMatrixBase<T>::ColumnVector
DenseMatrixBase<T>::get_column (size_type c) const noexcept  {

    return (
        ColumnVector (
            const_cast<value_type *>
        (&(BaseClass::_get_data () [c * BaseClass::rows ()])),
            const_cast<value_type *>
        (&(BaseClass::_get_data () [((c + 1) * BaseClass::rows ()) - 1]))));
}

// ----------------------------------------------------------------------------

template<class T>
inline typename DenseMatrixBase<T>::RowVector
DenseMatrixBase<T>::get_row (size_type r) noexcept  {

    return (
        RowVector (
            &(BaseClass::_get_data () [r]),
            &(*(BaseClass::_get_data ().end () - (BaseClass::rows () - r))),
            BaseClass::rows ()));
}

// ----------------------------------------------------------------------------

template<class T>
inline typename DenseMatrixBase<T>::RowVector
DenseMatrixBase<T>::get_row (size_type r) const noexcept  {

    return (
        RowVector (
            const_cast<value_type *>
             (&(BaseClass::_get_data () [r])),
            const_cast<value_type *>
             (&(*(BaseClass::_get_data ().end () - (BaseClass::rows () - r)))),
             BaseClass::rows ()));
}

// ----------------------------------------------------------------------------

template<class T>
template<class ITER>
inline void DenseMatrixBase<T>::
set_column (ITER col_data, size_type col)  {

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        at (r, col) = *col_data++;

    return;
}

// ----------------------------------------------------------------------------

template<class T>
template<class ITER>
inline void DenseMatrixBase<T>::
set_row (ITER row_data, size_type row)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)
        at (row, c) = *row_data++;

    return;
}

// ----------------------------------------------------------------------------

template<class T>
template<class OPT, class ITER>
inline void DenseMatrixBase<T>::
column_operation (OPT opt, ITER col_data, size_type col)  {

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        reference   col_ref = at (r, col);

        col_ref = opt (col_ref, *col_data++);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class T>
template<class OPT, class ITER>
inline void DenseMatrixBase<T>::
row_operation (OPT opt, ITER row_data, size_type row)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        reference   row_ref = at (row, c);

        row_ref = opt (row_ref, *row_data++);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class T>
template<class OPT, class EXPR>
inline void DenseMatrixBase<T>::
scale_column (OPT opt, const EXPR &e, size_type col)  {

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        reference   col_ref = at (r, col);

        col_ref = opt (col_ref, e);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class T>
template<class OPT, class EXPR>
inline void DenseMatrixBase<T>::
scale_row (OPT opt, const EXPR &e, size_type row)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        reference   row_ref = at (row, c);

        row_ref = opt (row_ref, e);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class T>
template<class OPT, class EXPR>
inline void
DenseMatrixBase<T>::
scale (OPT opt, const EXPR &e) noexcept  {

    for (col_iterator iter = col_begin (); iter != col_end (); ++iter)
        *iter = opt (*iter, e);

    return;
}

// ----------------------------------------------------------------------------

template<class T>
void DenseMatrixBase<T>::
resize (size_type in_row, size_type in_col, const_reference def_value)  {

    BaseClass::_resize (in_row, in_col, in_row * in_col, true, def_value);
    return;
}

// ----------------------------------------------------------------------------

template<class T>
std::ostream &DenseMatrixBase<T>::
dump (std::ostream &out_stream) const  {

    // const   size_type           old_precision = out_stream.precision (2);
    const   size_type           old_width = out_stream.width (6);
    const   std::ios::fmtflags  old_flags =
        out_stream.setf (std::ios::fixed, std::ios::floatfield);

    out_stream << "   ";

    for (size_type r = 0 ; r < BaseClass::rows () ; ++r)  {
        for (size_type c = 0 ; c < BaseClass::columns (); ++c)
            if (r == 0 && c == 0)
                out_stream << at (r, c);
            else
                out_stream << "     " << at (r, c);

        out_stream << std::endl;
    }

    out_stream.setf (old_flags);
    out_stream.width (old_width);
    // out_stream.precision (old_precision);
    return (out_stream);
}

} // namespace hmma

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
