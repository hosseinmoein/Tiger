// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein

#include <iostream>
#include <algorithm>
#include <vector>

#include <DMScu_VectorRange.h>
#include <DMScu_StepVectorRange.h>

// ----------------------------------------------------------------------------

typedef std::vector<int>        StdIntVecotr;
typedef DMScu_VectorRange<int>  MyIntVector;

// ----------------------------------------------------------------------------

int main (int argCnt, char *argVctr [])  {

    StdIntVecotr    std_vec;

    for (StdIntVecotr::size_type i = 0; i < 100; ++i)
        std_vec.push_back (i);

    const   MyIntVector my_vec (&(std_vec.front ()), &(std_vec.back ()));
    MyIntVector         my_vec2 = my_vec;
    const   MyIntVector my_vec3 = my_vec2;

    if (my_vec.size () == std_vec.size ())  {
        std::cout << "Both vector sizes matched." << std::endl;
    }
    else  {
        std::cout << "ERROR: std_vector size = " << std_vec.size ()
                  << " my_vector size = " << my_vec.size () << std::endl;
        return (EXIT_FAILURE);
    }

    for (MyIntVector::size_type i = 0; i < my_vec.size (); ++i)
        if (my_vec [i] != std_vec [i])  {
            std::cout << "ERROR: std_vector[" << i << "] = " << std_vec [i]
                      << " my_vector[" << i << "] = " << my_vec [i]
                      << std::endl;
            return (EXIT_FAILURE);
        }

    std::cout << "Random element checks matched." << std::endl;

    if (my_vec.back () == std_vec.back ())  {
        std::cout << "Both vector back()'s matched." << std::endl;
    }
    else  {
        std::cout << "ERROR: std_vector.back() = " << std_vec.back ()
                  << " my_vector.back() = " << my_vec.back () << std::endl;
        return (EXIT_FAILURE);
    }

    if (my_vec.front () == std_vec.front ())  {
        std::cout << "Both vector front()'s matched." << std::endl;
    }
    else  {
        std::cout << "ERROR: std_vector.front() = " << std_vec.front ()
                  << " my_vector.front() = " << my_vec.front () << std::endl;
        return (EXIT_FAILURE);
    }

    StdIntVecotr::const_iterator    std_citr = std_vec.begin ();
    MyIntVector::iterator           my_itr = my_vec2.begin ();
    MyIntVector::const_iterator     does_it_compile = my_itr;

    for (MyIntVector::const_iterator citr = my_vec.begin ();
         citr != my_vec.end (); ++citr, ++std_citr, my_itr++)
        if (*citr != *std_citr || *citr != *my_itr)  {
            std::cout << "ERROR: citr = " << *citr
                      << " std_citr = " << *std_citr
                      << " my_itr = " << *my_itr << std::endl;
            return (EXIT_FAILURE);
        }


    std::cout << "\nTesting the reverse_iterator ..." << std::endl;

    StdIntVecotr::const_reverse_iterator    std_critr = std_vec.rbegin ();

    for (MyIntVector::const_reverse_iterator critr = my_vec.rbegin ();
         critr != my_vec.rend (); ++critr, ++std_critr)
        if (*critr != *std_critr)  {
            std::cout << "ERROR: critr = " << *critr
                      << " std_critr = " << *std_critr << std::endl;
            return (EXIT_FAILURE);
        }
    std::cout << "All iterators and elementes matched." << std::endl;

    std::cout << "Now testing sorted vectors." << std::endl;
    std::sort (std_vec.begin (), std_vec.end (),
               std::greater<StdIntVecotr::value_type> ());

    for (StdIntVecotr::const_iterator citr = std_vec.begin ();
         citr != std_vec.end (); ++citr)
        std::cout << *citr << "  ";

    std::cout << std::endl;
    std::cout << std::endl;

    for (MyIntVector::const_iterator citr = my_vec.begin ();
         citr != my_vec.end (); ++citr, ++std_citr, my_itr++)
        std::cout << *citr << "  ";

    std::cout << std::endl;
    std::cout << std::endl;

    std::sort(my_vec2.begin(), my_vec2.end());

    for (MyIntVector::const_iterator citr = my_vec.begin ();
         citr != my_vec.end (); ++citr, ++std_citr, my_itr++)
        std::cout << *citr << "  ";

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "\nTesting an empty vector ..." << std::endl;

    MyIntVector empty_vec (NULL, NULL);

    if (! empty_vec.empty ())  {
        std::cout << "ERROR: The Vector must show empty." << std::endl;
        return (EXIT_FAILURE);
    }

    for (MyIntVector::const_iterator citr = empty_vec.begin ();
         citr != empty_vec.end (); ++citr)  {
        std::cout << "ERROR: I shouldn't be printing this: " << *citr
                  << std::endl;
        return (EXIT_FAILURE);
    }
    std::cout << "Empty vector works properly" << std::endl;

    std::cout << "\n\nTesting Step Vector ..." << std::endl;

    const   int         max_size = 20;
    const   int         col = 4;
    const   int         row_size = 5;
    std::vector<int>    int_vec;

    for (int i = 0; i < max_size; ++i)
        int_vec.push_back (i + 1);

    DMScu_StepVectorRange<int>  svec (&(int_vec.front ()) + col,
                                      &(int_vec.back ()) - row_size + col + 1,
                                      row_size);

    std::cout << "Size: " << svec.size () << std::endl;
    std::cout << "Front: " << svec.front () << std::endl;
    std::cout << "Back: " << svec.back () << std::endl;

    for (int i = 0; i < 4; ++i)
        std::cout << svec [i] << "  ";
    std::cout << std::endl;
    for (DMScu_StepVectorRange<int>::iterator iter = svec.begin ();
         iter != svec.end (); ++iter)
        std::cout << *iter << "  ";
    std::cout << std::endl;
    for (DMScu_StepVectorRange<int>::const_iterator citer = svec.begin ();
         citer != svec.end (); ++citer)
        std::cout << *citer << "  ";
    std::cout << std::endl;

    return (EXIT_SUCCESS);
}

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
