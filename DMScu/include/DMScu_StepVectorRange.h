// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein

#ifndef _INCLUDED_DMScu_StepVectorRange_h
#define _INCLUDED_DMScu_StepVectorRange_h 0

#include <cstdlib>
#include <vector>

#include <DMScu_Exception.h>

// ----------------------------------------------------------------------------

// This is an adopter that would make an already existing array of contiuous
// memeory look like an STL vector.
// NOTE: This class iterates over the voctor _not_ _one_ _element_ _at_ _a_
//       _time_, but step elements at a time.
//
template <class cu_TYPE>
class   DMScu_StepVectorRange  {

    public:

        typedef cu_TYPE                 value_type;
        typedef unsigned    int         size_type;
        typedef value_type *            pointer;
        typedef const value_type *      const_pointer;
        typedef const value_type *const const_pointer_const;
        typedef value_type &            reference;
        typedef const value_type &      const_reference;

        static  const   size_type   value_size = sizeof (value_type);

    public:

        inline DMScu_StepVectorRange () noexcept
            : begin_ptr_ (NULL), end_ptr_ (NULL), step_size_ (0)  {   }
        inline DMScu_StepVectorRange (value_type *bp,
                                      value_type *ep,
                                      size_type step)
            : begin_ptr_ (bp), end_ptr_ (ep), step_size_ (step)  {

            // std::cout <<
            // ((reinterpret_cast<unsigned long long int>(end_ptr_) -
            //       reinterpret_cast<unsigned long long int>(begin_ptr_)) /
            //      sizeof (value_type)) << "\n"
            //           << step_size_ << std::endl;

            if (((reinterpret_cast<unsigned long long int>(end_ptr_) -
                  reinterpret_cast<unsigned long long int>(begin_ptr_)) /
                 sizeof (value_type)) % step_size_)
                throw DMScu_Exception ("DMScu_StepVectorRange::"
                                       "DMScu_StepVectorRange(): "
                                       "The vector range isn't a multiple "
                                       "of steps");
        }

        inline bool empty () const noexcept  {

            return (step_size_ == 0 ||
                    (reinterpret_cast<unsigned long long int>(end_ptr_) -
                     reinterpret_cast<unsigned long long int>(begin_ptr_)) /
                    sizeof (value_type) <= step_size_);
        }
        inline size_type size () const noexcept  {

            return (step_size_ == 0
                    ? 0
                    : (((reinterpret_cast<unsigned long long int>(end_ptr_) -
                        reinterpret_cast<unsigned long long int>(begin_ptr_)) /
                       sizeof (value_type)) /
                      step_size_) + 1);
        }
        inline void clear () noexcept  {

            begin_ptr_ = end_ptr_ = NULL;
            step_size_ = 0;
        }

        inline reference operator [] (size_type i) noexcept  {

            return (*(begin_ptr_ + i * step_size_));
        }
        inline const_reference operator [] (size_type i) const noexcept  {

            return (*(begin_ptr_ + i * step_size_));
        }
        inline reference front () noexcept  { return (*begin_ptr_); }
        inline const_reference front () const noexcept  {

            return (*begin_ptr_);
        }
        inline reference back () noexcept  { return (*end_ptr_); }
        inline const_reference back () const noexcept  { return (*end_ptr_); }

        inline void swap (DMScu_StepVectorRange &rhs) noexcept  {

            std::swap (begin_ptr_, rhs.begin_ptr_);
            std::swap (end_ptr_, rhs.end_ptr_);
            std::swap (step_size_, rhs.step_size_);
            return;
        }

    private:

        value_type  *begin_ptr_;
        value_type  *end_ptr_;
        size_type   step_size_;

    public:

        class   const_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type const, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be an "undefined" iterator
               //
                inline const_iterator () noexcept
                    : node_ (NULL), step_ (0)  {   }

                inline const_iterator (value_type const *node,
                                       size_type step) noexcept
                    : node_ (node), step_ (step)  {   }

                inline const_iterator (const const_iterator &that) noexcept  {

                    *this = that;
                }

                inline const_iterator (const iterator &itr) noexcept
                    : node_ (NULL)  {

                    *this = itr;
                }

                inline const_iterator &operator = (const const_iterator &rhs)
                    noexcept  {

                    if (this != &rhs)  {
                        node_ = rhs.node_;
                        step_ = rhs.step_;
                    }

                    return (*this);
                }

                inline const_iterator &operator = (const iterator &rhs)
                    noexcept  {

                    node_ = rhs.node_;
                    step_ = rhs.step_;
                    return (*this);
                }

