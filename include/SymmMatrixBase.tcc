// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <SymmMatrixBase.h>

// ----------------------------------------------------------------------------

namespace hmma
{

template<class TYPE>
inline typename SymmMatrixBase<TYPE>::reference
SymmMatrixBase<TYPE>::at (size_type r, size_type c) noexcept  {

    if (r > c)
        std::swap (r, c);

    return (BaseClass::_get_data ()
                [(r * BaseClass::columns ()) + c - ((r * (r + 1)) >> 1)]);
}

// ----------------------------------------------------------------------------

template<class TYPE>
inline typename SymmMatrixBase<TYPE>::const_reference
SymmMatrixBase<TYPE>::
at (size_type r, size_type c) const noexcept  {

    if (r > c)
        std::swap (r, c);

    return (BaseClass::_get_data ()
                [(r * BaseClass::columns ()) + c - ((r * (r + 1)) >> 1)]);
}

// ----------------------------------------------------------------------------

template<class TYPE>
void SymmMatrixBase<TYPE>::
resize (size_type in_row, size_type in_col, const_reference def_value)  {

    BaseClass::_resize (in_row,
                        in_col,
                        (in_col * (in_col + 1)) / 2,
                        true,
                        def_value);
    return;
}

// ----------------------------------------------------------------------------

template<class TYPE>
std::ostream &SymmMatrixBase<TYPE>::
dump (std::ostream &out_stream) const {

    // const   size_type           old_precision = out_stream.precision (2);
    const   size_type           old_width = out_stream.width (6);
    const   std::ios::fmtflags  old_flags =
        out_stream.setf (std::ios::fixed, std::ios::floatfield);

    out_stream << "   ";

    for (size_type r = 0 ; r < BaseClass::rows (); ++r)  {
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
