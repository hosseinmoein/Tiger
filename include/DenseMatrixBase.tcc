// Hossein Moein
// August 1, 2009
// Copyright (C) 2017-2018 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <DenseMatrixBase.h>

// ----------------------------------------------------------------------------

namespace hmma
{

// This is a column major matrix. if it were row major, we would have
// return (*(data_.begin () + (r * columns () + c)));
//
template<class TYPE>
inline typename DenseMatrixBase<TYPE>::reference
DenseMatrixBase<TYPE>::at (size_type r, size_type c) noexcept  {

    return (BaseClass::_get_data () [c * BaseClass::rows () + r]);
}

// ----------------------------------------------------------------------------

template<class TYPE>
inline typename DenseMatrixBase<TYPE>::const_reference
DenseMatrixBase<TYPE>::
at (size_type r, size_type c) const noexcept  {

    return (BaseClass::_get_data () [c * BaseClass::rows () + r]);
}

// ----------------------------------------------------------------------------

template<class TYPE>
inline typename DenseMatrixBase<TYPE>::ColumnVector
DenseMatrixBase<TYPE>::get_column (size_type c) noexcept  {

    return (
        ColumnVector (
            &(BaseClass::_get_data () [c * BaseClass::rows ()]),
            &(BaseClass::_get_data () [((c + 1) * BaseClass::rows ()) - 1])));
}

// ----------------------------------------------------------------------------

template<class TYPE>
inline typename DenseMatrixBase<TYPE>::ColumnVector
DenseMatrixBase<TYPE>::get_column (size_type c) const noexcept  {

    return (
        ColumnVector (
            const_cast<value_type *>
        (&(BaseClass::_get_data () [c * BaseClass::rows ()])),
            const_cast<value_type *>
        (&(BaseClass::_get_data () [((c + 1) * BaseClass::rows ()) - 1]))));
}

// ----------------------------------------------------------------------------

template<class TYPE>
inline typename DenseMatrixBase<TYPE>::RowVector
DenseMatrixBase<TYPE>::get_row (size_type r) noexcept  {

    return (
        RowVector (
            &(BaseClass::_get_data () [r]),
            &(*(BaseClass::_get_data ().end () - (BaseClass::rows () - r))),
            BaseClass::rows ()));
}

// ----------------------------------------------------------------------------

template<class TYPE>
inline typename DenseMatrixBase<TYPE>::RowVector
DenseMatrixBase<TYPE>::get_row (size_type r) const noexcept  {

    return (
        RowVector (
            const_cast<value_type *>
             (&(BaseClass::_get_data () [r])),
            const_cast<value_type *>
             (&(*(BaseClass::_get_data ().end () - (BaseClass::rows () - r)))),
             BaseClass::rows ()));
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class ITER>
inline void DenseMatrixBase<TYPE>::
set_column (ITER col_data, size_type col)  {

    for (size_type r = 0; r < BaseClass::rows (); ++r)
        at (r, col) = *col_data++;

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class ITER>
inline void DenseMatrixBase<TYPE>::
set_row (ITER row_data, size_type row)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)
        at (row, c) = *row_data++;

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class OPT, class ITER>
inline void DenseMatrixBase<TYPE>::
column_operation (OPT opt, ITER col_data, size_type col)  {

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        reference   col_ref = at (r, col);

        col_ref = opt (col_ref, *col_data++);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class OPT, class ITER>
inline void DenseMatrixBase<TYPE>::
row_operation (OPT opt, ITER row_data, size_type row)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        reference   row_ref = at (row, c);

        row_ref = opt (row_ref, *row_data++);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class OPT, class EXPR>
inline void DenseMatrixBase<TYPE>::
scale_column (OPT opt, const EXPR &e, size_type col)  {

    for (size_type r = 0; r < BaseClass::rows (); ++r)  {
        reference   col_ref = at (r, col);

        col_ref = opt (col_ref, e);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class OPT, class EXPR>
inline void DenseMatrixBase<TYPE>::
scale_row (OPT opt, const EXPR &e, size_type row)  {

    for (size_type c = 0; c < BaseClass::columns (); ++c)  {
        reference   row_ref = at (row, c);

        row_ref = opt (row_ref, e);
    }

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
template<class OPT, class EXPR>
inline void
DenseMatrixBase<TYPE>::
scale (OPT opt, const EXPR &e) noexcept  {

    for (col_iterator iter = col_begin (); iter != col_end (); ++iter)
        *iter = opt (*iter, e);

    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
void DenseMatrixBase<TYPE>::
resize (size_type in_row, size_type in_col, const_reference def_value)  {

    BaseClass::_resize (in_row, in_col, in_row * in_col, true, def_value);
    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
std::ostream &DenseMatrixBase<TYPE>::
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