                inline bool operator == (const const_iterator &rhs)
                    const noexcept  {

                    return (node_ == rhs.node_);
                }
                inline bool operator != (const const_iterator &rhs)
                    const noexcept  {

                    return (node_ != rhs.node_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline const_pointer operator -> () const noexcept  {

                    return (node_);
                }
                inline const_reference operator * () const noexcept  {

                    return (*node_);
                }
                inline operator const_pointer () const noexcept  {

                    return (node_);
                }

               // ++Prefix
               //
                inline const_iterator &operator ++ () noexcept  {

                    node_ += step_;
                    return (*this);
                }

               // Postfix++
               //
                inline const_iterator operator ++ (int) noexcept  {

                    value_type   const  *ret_node = node_;

                    node_ += step_;
                    return (const_iterator (ret_node, step_));
                }

                inline const_iterator &operator += (int i) noexcept  {

                    node_ += i * step_;
                    return (*this);
                }

               // --Prefix
               //
                inline const_iterator &operator -- () noexcept  {

                    node_ -= step_;
                    return (*this);
                }

               // Postfix--
               //
                inline const_iterator operator -- (int) noexcept  {

                    value_type  const  *ret_node = node_;

                    node_ -= step_;
                    return (const_iterator (ret_node, step_));
                }

                inline const_iterator &operator -= (int i) noexcept  {

                    node_ -= i * step_;
                    return (*this);
                }

                inline const_iterator operator + (int i) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node += i * step_;
                    return (const_iterator (ret_node, step_));
                }

                inline const_iterator operator - (int i) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node -= i * step_;
                    return (const_iterator (ret_node, step_));
                }

                inline const_iterator operator + (long i) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node += i * step_;
                    return (const_iterator (ret_node, step_));
                }

                inline const_iterator operator - (long i) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node -= i * step_;
                    return (const_iterator (ret_node, step_));
                }

            private:

                const_pointer   node_;
                size_type       step_;
        };

       // This iterator contains only one pointer. Like STL iterators,
       // it is cheap to create and copy around.
       //
        class   iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be an "undefined" iterator
               //
                inline iterator () noexcept : node_ (NULL), step_ (0)  {   }

                inline iterator (value_type *node, size_type step) noexcept
                    : node_ (node), step_ (step)  {  }

                inline iterator (const iterator &that) noexcept  {

                    *this = that;
                }

                inline iterator &operator = (const iterator &rhs) noexcept  {

                    if (this != &rhs)  {
                        node_ = rhs.node_;
                        step_ = rhs.step_;
                    }

                    return (*this);
                }

                inline bool operator == (const iterator &rhs) const noexcept  {

                    return (node_ == rhs.node_);
                }
                inline bool operator != (const iterator &rhs) const noexcept  {

                    return (node_ != rhs.node_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline pointer operator -> () const noexcept  {

                    return (node_);
                }
                inline reference operator * () const noexcept  {

                    return (*node_);
                }
                inline operator pointer () const noexcept  { return (node_); }

               // We are following STL style iterator interface.
               //
                inline iterator &operator ++ () noexcept  {    // ++Prefix

                    node_ += step_;
                    return (*this);
                }
                inline iterator operator ++ (int) noexcept  {  // Postfix++

                    value_type   *ret_node = node_;

                    node_ += step_;
                    return (iterator (ret_node, step_));
                }

                inline iterator &operator += (long i) noexcept  {

                    node_ += i * step_;
                    return (*this);
                }

                inline iterator &operator -- () noexcept  {    // --Prefix

                    node_ -= step_;
                    return (*this);
                }
                inline iterator operator -- (int) noexcept  {  // Postfix--

                    value_type   *ret_node = node_;

                    node_ -= step_;
                    return (iterator (ret_node, step_));
                }

                inline iterator &operator -= (int i) noexcept  {

                    node_ -= i * step_;
                    return (*this);
                }

                inline iterator operator + (int i) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node += i * step_;
                    return (iterator (ret_node, step_));
                }

                inline iterator operator - (int i) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node -= i * step_;
                    return (iterator (ret_node, step_));
                }

                inline iterator operator + (long i) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node += i * step_;
                    return (iterator (ret_node, step_));
                }

                inline iterator operator - (long i) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node -= i * step_;
                    return (iterator (ret_node, step_));
                }

            private:

                pointer     node_;
                size_type   step_;

                friend  class   DMScu_StepVectorRange::const_iterator;
        };


    public:

        inline iterator begin () noexcept  {

            return (empty () ? end () : iterator (begin_ptr_, step_size_));
        }
        inline iterator end () noexcept  {

            return (iterator (end_ptr_ + step_size_, step_size_));
        }
        inline const_iterator begin () const noexcept  {

            return (empty ()
                        ? end () : const_iterator (begin_ptr_, step_size_));
        }
        inline const_iterator end () const noexcept  {

            return (const_iterator (end_ptr_ + step_size_, step_size_));
        }

       // This is required to support the C++ template expressions
       //
        template<class cu_EXPR>
        inline DMScu_StepVectorRange &operator = (const cu_EXPR &rhs)
            noexcept  {

            rhs.assign (*this);
            return (*this);
        }
};

// ----------------------------------------------------------------------------

#undef _INCLUDED_DMScu_StepVectorRange_h
#define _INCLUDED_DMScu_StepVectorRange_h 1
#endif  // _INCLUDED_DMScu_StepVectorRange_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
