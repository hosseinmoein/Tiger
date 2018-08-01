// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein

#ifndef _INCLUDED_DMScu_VectorRange_h
#define _INCLUDED_DMScu_VectorRange_h 0

#include <cstdlib>
#include <iterator>

// ----------------------------------------------------------------------------

// This is an adopter that would make an already existing array of contiuous
// memeory look like an STL vector.
// It also gives you STL conformant iterators.
//
template <class cu_TYPE>
class   DMScu_VectorRange  {

    public:

        typedef cu_TYPE                 value_type;
        typedef unsigned long long int  size_type;
        typedef value_type *            pointer;
        typedef const value_type *      const_pointer;
        typedef const value_type *const const_pointer_const;
        typedef value_type &            reference;
        typedef const value_type &      const_reference;

        static  const   size_type   value_size = sizeof (value_type);

    public:

        inline DMScu_VectorRange () noexcept
            : begin_ptr_ (NULL), end_ptr_ (NULL)  {   }
        inline DMScu_VectorRange (value_type *bp, value_type *ep) noexcept
            : begin_ptr_ (bp), end_ptr_ (ep)  {   }

        inline bool empty () const noexcept  {

            return (begin_ptr_ == end_ptr_);
        }
        inline size_type size () const noexcept  {

            return (begin_ptr_
                        ? static_cast<size_type>(end_ptr_ - begin_ptr_) + 1
                        : 0);
        }
        inline void clear () noexcept  { begin_ptr_ = end_ptr_ = NULL; }

        inline reference operator [] (size_type i) noexcept  {

            return (*(begin_ptr_ + i));
        }
        inline const_reference operator [] (size_type i) const noexcept  {

            return (*(begin_ptr_ + i));
        }
        inline reference front () noexcept  { return (*begin_ptr_); }
        inline const_reference front () const noexcept  {

            return (*begin_ptr_);
        }
        inline reference back () noexcept  { return (*end_ptr_); }
        inline const_reference back () const noexcept  { return (*end_ptr_); }

        inline void swap (DMScu_VectorRange &rhs) noexcept  {

            std::swap (begin_ptr_, rhs.begin_ptr_);
            std::swap (end_ptr_, rhs.end_ptr_);
            return;
        }

    private:

        value_type  *begin_ptr_;
        value_type  *end_ptr_;

    public:

        class const_iterator;
        class   const_reverse_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type const, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be the "end" iterator
               //
                inline const_reverse_iterator () noexcept : node_ (NULL)  {   }

                inline const_reverse_iterator (value_type const *const node)
                    noexcept : node_ (node)  {   }

                inline const_reverse_iterator (const const_iterator &itr)
                    noexcept : node_ (NULL)  {

                    *this = itr;
                }

                inline const_reverse_iterator &
                operator = (const const_iterator &rhs) noexcept  {

                    node_ = rhs.node_;
                    return (*this);
                }

