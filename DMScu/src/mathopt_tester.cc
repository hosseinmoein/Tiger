// Hossein Moein
// July 17 2009

#include <iostream>
#include <time.h>

#include <DMScu_MathOperators.h>

// ----------------------------------------------------------------------------

// Integrate
//
double old_eval (double start, double end)  {

    const   double  step = 0.01;
    double          sum = 0.0;

    for (double i = start; i < end; i += step)
        sum += (i / (1.0 + i)) * step;
	
    return (sum);
}

// ----------------------------------------------------------------------------

// Integrate
//
template<class EXPR, class TYPE>
double evaluate (DMScu_Expression<EXPR, TYPE> expr,
                 double start, double end)  {

    const   double  step = 0.01;
    double          sum = 0.0;

    for (double i = start; i < end; i += step)
        sum += expr (i) * step;

    return (sum);
}

// ----------------------------------------------------------------------------

int main (int argCnt, char *argVctr [])  {

    DMScu_Plus<double>                                  p_opt;
    DMScu_Expression<DMScu_Identity<double>, double>    x;

    std::cout << "3.5 + 10.2 = " << p_opt (x (3.5), x (10.2)) << std::endl;
    std::cout << "sum = " << evaluate (x / (1.0 + x), 0.0, 10.0) << std::endl;
    std::cout << "sum = " << old_eval (0.0, 10.0) << std::endl;
    std::cout << "sum = " << evaluate ((1.0 + x / 2.0) / x, 0.0, 10.0)
              << std::endl;
    std::cout << "sum = " << evaluate (1.0 / (x * x), 0.0, 10.0) << std::endl;
    std::cout << "sum = " << evaluate (x + x, 0.0, 10.0) << std::endl;
    std::cout << "sum = " << evaluate (2.0 * x, 0.0, 10.0) << std::endl;

    DMScu_Power<double> pow_opt;

    std::cout << "Testing the Power function ..." << std::endl;
    std::cout << "Pow = " << pow_opt (x (3.5) + x (4.5), 2.0) << std::endl;
    std::cout << "Pow = " << pow_opt (8.0, 2.0) << std::endl;
    std::cout << "Pow = " << pow_opt (2.0, x (3.5) + x (4.5)) << std::endl;
    std::cout << "Pow = "
              << pow_opt (x (3.5) + x (4.5), x (2.0) * x (6.0) / x (6.0))
              << std::endl;

    DMScu_NLog<double> log_opt;

    std::cout << "Testing the Log function ..." << std::endl;
    std::cout << "Log = " << log_opt (x (3.5) + x (4.5)) << std::endl;
    std::cout << "Log = " << log_opt (8.0) << std::endl;

    DMScu_EExpo<double> exp_opt;

    std::cout << "Testing the Exponential function ..." << std::endl;
    std::cout << "Exp = " << exp_opt (x (3.5) + x (4.5)) << std::endl;
    std::cout << "Exp = " << exp_opt (8.0) << std::endl;

/*
    for (size_t i = 0; i < 5; ++i)
        old_eval (0.0, 10.0);
    for (size_t i = 0; i < 5; ++i)
        evaluate (x / (1.0 + x), 0.0, 10.0);

    const   size_t  max_iter = 10000000;
    const   time_t  old_start = time (NULL);

    for (size_t i = 0; i < max_iter; ++i)
        old_eval (0.0, 10.0);

    const   time_t  old_end = time (NULL);
    const   time_t  new_start = time (NULL);

    for (size_t i = 0; i < max_iter; ++i)
        evaluate (x / (1.0 + x), 0.0, 10.0);

    const   time_t  new_end = time (NULL);

    std::cout << "Old time = " << (old_end - old_start) << "\n"
              << "New time = " << (new_end - new_start) << std::endl;
*/

    DMScu_Vector<double>    vec1 (10, 9);
    DMScu_Vector<double>    vec2 (10, 3);
    DMScu_Vector<double>    vec3 (10);

    vec3 = (vec1 + vec2) / vec2 + (vec1 - vec2);

    for (DMScu_Vector<double>::const_iterator citer = vec3.begin ();
         citer != vec3.end (); ++citer)
        std::cout << *citer << "  ";
    std::cout << std::endl;

    std::cout << "\n\nTesting the NL stuff ...\n" << std::endl;

    typedef DMScu_NL::NumList
                <-3,
                 DMScu_NL::NumList
                     <2,
                      DMScu_NL::NumList
                          <10, DMScu_NL::NullType> > >  NumList;
    typedef DMScu_NL::Length<NumList>                   NumListLength;
    typedef DMScu_NL::Append<NumList, 5>::Result        BiggerNumList;
    typedef DMScu_NL::Length<BiggerNumList>             BiggerNumListLength;
    typedef DMScu_NL::NumAt<BiggerNumList, 2>           NumAt2;
    typedef DMScu_NL::NumAt<BiggerNumList, 0>           NumAt0;
    typedef DMScu_NL::Erase<BiggerNumList, 2>::Result   ErasedAt2;
    typedef DMScu_NL::Length<ErasedAt2>                 ErasedAt2Length;

    std::cout << "nl2:  "
              << "Should be -3: " << NumList::value << "\n"
              << "Should be 2: " << NumList::tail::value << "\n"  
              << "Should be 10: " << NumList::tail::tail::value << "\n"
              << "Length: " << NumListLength::value << "\n"
              << "\tAfter Adding One More:\n"
              << "Length: " << BiggerNumListLength::value << "\n"
              << "Num at 2 (2): " << NumAt2::value << "\n"
              << "Num at 0 (5): " << NumAt0::value << "\n"
              << "\tAfter Erasing One At 2:\n"
              << "Length: " << ErasedAt2Length::value << "\n"
              << std::endl;

    return (EXIT_SUCCESS);
}

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
