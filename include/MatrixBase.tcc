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

#include <MatrixBase.h>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <cstdlib>

// ----------------------------------------------------------------------------

//
// Dense Matrix Storage
//

// ----------------------------------------------------------------------------

namespace hmma
{

template<typename T>
inline T _str_to_num_ (const char *str) noexcept;

template<>
inline int
_str_to_num_<int>(const char *str) noexcept  {

    return (static_cast<int>(::strtol(str, nullptr, 0)));
}

template<>
inline unsigned int
_str_to_num_<unsigned int>(const char *str) noexcept  {

    return (static_cast<unsigned int>(::strtol(str, nullptr, 0)));
}

template<>
inline long int
_str_to_num_<long int>(const char *str) noexcept  {

    return (::strtol(str, nullptr, 0));
}

template<>
inline unsigned long int
_str_to_num_<unsigned long int>(const char *str) noexcept  {

    return (static_cast<unsigned long int>(::strtol(str, nullptr, 0)));
}

template<>
inline long long int
_str_to_num_<long long int>(const char *str) noexcept  {

    return (::strtoll(str, nullptr, 0));
}

template<>
inline unsigned long long int
_str_to_num_<unsigned long long int>(const char *str) noexcept  {

    return (static_cast<unsigned long long int>(::strtol(str, nullptr, 0)));
}

template<>
inline float
_str_to_num_<float>(const char *str) noexcept  {

    return (::strtof(str, nullptr));
}

template<>
inline double
_str_to_num_<double>(const char *str) noexcept  {

    return (::strtod(str, nullptr));
}

template<>
inline long double
_str_to_num_<long double>(const char *str) noexcept  {

    return (::strtold(str, nullptr));
}

// ----------------------------------------------------------------------------

template<class T, class S>
void DenseMatrixStorage<T, S>::
_resize (size_type in_row,
         size_type in_col,
         size_type data_size,
         bool set_all_to_def,
         const_reference def_value)  {

    if (data_size != data_.size ())
        data_.resize (data_size, def_value);
    if (set_all_to_def)
        std::fill (data_.begin (), data_.end (), def_value);

    rows_ = in_row;
    cols_ = in_col;
    return;
}

// ----------------------------------------------------------------------------

template<class T, class S>
void DenseMatrixStorage<T, S>::clear () noexcept  {

    data_.clear ();
    rows_ = 0;
    cols_ = 0;
    return;
}

// ----------------------------------------------------------------------------

template<class T, class S>
inline void DenseMatrixStorage<T, S>::
swap (DenseMatrixStorage &rhs) noexcept  {

    data_.swap (rhs.data_);
    std::swap (rows_, rhs.rows_);
    std::swap (cols_, rhs.cols_);

    return;
}

// ----------------------------------------------------------------------------

template<class T, class S>
template<typename STRM>
bool DenseMatrixStorage<T, S>::write (STRM &stream, io_format iof) const  {

    if (iof != io_format::csv)
        throw std::runtime_error ("DenseMatrixStorage::write(): Currently, "
                                  "only csv I/O format is supported");

    // Number of rows X number of columns X actual data vector size
    // In case of symmetric matrices the actual data vector size is smaller.
    stream << rows() << 'X' << columns() << 'X' << data_.size() << '\n';

    size_type   counter = 0;

    for (const auto &citer : data_)  {
        stream << std::setprecision(12) << citer << ',';
        if (++counter == 2048)  {
            stream << '|';
            counter = 0;
        }
    }
    if (counter != 0)  stream << '|';
    stream << std::flush;
    return (true);
}

// ----------------------------------------------------------------------------

template<class T, class S>
bool DenseMatrixStorage<T, S>::read (const char *file_name, io_format iof)  {

    if (iof != io_format::csv)
        throw std::runtime_error ("DenseMatrixStorage::read(): Currently, "
                                  "only csv I/O format is supported");

    std::ifstream   file;

    file.open(file_name, std::ios::in);  // Open for reading

    char    buffer[65536];

    file.getline(buffer, sizeof(buffer));

    char            *marker = ::strtok(buffer, "X");
    const long int  num_rows = _str_to_num_<long int>(marker);
    const long int  num_cols = _str_to_num_<long int>(::strtok(nullptr, "X"));
    const long int  data_size = _str_to_num_<long int>(::strtok(nullptr, "X"));
    long int        counter = 0;

    _resize(num_rows, num_cols, data_size);
    while (file.getline(buffer, sizeof(buffer), '|'))  {
        marker = ::strtok(buffer, ",");
        while (marker != nullptr)  {
            data_[counter++] = _str_to_num_<T>(marker);
            marker = ::strtok(nullptr, ",");
        }
    }

    file.close();
    return (true);
}

} // namespace hmma

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
