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

#include <iostream>

#include <Tiger/VectorRange.h>
#include <Tiger/StepVectorRange.h>

#include <Tiger/MatrixBase.h>

// ----------------------------------------------------------------------------

namespace hmma
{

template<class T>
class   SymmMatrixBase : public DenseMatrixStorage<T>  {

    public:

        typedef DenseMatrixStorage<T>               BaseClass;

        typedef typename BaseClass::size_type       size_type;
        typedef typename BaseClass::value_type      value_type;
        typedef typename BaseClass::reference       reference;
        typedef typename BaseClass::const_reference const_reference;
        typedef typename BaseClass::pointer         pointer;
        typedef typename BaseClass::const_pointer   const_pointer;

        typedef SymmMatrixBase<value_type>          SelfType;

    protected:

        inline SymmMatrixBase () noexcept  {   }

        inline
        SymmMatrixBase (size_type row,
                        size_type col,
                        const_reference def_value = value_type ())
            // throw (NotSquare)
            : BaseClass (row, col, (col * (col + 1)) / 2, def_value)  {

            if (row != col)
                throw NotSquare ();
        }

        static inline bool _is_symmetric_matrix () noexcept { return (true); }

    public:

        void resize (size_type in_row,
                     size_type in_col, 
                     const_reference def_value = value_type ());

        inline reference at (size_type r, size_type c) noexcept;
        inline const_reference at (size_type r, size_type c) const noexcept;

        std::ostream &dump (std::ostream &out_stream) const;

    public:

        class   iterator  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be an "undefined" iterator
               //
                inline iterator () noexcept : matx_ (NULL), idx_ (0)  {   }

                inline iterator (SelfType *m, size_type idx = 0) noexcept
                    : matx_ (m), idx_ (idx)  {   }

                inline bool operator == (const iterator &rhs) const  {

                    return (matx_ == rhs.matx_ && idx_ == rhs.idx_);
                }
                inline bool operator != (const iterator &rhs) const  {

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
                inline iterator &operator ++ () noexcept  {    // ++Prefix

                    idx_ += 1;
                    return (*this);
                }
                inline iterator operator ++ (int) noexcept  {  // Postfix++

                    const   size_type   ret_idx = idx_;

                    idx_ += 1;
                    return (iterator (matx_, ret_idx));
                }

                inline iterator &operator += (long i) noexcept  {

                    idx_ += i;
                    return (*this);
                }

                inline iterator &operator -- () noexcept  {    // --Prefix

                    idx_ -= 1;
                    return (*this);
                }
                inline iterator operator -- (int) noexcept  {  // Postfix--

                    const   size_type   ret_idx = idx_;

                    idx_ -= 1;
                    return (iterator (matx_, ret_idx));
                }

                inline iterator &operator -= (int i) noexcept  {

                    idx_ -= i;
                    return (*this);
                }

                inline iterator operator + (int i) noexcept  {

                    return (iterator (matx_, idx_ + i));
                }

                inline iterator operator - (int i) noexcept  {

                    return (iterator (matx_, idx_ - i));
                }

                inline iterator operator + (long i) noexcept  {

                    return (iterator (matx_, idx_ + i));
                }

                inline iterator operator - (long i) noexcept  {

                    return (iterator (matx_, idx_ - i));
                }

            private:

                SelfType    *matx_;
                size_type   idx_;

                friend  class   SymmMatrixBase::const_iterator;
        };

        class   const_iterator  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the const_iterator to be an "undefined"
               //       const_iterator
               //
                inline const_iterator () noexcept
                    : matx_ (NULL), idx_ (0)  {   }

                inline const_iterator (const SelfType *m, 
                                       size_type idx = 0) noexcept
                    : matx_ (m), idx_ (idx)  {   }

                inline const_iterator (
                    const typename SelfType::iterator &that)  {

                    *this = that;
                }

                inline const_iterator &operator = (
                    const typename SelfType::iterator &rhs)  {

                    matx_ = rhs.matx_;
                    idx_ = rhs.idx_;
                    return (*this);
                }

                inline bool operator == (const const_iterator &rhs) const {

                    return (matx_ == rhs.matx_ && idx_ == rhs.idx_);
                }
                inline bool operator != (const const_iterator &rhs) const {

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
                inline const_iterator &operator ++ () noexcept  {

                    idx_ += 1;
                    return (*this);
                }

               // Postfix++
               //
                inline const_iterator operator ++ (int) noexcept  {

                    const   size_type   ret_idx = idx_;

                    idx_ += 1;
                    return (const_iterator (matx_, ret_idx));
                }
                inline const_iterator &operator += (long i) noexcept  {

                    idx_ += i;
                    return (*this);
                }

               // --Prefix
               //
                inline const_iterator &operator -- () noexcept  {

                    idx_ -= 1;
                    return (*this);
                }

               // Postfix--
               //
                inline const_iterator operator -- (int) noexcept  {

                    const   size_type   ret_idx = idx_;

                    idx_ -= 1;
                    return (const_iterator (matx_, ret_idx));
                }

                inline const_iterator &operator -= (int i) noexcept  {

                    idx_ -= i;
                    return (*this);
                }

                inline const_iterator operator + (int i) noexcept  {

                    return (const_iterator (matx_, idx_ + i));
                }

                inline const_iterator operator - (int i) noexcept  {

                    return (const_iterator (matx_, idx_ - i));
                }

                inline const_iterator operator + (long i) noexcept  {

                    return (const_iterator (matx_, idx_ + i));
                }

                inline const_iterator operator - (long i) noexcept  {

                    return (const_iterator (matx_, idx_ - i));
                }

            private:

                const   SelfType    *matx_;
                size_type           idx_;
        };

        typedef iterator        row_iterator;
        typedef iterator        col_iterator;
        typedef const_iterator  row_const_iterator;
        typedef const_iterator  col_const_iterator;

        inline col_iterator col_begin () noexcept  {

            return (col_iterator (this));
        }
        inline col_const_iterator col_begin () const noexcept  {

            return (col_const_iterator (this));
        }
        inline col_iterator col_end () noexcept  {

            return (col_iterator (this,
                                  BaseClass::rows () * BaseClass::columns ()));
        }
        inline col_const_iterator col_end () const noexcept  {

            return (col_const_iterator (
                        this,
                        BaseClass::rows () * BaseClass::columns ()));
        }

        inline row_iterator row_begin () noexcept  {

            return (row_iterator (this));
        }
        inline row_const_iterator row_begin () const noexcept  {

            return (row_const_iterator (this));
        }
        inline row_iterator row_end () noexcept  {

            return (row_iterator (this,
                                  BaseClass::rows () * BaseClass::columns ()));
        }
        inline row_const_iterator row_end () const noexcept  {

            return (row_const_iterator (
                        this,
                        BaseClass::rows () * BaseClass::columns ()));
        }
};

} // namespace hmma

// ----------------------------------------------------------------------------

#  ifdef DMS_INCLUDE_SOURCE
#    include <Tiger/SymmMatrixBase.tcc>
#  endif // DMS_INCLUDE_SOURCE

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
