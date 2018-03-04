// Hossein Moein
// August 1, 2009
// Copyright (C) 2017-2018 Hossein Moein
// Distributed under the BSD Software License (see file License)

#ifndef _INCLUDED_MatrixBase_h
#define _INCLUDED_MatrixBase_h

// ----------------------------------------------------------------------------

namespace hmma
{

template<class TYPE>
class   MatrixBase  {

    public:

        struct  Exception { inline Exception () noexcept  {   }; };

        struct  NotSquare : public Exception  {

            inline NotSquare () noexcept  {   };
        };
        struct  Singular : public Exception  {

            inline Singular () noexcept  {   };
        };
        struct  NotSolvable : public Exception  {

            inline NotSolvable () noexcept  {   };
        };

    public:

        typedef unsigned int        size_type;
        typedef TYPE            value_type;
        typedef value_type &        reference;
        typedef const value_type &  const_reference;
        typedef value_type *        pointer;
        typedef const value_type *  const_pointer;
};

// ----------------------------------------------------------------------------

#include <vector>

// ----------------------------------------------------------------------------

template<class TYPE, class STORAGE = std::vector<TYPE> >
class   DenseMatrixStorage : public MatrixBase<TYPE>  {

    public:

        typedef MatrixBase<TYPE>         BaseClass;
        typedef typename BaseClass::size_type       size_type;
        typedef typename BaseClass::value_type      value_type;
        typedef typename BaseClass::reference       reference;
        typedef typename BaseClass::const_reference const_reference;
        typedef typename BaseClass::pointer         pointer;
        typedef typename BaseClass::const_pointer   const_pointer;

    protected:

        typedef STORAGE DataVector;

        static  const   size_type   _NOPOS = static_cast<size_type>(-1);

        inline DataVector &_get_data () noexcept  { return (data_); }
        inline const DataVector &_get_data () const noexcept  {

            return (data_);
        }

        void _resize (size_type in_row,
                      size_type in_col,
                      size_type data_size,
                      bool set_all_to_def = true,
                      const_reference def_value = value_type ());

        inline DenseMatrixStorage () noexcept
            : rows_ (0), cols_ (0), data_ ()  {   }

        inline DenseMatrixStorage (size_type row,
                                  size_type col,
                                  size_type data_size,
                                  const_reference def_value = value_type ())
            noexcept
            : rows_ (row),
              cols_ (col),
              data_ (data_size, def_value)  {   }

    public:

        void clear () noexcept;
        inline void swap (DenseMatrixStorage &rhs) noexcept;
        inline bool empty () const noexcept  { return (data_.empty ()); }

        inline size_type rows () const noexcept  { return (rows_); }
        inline size_type columns () const noexcept  { return (cols_); }

    private:

        size_type   rows_;
        size_type   cols_;
        DataVector  data_;
};

} // namespace hmma

// ----------------------------------------------------------------------------

#  ifdef DMS_INCLUDE_SOURCE
#    include <MatrixBase.tcc>
#  endif // DMS_INCLUDE_SOURCE

// ----------------------------------------------------------------------------

#undef _INCLUDED_MatrixBase_h
#define _INCLUDED_MatrixBase_h 1
#endif  // _INCLUDED_MatrixBase_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
