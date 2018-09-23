// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#ifndef _INCLUDED_DenseMatrixBase_h
#define _INCLUDED_DenseMatrixBase_h

#include <iostream>

#include <DMScu_VectorRange.h>
#include <DMScu_StepVectorRange.h>

#include <MatrixBase.h>

// ----------------------------------------------------------------------------

namespace hmma
{

template<class T>
class   DenseMatrixBase : public DenseMatrixStorage<T>  {


    public:

        typedef DenseMatrixStorage<T>               BaseClass;

        typedef typename BaseClass::size_type       size_type;
        typedef typename BaseClass::value_type      value_type;
        typedef typename BaseClass::reference       reference;
        typedef typename BaseClass::const_reference const_reference;
        typedef typename BaseClass::pointer         pointer;
        typedef typename BaseClass::const_pointer   const_pointer;

        typedef typename BaseClass::Exception       Exception;
        typedef typename BaseClass::NotSquare       NotSquare;
        typedef typename BaseClass::Singular        Singular;
        typedef typename BaseClass::NotSolvable     NotSolvable;

        typedef DMScu_VectorRange<value_type>       ColumnVector;
        typedef DMScu_StepVectorRange<value_type>   RowVector;

        typedef DenseMatrixBase<value_type>         SelfType;

    protected:

        typedef typename BaseClass::DataVector  DataVector;

        inline DenseMatrixBase () noexcept  {   }

        inline
        DenseMatrixBase (size_type row,
                         size_type col,
                         const_reference def_value = value_type ())
            noexcept
            : BaseClass (row, col, row * col, def_value)  {   }

        static inline bool _is_symmetric_matrix () noexcept { return (false); }

    public:

        void resize (size_type in_row,
                     size_type in_col,
                     const_reference def_value = value_type ());

        inline reference at (size_type r, size_type c) noexcept;
        inline const_reference at (size_type r, size_type c) const noexcept;

       // Return the given row or column in vector format
       //
        inline ColumnVector get_column (size_type c) noexcept;
        inline ColumnVector get_column (size_type c) const noexcept;
        inline RowVector get_row (size_type r) noexcept;
        inline RowVector get_row (size_type r) const noexcept;

       // Set the given row or column from the given iterator
       //
        template<class ITER>
        inline void set_column (ITER col_data, size_type col);

        template<class ITER>
        inline void set_row (ITER row_data, size_type row);

       // Row and column operations run a binary operator on the row/column
       // and the given iterator.
       //
        template<class OPT, class ITER>
        inline void
        column_operation (OPT opt, ITER col_data, size_type col);

        template<class OPT, class ITER>
        inline void
        row_operation (OPT opt, ITER row_data, size_type row);

       // Row and column scale run a binary operator on the row/column
       // and the given expression.
       //
        template<class OPT, class EXPR>
        inline void scale_column (OPT opt, const EXPR &e, size_type col);

        template<class OPT, class EXPR>
        inline void scale_row (OPT opt, const EXPR &e, size_type row);

       // Scale the matrix by the given operator and scalar
       //
        template<class OPT, class EXPR>
        inline void scale (OPT opt, const EXPR &e) noexcept;

        std::ostream &dump (std::ostream &out_stream) const;

    public:

       // It goes through the matrix column-by-column starting at [0, 0]
       //
        typedef typename DataVector::iterator       col_iterator;
        typedef typename DataVector::const_iterator col_const_iterator;

       // It goes through the matrix row-by-row starting at [0, 0]
       //
        class   row_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the row_iterator to be an "undefined" row_iterator
               //
                inline row_iterator () noexcept : matx_ (NULL), idx_ (0)  {   }

                inline row_iterator (SelfType *m, size_type idx = 0) noexcept
                    : matx_ (m), idx_ (idx)  {   }

                inline bool operator == (const row_iterator &rhs) const  {

                    return (matx_ == rhs.matx_ && idx_ == rhs.idx_);
                }
                inline bool operator != (const row_iterator &rhs) const  {

                    return (matx_ != rhs.matx_ || idx_ != rhs.idx_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline pointer operator -> () const noexcept  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }
                inline reference operator * () const noexcept  {

                    return (matx_->at (idx_ / matx_->columns (),
                                       idx_ % matx_->columns ()));
                }
                inline operator pointer () const noexcept  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }

               // We are following STL style iterator interface.
               //
                inline row_iterator &operator ++ () noexcept  {    // ++Prefix

                    idx_ += 1;
                    return (*this);
                }
                inline row_iterator operator ++ (int) noexcept  {  // Postfix++

                    const   size_type   ret_idx = idx_;

                    idx_ += 1;
                    return (row_iterator (matx_, ret_idx));
                }

                inline row_iterator &operator += (long i) noexcept  {

                    idx_ += i;
                    return (*this);
                }

                inline row_iterator &operator -- () noexcept  {    // --Prefix

                    idx_ -= 1;
                    return (*this);
                }
                inline row_iterator operator -- (int) noexcept  {  // Postfix--

                    const   size_type   ret_idx = idx_;

                    idx_ -= 1;
                    return (row_iterator (matx_, ret_idx));
                }