                inline bool
                operator == (const const_reverse_iterator &rhs) const noexcept{

                    return (node_ == rhs.node_);
                }
                inline bool
                operator != (const const_reverse_iterator &rhs) const noexcept{

                    return (node_ != rhs.node_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline const_pointer operator -> () const noexcept  {

                    return (node_);
                }
                inline const_reference operator *() const noexcept  {

                    return (*node_);
                }
                inline operator pointer () const noexcept  { return (node_); }

               // ++Prefix
               //
                inline const_reverse_iterator &operator ++ () noexcept  {

                    node_ -= 1;
                    return (*this);
                }

               // Postfix++
               //
                inline const_reverse_iterator operator ++ (int) noexcept  {

                    value_type   const  *ret_node = node_;

                    node_ -= 1;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator &
                operator += (int step) noexcept  {

                    node_ -= step;
                    return (*this);
                }

               // --Prefix
               //
                inline const_reverse_iterator &operator -- () noexcept  {

                    node_ += 1;
                    return (*this);
                }

               // Postfix--
               //
                inline const_reverse_iterator operator -- (int) noexcept  {

                    value_type   const  *ret_node = node_;

                    node_ += 1;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator &
                operator -= (int step) noexcept  {

                    node_ += step;
                    return (*this);
                }

                inline const_reverse_iterator operator + (int step) noexcept  {

                    value_type   const  *ret_node = node_;

                    ret_node -= step;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator operator - (int step) noexcept  {

                    value_type   const  *ret_node = node_;

                    ret_node += step;
                    return (const_reverse_iterator (ret_node));
                }

#ifndef _WIN64
                inline const_reverse_iterator
                operator + (long step) noexcept  {

                    value_type   const  *ret_node = node_;

                    ret_node -= step;
                    return (const_reverse_iterator (ret_node));
				}
#endif

                inline const_reverse_iterator
                operator - (long step) noexcept  {

                    value_type   const  *ret_node = node_;

                    ret_node += step;
                    return (const_reverse_iterator (ret_node));
                }

            private:

                const_pointer   node_;
        };
        
        class iterator;
        class   const_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type const, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be the "end" iterator
               //
                inline const_iterator () noexcept : node_ (NULL)  {   }

                inline const_iterator (value_type const *const node) noexcept
                    : node_ (node)  {   }

                inline const_iterator (const iterator &itr) noexcept
                    : node_ (NULL)  {

                    *this = itr;
                }

                inline const_iterator &operator = (const iterator &rhs)
                    noexcept  {

                    node_ = rhs.node_;
                    return (*this);
                }

                inline bool
                operator == (const const_iterator &rhs) const noexcept  {

                    return (node_ == rhs.node_);
                }
                inline bool
                operator != (const const_iterator &rhs) const noexcept  {

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

                    node_ += 1;
                    return (*this);
                }

               // Postfix++
               //
                inline const_iterator operator ++ (int) noexcept  {

                    value_type   const  *ret_node = node_;

                    node_ += 1;
                    return (const_iterator (ret_node));
                }

                inline const_iterator &operator += (int step) noexcept  {

                    node_ += step;
                    return (*this);
                }

               // --Prefix
               //
                inline const_iterator &operator -- () noexcept  {

                    node_ -= 1;
                    return (*this);
                }

               // Postfix--
               //
                inline const_iterator operator -- (int) noexcept  {

                    value_type  const  *ret_node = node_;

                    node_ -= 1;
                    return (const_iterator (ret_node));
                }

                inline const_iterator &operator -= (int step) noexcept  {

                    node_ -= step;
                    return (*this);
                }

#ifndef _WIN64
                inline const_iterator operator + (int step) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node += step;
                    return (const_iterator (ret_node));
                }
#endif

                inline const_iterator operator - (int step) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node -= step;
                    return (const_iterator (ret_node));
                }

#ifndef _WIN64
                inline const_iterator operator + (long step) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node += step;
                    return (const_iterator (ret_node));
                }
#endif

                inline const_iterator operator - (long step) noexcept  {

                    value_type  const  *ret_node = node_;

                    ret_node -= step;
                    return (const_iterator (ret_node));
                }

            private:

                const_pointer   node_;

                friend  class   DMScu_VectorRange::const_reverse_iterator;
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
               //       the iterator to be the "end" iterator
               //
                inline iterator () noexcept : node_ (NULL)  {   }

                inline iterator (value_type *node) noexcept
                    : node_ (node)  {  }

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

                    node_ += 1;
                    return (*this);
                }
                inline iterator operator ++ (int) noexcept  {  // Postfix++

                    value_type   *ret_node = node_;

                    node_ += 1;
                    return (iterator (ret_node));
                }

                inline iterator &operator += (long step) noexcept  {

                    node_ += step;
                    return (*this);
                }

                inline iterator &operator -- () noexcept  {    // --Prefix

                    node_ -= 1;
                    return (*this);
                }
                inline iterator operator -- (int) noexcept  {  // Postfix--

                    value_type   *ret_node = node_;

                    node_ -= 1;
                    return (iterator (ret_node));
                }

                inline iterator &operator -= (int step) noexcept  {

                    node_ -= step;
                    return (*this);
                }

                inline iterator operator + (int step) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node += step;
                    return (iterator (ret_node));
                }

                inline iterator operator - (int step) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node -= step;
                    return (iterator (ret_node));
                }

#ifndef _WIN64
                inline iterator operator + (long step) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node += step;
                    return (iterator (ret_node));
                }
#endif

                inline iterator operator - (long step) noexcept  {

                    value_type   *ret_node = node_;

                    ret_node -= step;
                    return (iterator (ret_node));
                }

            private:

                pointer node_;

                friend  class   DMScu_VectorRange::const_iterator;
        };

    public:

        inline iterator begin () noexcept  {

            return (empty () ? end () : iterator (begin_ptr_));
        }
        inline iterator end () noexcept  { return (iterator (end_ptr_ + 1)); }
        inline const_iterator begin () const noexcept  {

            return (empty () ? end () : const_iterator (begin_ptr_));
        }
        inline const_iterator end () const noexcept  {

            return (const_iterator (end_ptr_ + 1));
        }

        inline const_reverse_iterator rbegin () const noexcept  {

            return (empty () ? rend () : const_iterator (end_ptr_));
        }
        inline const_reverse_iterator rend () const noexcept  {

            return (const_iterator (begin_ptr_ - 1));
        }

       // This is required to support the C++ template expressions
       //
        template<class cu_EXPR>
        inline DMScu_VectorRange &
        operator = (const cu_EXPR &rhs) noexcept  {

            rhs.assign (*this);
            return (*this);
        }
};

// ----------------------------------------------------------------------------

#undef _INCLUDED_DMScu_VectorRange_h
#define _INCLUDED_DMScu_VectorRange_h 1
#endif  // _INCLUDED_DMScu_VectorRange_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
