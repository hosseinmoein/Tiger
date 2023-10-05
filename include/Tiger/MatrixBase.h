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

#pragma once

#include <vector>

// ----------------------------------------------------------------------------

namespace hmma
{

struct  MatrixException {  };
struct  NotSquare : public MatrixException  {  };
struct  Singular : public MatrixException  {  };
struct  NotSolvable : public MatrixException  {  };

// -------------------------------------

enum class io_format : unsigned char  {
    csv = 1,
    binary = 2
};

// -------------------------------------

template<class T>
struct  MatrixBase  {

    MatrixBase() = default;

    using size_type = unsigned int;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
};

// ----------------------------------------------------------------------------

template<class T, class S = std::vector<T> >
class   DenseMatrixStorage : public MatrixBase<T>  {

protected:

    using BaseClass = MatrixBase<T>;
    using size_type = typename BaseClass::size_type;
    using value_type = typename BaseClass::value_type;
    using reference = typename BaseClass::reference;
    using const_reference = typename BaseClass::const_reference;
    using pointer = typename BaseClass::pointer;
    using const_pointer = typename BaseClass::const_pointer;

    using DataVector = S;

    static const size_type  _NOPOS = static_cast<size_type>(-1);

    inline DataVector &_get_data () noexcept  { return (data_); }
    inline const DataVector &_get_data () const noexcept  { return (data_); }

    void _resize (size_type in_row,
                  size_type in_col,
                  size_type data_size,
                  bool set_all_to_def = true,
                  const_reference def_value = value_type ());

    inline DenseMatrixStorage (
        size_type row,
        size_type col,
        size_type data_size,
        const_reference def_value = value_type ()) noexcept
        : rows_ (row),
          cols_ (col),
          data_ (data_size, def_value)  {   }

public:

    DenseMatrixStorage() = default;

    void clear () noexcept;
    inline void swap (DenseMatrixStorage &rhs) noexcept;
    inline bool empty () const noexcept  { return (data_.empty ()); }

    inline size_type rows () const noexcept  { return (rows_); }
    inline size_type columns () const noexcept  { return (cols_); }

    template<typename STRM>
    bool write (STRM &stream, io_format iof = io_format::csv) const;
    bool read (const char *file_name, io_format iof = io_format::csv);

private:

    size_type   rows_ { 0 };
    size_type   cols_ { 0 };
    DataVector  data_ { };
};

} // namespace hmma

// ----------------------------------------------------------------------------

#  ifdef DMS_INCLUDE_SOURCE
#    include <Tiger/MatrixBase.tcc>
#  endif // DMS_INCLUDE_SOURCE

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