                inline row_iterator &operator -= (int i) noexcept  {

                    idx_ -= i;
                    return (*this);
                }

                inline row_iterator operator + (int i) noexcept  {

                    return (row_iterator (matx_, idx_ + i));
                }

                inline row_iterator operator - (int i) noexcept  {

                    return (row_iterator (matx_, idx_ - i));
                }

                inline row_iterator operator + (long i) noexcept  {

                    return (row_iterator (matx_, idx_ + i));
                }

                inline row_iterator operator - (long i) noexcept  {

                    return (row_iterator (matx_, idx_ - i));
                }

            private:

                SelfType    *matx_;
                size_type   idx_;

                friend  class   DenseMatrixBase::row_const_iterator;
        };

        class   row_const_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the row_const_iterator to be an "undefined"
               //       row_const_iterator
               //
                inline row_const_iterator () noexcept
                    : matx_ (NULL), idx_ (0)  {   }

                inline row_const_iterator (const SelfType *m,
                                           size_type idx = 0) noexcept
                    : matx_ (m), idx_ (idx)  {   }

                inline row_const_iterator (const row_iterator &that)  {

                    *this = that;
                }

                inline row_const_iterator &
                operator = (const row_iterator &rhs)  {

                    matx_ = rhs.matx_;
                    idx_ = rhs.idx_;
                    return (*this);
                }

                inline bool operator == (const row_const_iterator &rhs) const {

                    return (matx_ == rhs.matx_ && idx_ == rhs.idx_);
                }
                inline bool operator != (const row_const_iterator &rhs) const {

                    return (matx_ != rhs.matx_ || idx_ != rhs.idx_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline const_pointer operator -> () const noexcept  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }
                inline const_reference operator * () const noexcept  {

                    return (matx_->at (idx_ / matx_->columns (),
                                       idx_ % matx_->columns ()));
                }
                inline operator const_pointer () const noexcept  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }

               // ++Prefix
               //
                inline row_const_iterator &operator ++ () noexcept  {

                    idx_ += 1;
                    return (*this);
                }

               // Postfix++
               //
                inline row_const_iterator operator ++ (int) noexcept  {

                    const   size_type   ret_idx = idx_;

                    idx_ += 1;
                    return (row_const_iterator (matx_, ret_idx));
                }

                inline row_const_iterator &operator += (long i) noexcept  {

                    idx_ += i;
                    return (*this);
                }

               // --Prefix
               //
                inline row_const_iterator &operator -- () noexcept  {

                    idx_ -= 1;
                    return (*this);
                }

               // Postfix--
               //
                inline row_const_iterator operator -- (int) noexcept  {

                    const   size_type   ret_idx = idx_;

                    idx_ -= 1;
                    return (row_const_iterator (matx_, ret_idx));
                }

                inline row_const_iterator &operator -= (int i) noexcept  {

                    idx_ -= i;
                    return (*this);
                }

                inline row_const_iterator operator + (int i) noexcept  {

                    return (row_const_iterator (matx_, idx_ + i));
                }

                inline row_const_iterator operator - (int i) noexcept  {

                    return (row_const_iterator (matx_, idx_ - i));
                }

                inline row_const_iterator operator + (long i) noexcept  {

                    return (row_const_iterator (matx_, idx_ + i));
                }

                inline row_const_iterator operator - (long i) noexcept  {

                    return (row_const_iterator (matx_, idx_ - i));
                }

            private:

                const   SelfType    *matx_;
                size_type           idx_;
        };

        inline col_iterator col_begin () noexcept  {

            return (BaseClass::_get_data ().begin ());
        }
        inline col_const_iterator col_begin () const noexcept  {

            return (BaseClass::_get_data ().begin ());
        }
        inline col_iterator col_end () noexcept  {

            return (BaseClass::_get_data ().end ());
        }
        inline col_const_iterator col_end () const noexcept  {

            return (BaseClass::_get_data ().end ());
        }

        inline row_iterator row_begin () noexcept  {

            return (row_iterator (this));
        }
        inline row_const_iterator row_begin () const noexcept  {

            return (row_const_iterator (this));
        }
        inline row_iterator row_end () noexcept  {

            return (row_iterator (this, BaseClass::_get_data ().size ()));
        }
        inline row_const_iterator row_end () const noexcept  {

            return (row_const_iterator (this, BaseClass::_get_data().size ()));
        }
};

} // namespace hmma

// ----------------------------------------------------------------------------

#  ifdef DMS_INCLUDE_SOURCE
#    include <DenseMatrixBase.tcc>
#  endif // DMS_INCLUDE_SOURCE

// ----------------------------------------------------------------------------

#undef _INCLUDED_DenseMatrixBase_h
#define _INCLUDED_DenseMatrixBase_h 1
#endif  // _INCLUDED_DenseMatrixBase_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
