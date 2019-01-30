// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

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
_str_to_num_<int>(const char *str) noexcept  { return (::atoi (str)); }

template<>
inline unsigned int
_str_to_num_<unsigned int>(const char *str) noexcept  { return (::atoi (str)); }

template<>
inline long int
_str_to_num_<long int>(const char *str) noexcept  { return (::atol (str)); }

template<>
inline unsigned long int
_str_to_num_<unsigned long int>(const char *str) noexcept  {

    return (::atol (str));
}

template<>
inline long long int
_str_to_num_<long long int>(const char *str) noexcept  {

    return (::atoll (str));
}

template<>
inline unsigned long long int
_str_to_num_<unsigned long long int>(const char *str) noexcept  {

    return (::atoll (str));
}

template<>
inline float
_str_to_num_<float>(const char *str) noexcept  {

    return (static_cast<float>(::atof (str)));
}

template<>
inline double
_str_to_num_<double>(const char *str) noexcept  { return (::atof (str)); }

template<>
inline long double
_str_to_num_<long double>(const char *str) noexcept  {

    return (static_cast<long double>(::atof (str)));
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
