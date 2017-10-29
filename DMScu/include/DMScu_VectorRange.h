// Hossein Moein
// July 17 2009

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

        inline DMScu_VectorRange () throw ()
            : begin_ptr_ (NULL), end_ptr_ (NULL)  {   }
        inline DMScu_VectorRange (value_type *bp, value_type *ep) throw ()
            : begin_ptr_ (bp), end_ptr_ (ep)  {   }

        inline bool empty () const throw ()  {

            return (begin_ptr_ == end_ptr_);
        }
        inline size_type size () const throw ()  {

            return (begin_ptr_
                        ? static_cast<size_type>(end_ptr_ - begin_ptr_) + 1
                        : 0);
        }
        inline void clear () throw ()  { begin_ptr_ = end_ptr_ = NULL; }

        inline reference operator [] (size_type i) throw ()  {

            return (*(begin_ptr_ + i));
        }
        inline const_reference operator [] (size_type i) const throw ()  {

            return (*(begin_ptr_ + i));
        }
        inline reference front () throw ()  { return (*begin_ptr_); }
        inline const_reference front () const throw ()  {

            return (*begin_ptr_);
        }
        inline reference back () throw ()  { return (*end_ptr_); }
        inline const_reference back () const throw ()  { return (*end_ptr_); }

        inline void swap (DMScu_VectorRange &rhs) throw ()  {

            std::swap (begin_ptr_, rhs.begin_ptr_);
            std::swap (end_ptr_, rhs.end_ptr_);
            return;
        }

    private:

        value_type  *begin_ptr_;
        value_type  *end_ptr_;

    public:

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
                inline iterator () throw () : node_ (NULL)  {   }

                inline iterator (value_type *node) throw ()
                    : node_ (node)  {  }

                inline bool operator == (const iterator &rhs) const throw ()  {

                    return (node_ == rhs.node_);
                }
                inline bool operator != (const iterator &rhs) const throw ()  {

                    return (node_ != rhs.node_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline pointer operator -> () const throw ()  {

                    return (node_);
                }
                inline reference operator * () const throw ()  {

                    return (*node_);
                }
                inline operator pointer () const throw ()  { return (node_); }

               // We are following STL style iterator interface.
               //
                inline iterator &operator ++ () throw ()  {    // ++Prefix

                    node_ += 1;
                    return (*this);
                }
                inline iterator operator ++ (int) throw ()  {  // Postfix++

                    value_type   *ret_node = node_;

                    node_ += 1;
                    return (iterator (ret_node));
                }

                inline iterator &operator += (long step) throw ()  {

                    node_ += step;
                    return (*this);
                }

                inline iterator &operator -- () throw ()  {    // --Prefix

                    node_ -= 1;
                    return (*this);
                }
                inline iterator operator -- (int) throw ()  {  // Postfix--

                    value_type   *ret_node = node_;

                    node_ -= 1;
                    return (iterator (ret_node));
                }

                inline iterator &operator -= (int step) throw ()  {

                    node_ -= step;
                    return (*this);
                }

                inline iterator operator + (int step) throw ()  {

                    value_type   *ret_node = node_;

                    ret_node += step;
                    return (iterator (ret_node));
                }

                inline iterator operator - (int step) throw ()  {

                    value_type   *ret_node = node_;

                    ret_node -= step;
                    return (iterator (ret_node));
                }

                inline iterator operator + (long step) throw ()  {

                    value_type   *ret_node = node_;

                    ret_node += step;
                    return (iterator (ret_node));
                }

                inline iterator operator - (long step) throw ()  {

                    value_type   *ret_node = node_;

                    ret_node -= step;
                    return (iterator (ret_node));
                }

            private:

                pointer node_;

                friend  class   DMScu_VectorRange::const_iterator;
        };

        class   const_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type const, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be the "end" iterator
               //
                inline const_iterator () throw () : node_ (NULL)  {   }

                inline const_iterator (value_type const *const node) throw ()
                    : node_ (node)  {   }

                inline const_iterator (const iterator &itr) throw ()
                    : node_ (NULL)  {

                    *this = itr;
                }

                inline const_iterator &operator = (const iterator &rhs)
                    throw ()  {

                    node_ = rhs.node_;
                    return (*this);
                }

                inline bool
                operator == (const const_iterator &rhs) const throw ()  {

                    return (node_ == rhs.node_);
                }
                inline bool
                operator != (const const_iterator &rhs) const throw ()  {

                    return (node_ != rhs.node_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline const_pointer operator -> () const throw ()  {

                    return (node_);
                }
                inline const_reference operator * () const throw ()  {

                    return (*node_);
                }
                inline operator const_pointer () const throw ()  {

                    return (node_);
                }

               // ++Prefix
               //
                inline const_iterator &operator ++ () throw ()  {

                    node_ += 1;
                    return (*this);
                }

               // Postfix++
               //
                inline const_iterator operator ++ (int) throw ()  {

                    value_type   const  *ret_node = node_;

                    node_ += 1;
                    return (const_iterator (ret_node));
                }

                inline const_iterator &operator += (int step) throw ()  {

                    node_ += step;
                    return (*this);
                }

               // --Prefix
               //
                inline const_iterator &operator -- () throw ()  {

                    node_ -= 1;
                    return (*this);
                }

               // Postfix--
               //
                inline const_iterator operator -- (int) throw ()  {

                    value_type  const  *ret_node = node_;

                    node_ -= 1;
                    return (const_iterator (ret_node));
                }

                inline const_iterator &operator -= (int step) throw ()  {

                    node_ -= step;
                    return (*this);
                }

                inline const_iterator operator + (int step) throw ()  {

                    value_type  const  *ret_node = node_;

                    ret_node += step;
                    return (const_iterator (ret_node));
                }

                inline const_iterator operator - (int step) throw ()  {

                    value_type  const  *ret_node = node_;

                    ret_node -= step;
                    return (const_iterator (ret_node));
                }

                inline const_iterator operator + (long step) throw ()  {

                    value_type  const  *ret_node = node_;

                    ret_node += step;
                    return (const_iterator (ret_node));
                }

                inline const_iterator operator - (long step) throw ()  {

                    value_type  const  *ret_node = node_;

                    ret_node -= step;
                    return (const_iterator (ret_node));
                }

            private:

                const_pointer   node_;

                friend  class   DMScu_VectorRange::const_reverse_iterator;
        };

        class   const_reverse_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type const, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be the "end" iterator
               //
                inline const_reverse_iterator () throw () : node_ (NULL)  {   }

                inline const_reverse_iterator (value_type const *const node)
                    throw () : node_ (node)  {   }

                inline const_reverse_iterator (const const_iterator &itr)
                    throw () : node_ (NULL)  {

                    *this = itr;
                }

                inline const_reverse_iterator &
                operator = (const const_iterator &rhs) throw ()  {

                    node_ = rhs.node_;
                    return (*this);
                }

                inline bool
                operator == (const const_reverse_iterator &rhs) const throw() {

                    return (node_ == rhs.node_);
                }
                inline bool
                operator != (const const_reverse_iterator &rhs) const throw() {

                    return (node_ != rhs.node_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline const_pointer operator -> () const throw ()  {

                    return (node_);
                }
                inline const_reference operator *() const throw ()  {

                    return (*node_);
                }
                inline operator pointer () const throw ()  { return (node_); }

               // ++Prefix
               //
                inline const_reverse_iterator &operator ++ () throw ()  {

                    node_ -= 1;
                    return (*this);
                }

               // Postfix++
               //
                inline const_reverse_iterator operator ++ (int) throw ()  {

                    value_type   const  *ret_node = node_;

                    node_ -= 1;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator &
                operator += (int step) throw ()  {

                    node_ -= step;
                    return (*this);
                }

               // --Prefix
               //
                inline const_reverse_iterator &operator -- () throw ()  {

                    node_ += 1;
                    return (*this);
                }

               // Postfix--
               //
                inline const_reverse_iterator operator -- (int) throw ()  {

                    value_type   const  *ret_node = node_;

                    node_ += 1;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator &
                operator -= (int step) throw ()  {

                    node_ += step;
                    return (*this);
                }

                inline const_reverse_iterator operator + (int step) throw ()  {

                    value_type   const  *ret_node = node_;

                    ret_node -= step;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator operator - (int step) throw ()  {

                    value_type   const  *ret_node = node_;

                    ret_node += step;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator
                operator + (long step) throw ()  {

                    value_type   const  *ret_node = node_;

                    ret_node -= step;
                    return (const_reverse_iterator (ret_node));
                }

                inline const_reverse_iterator
                operator - (long step) throw ()  {

                    value_type   const  *ret_node = node_;

                    ret_node += step;
                    return (const_reverse_iterator (ret_node));
                }

            private:

                const_pointer   node_;
        };

    public:

        inline iterator begin () throw ()  {

            return (empty () ? end () : iterator (begin_ptr_));
        }
        inline iterator end () throw ()  { return (iterator (end_ptr_ + 1)); }
        inline const_iterator begin () const throw ()  {

            return (empty () ? end () : const_iterator (begin_ptr_));
        }
        inline const_iterator end () const throw ()  {

            return (const_iterator (end_ptr_ + 1));
        }

        inline const_reverse_iterator rbegin () const throw ()  {

            return (empty () ? rend () : const_iterator (end_ptr_));
        }
        inline const_reverse_iterator rend () const throw ()  {

            return (const_iterator (begin_ptr_ - 1));
        }

       // This is required to support the C++ template expressions
       //
        template<class cu_EXPR>
        inline DMScu_VectorRange &
        operator = (const cu_EXPR &rhs) throw ()  {

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
