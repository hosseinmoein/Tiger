// Hossein Moein
// February 11, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <math.h>
#include <iostream>

// ----------------------------------------------------------------------------

namespace hmma
{

// For hunderds of years, mathematicians considered the equation x^2 + 1 = 0
// to be insolvable.  From a functional point of view, we know that the range
// of the square function, f(x) = x^2, contains only positive numbers, so that
// x^2 = -1 (and x^2 + 1 = 0) has no solution in the real number system.  In
// other words, if the solution (usually denoted by the letter i), is a number,
// it is not located anywhere on the real number line.  For this reason,
// whenever this expression arose in a calculation, mathematicians simply
// assumed that the original problem had no solution.  However, eventually they
// realized that they could not dismiss i so easily.
// In 1545, while examining the quadratic equation x^2 - 4x + 15 = 0,
// Girolamo Cardano noticed that, if he plugged either of the "solutions" into
// the equation, and used the fact that i^2 = -1, they did prove to "work"
// as valid solutions.
//
// Looking at it geometrically:
// A complex number, (a + ib with a and b real numbers) can be represented by a
// point in a plane, with x coordinate a and y coordinate b.  This is called
// the "complex plane". It differs from an ordinary plane only in the fact that
// we know how to multiply and divide complex numbers to get another complex
// number, something we do not generally know how to do for points in a plane.
// This picture suggests that there is another way to describe a complex
// number. Instead of using its real and imaginary parts, which are its x and y
// coordinates to describe it. We can use its distance from the origin, and the
// angle formed by a line segment from the origin to it, and the positive half
// of the x axis. The distance to the origin is usually denoted as r, that
// angle is usually called t (theta). t is called the "phase" and sometimes the
// "argument" of the complex number. The distance to the origin is called its
// "magnitude" and also its "absolute value".
// We use the Euclidean definition of distance, for which the Pythagorean
// theorem holds. This tells us
//     sqr(r) = sqr(a) + sqr(b)   and   r = sqrt(sqr(a) + sqr(b))
//
// As for t, we use the standard trigonometric definitions of sines and
// cosines. The sine of an angle is defined to be the ratio of y-coordinate b
// to length r, and the cosine is the ratio of x-coordinate a to r.
// This gives us the relations
//     a = r * cos(t)   and   b = r * sin(t)
//
template<class T = double>
class   Complex  {

public:

    using value_type = T;

private:

    value_type  real_ { };
    value_type  imaginary_ { };

public:

    // inline Complex () = default;
    inline Complex (const value_type &r, const value_type &i) noexcept
        : real_ (r), imaginary_ (i)  {   }

    inline const value_type &real () const noexcept  { return (real_); }
    inline value_type &real () noexcept  { return (real_); }
    inline const value_type &imaginary () const noexcept  {

        return (imaginary_);
    }
    inline value_type &imaginary () noexcept  { return (imaginary_); }

   // NOTE: Mathematically speaking this assignment makes no sense. But
   //       I define it here for practical reasons. For example, I want
   //       the following two lines to live side by side in harmony.
   //           double           d = 0;
   //           Complex<double>  c = 0;
   //
    inline Complex &operator = (const value_type &rhs) noexcept  {

        real () = imaginary () = rhs;
        return (*this);
    }

   // Transforms and returns the complex number to a new complex number
   // by running func().
   //
    template<typename F>
    inline Complex transform(F &func) const  { return (func(*this)); }

   // Transforms the complex number to a new complex number,
   // by running func().
   //
    template<typename F>
    inline void self_transform(F &func) { *this = func(*this); }

   // Runs func() on the complex number which returns true or false
   //
    template<typename F>
    inline bool filter(F &func) const  { return (func(*this)); }

    inline Complex conjugate () const noexcept  {

        return (Complex (real (), -imaginary ()));
    }

    inline value_type norm () const noexcept  {

        return (real () * real () + imaginary () * imaginary ());
    }

   // Complex absolute value also called Modulus, also called magnitude,
   // also called the distance from the origin
   //
    inline value_type cabs () const noexcept { return (::sqrt (norm ())); }

    inline Complex sqrt () const noexcept  {

        const value_type    y = ::sqrt ((cabs () - real ()) / 2.0);

       // NOTE: We have two roots: (+/-x + yi)
       //
        return (Complex (::fabs (imaginary () / (2.0 * y)), y));
    }

   // The angle or phase or argument of the complex number a + bi is the
   // angle, measured in radians, from the point 1 + 0i to a + bi, with
   // counterclockwise denoting positive angle. The angle of a complex
   // number c = a + bi is denoted Lc:
   //     Lc = arctan(b / a):
   //
   // A few comments are in order. First, angles that differ by a
   // multiple of 2PI are considered equal. Second, the formula above uses
   // the four quadrant arctan (often expressed as atan2(b, a) in computer
   // languages). The angle of the complex number 0 is undefined.
   //
    inline value_type angle () const noexcept  {

        return (::atan2 (imaginary (), real ()));
    }

   // e raised to the power of (a + bi)
   //
    inline Complex exp () const noexcept  {

        return (Complex (::exp (real ()) * ::cos (imaginary ()),
                         ::exp (real ()) * ::sin (imaginary ())));
    }

   // We define the natural logarithm of a (nonzero) complex number z as
   //     ln(z) = ln(|z|) + i * angle()
   // so that e^ln(z) = z.
   //
   // NOTE: The imaginary component of ln(z) is ambiguous; we can freely
   //       add any multiple of 2PI. Thus we can say that
   //       ln(1 - i) = ln(sqrt(2)) - i(PI/4 + 2*PI*k)
   //       where k is any integer 0, +/-1, +/-2, ...
   // k == 0 is called the principal logarithm
   //
    inline Complex ln (int k = 0) const noexcept  {

        return (Complex (::log (cabs ()),
                         angle () + k * 2.0 * 3.14159265358979323846));
    }

   // a + bi raised to the power of c + di
   //
    inline Complex pow (const Complex &n) const noexcept  {

        const value_type    ca = cabs ();
        const value_type    an = angle ();
        const value_type    v =
            ::exp (n.real () * ::log (ca) - n.imaginary () * an);

        return (Complex (
            v * ::cos (n.real () * an + n.imaginary () * ::log (ca)),
            v * ::sin (n.real () * an + n.imaginary () * ::log (ca))));
    }

   // a + bi raised to the power real number n
   //
    inline Complex pow (const value_type &n) const noexcept  {

        const value_type    an = angle ();
        const value_type    v = ::exp (n * ::log (cabs ()));

        return (Complex (v * ::cos (n * an), v * ::sin (n * an)));
    }

    inline Complex &operator ++ () noexcept  {    // ++Prefix

        real () += 1;
        imaginary () += 1;
        return (*this);
    }
    inline Complex operator ++ (int) noexcept  {  // Postfix++

        const Complex   slug = *this;

        real () += 1;
        imaginary () += 1;
        return (slug);
    }

    inline Complex &operator -- () noexcept  {    // --Prefix

        real () -= 1;
        imaginary () -= 1;
        return (*this);
    }
    inline Complex operator -- (int) noexcept  {  // Postfix--

        const Complex   slug = *this;

        real () -= 1;
        imaginary () -= 1;
        return (slug);
    }

    inline Complex &operator + () noexcept  { return (*this); }
    inline Complex operator - () noexcept  {

        return (Complex (-real (), -imaginary ()));
    }
    inline const Complex &operator + () const noexcept { return (*this); }
    inline Complex operator - () const noexcept  {

        return (Complex (-real (), -imaginary ()));
    }
};

// ----------------------------------------------------------------------------

template<class S, class T>
inline S &operator << (S &lhs, const Complex<T> &rhs)  {

    return (lhs << rhs.real ()
                << (rhs.imaginary () >= 0 ? '+' : '-')
                << ::fabs (rhs.imaginary ()) << 'i');
}

// ----------------------------------------------------------------------------

//
// Global Arithmetic operators
//

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator + (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (Complex<T>(lhs.real () + rhs.real (),
                       lhs.imaginary () + rhs.imaginary ()));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator + (const Complex<T> &lhs, const T &rhs)  {

    return (Complex<T>(lhs.real () + rhs, lhs.imaginary () + rhs));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator += (Complex<T> &lhs, const Complex<T> &rhs)  {

    lhs.real () += rhs.real ();
    lhs.imaginary () += rhs.imaginary ();
    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator += (Complex<T> &lhs, const T &rhs)  {

    lhs.real () += rhs;
    lhs.imaginary () += rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator - (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (Complex<T>(lhs.real () - rhs.real (),
                       lhs.imaginary () - rhs.imaginary ()));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T>
operator - (const Complex<T> &lhs, const T &rhs)  {

    return (Complex<T>(lhs.real () - rhs, lhs.imaginary () - rhs));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator -= (Complex<T> &lhs, const Complex<T> &rhs)  {

    lhs.real () -= rhs.real ();
    lhs.imaginary () -= rhs.imaginary ();
    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator -= (Complex<T> &lhs, const T &rhs)  {

    lhs.real () -= rhs;
    lhs.imaginary () -= rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator * (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (Complex<T>(
        lhs.real () * rhs.real () - lhs.imaginary () * rhs.imaginary (),
        lhs.real () * rhs.imaginary () + lhs.imaginary () * rhs.real ()));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator * (const Complex<T> &lhs, const T &rhs)  {

    return (Complex<T>(lhs.real () * rhs, lhs.imaginary () * rhs));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator *= (Complex<T> &lhs, const Complex<T> &rhs)  {

    lhs.real () =
        lhs.real () * rhs.real () - lhs.imaginary () * rhs.imaginary ();
    lhs.imaginary () =
        lhs.real () * rhs.imaginary () + lhs.imaginary () * rhs.real ();

    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator *= (Complex<T> &lhs, const T &rhs)  {

    lhs.real () *= rhs;
    lhs.imaginary () *= rhs;

    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator / (const Complex<T> &lhs, const Complex<T> &rhs)  {

    const   T   d =
        rhs.real () * rhs.real () + rhs.imaginary () * rhs.imaginary ();

    return (Complex<T>(
        (lhs.real () * rhs.real () + lhs.imaginary () * rhs.imaginary ()) / d,
        (lhs.imaginary() * rhs.real () - lhs.real () * rhs.imaginary ()) / d));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> operator / (const Complex<T> &lhs, const T &rhs)  {

    return (Complex<T>(lhs.real () / rhs, lhs.imaginary () / rhs));
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator /= (Complex<T> &lhs, const Complex<T> &rhs)  {

    const   T   d =
        rhs.real () * rhs.real () + rhs.imaginary () * rhs.imaginary ();

    lhs.real () =
        (lhs.real () * rhs.real () + lhs.imaginary () * rhs.imaginary ()) / d;
    lhs.imaginary () =
        (lhs.imaginary * rhs.real () - lhs.real () * rhs.imaginary ()) / d;

    return (lhs);
}

// ----------------------------------------------------------------------------

template<class T>
inline Complex<T> &operator /= (Complex<T> &lhs, const T &rhs)  {

    lhs.real () /= rhs;
    lhs.imaginary () /= rhs;

    return (lhs);
}

// ----------------------------------------------------------------------------

//
// Global Comparison operators
//

// ----------------------------------------------------------------------------

template<class T>
inline bool operator == (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (lhs.real () == rhs.real () && lhs.imaginary () == rhs.imaginary());
}

// ----------------------------------------------------------------------------

template<class T>
inline bool operator != (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (lhs.real () != rhs.real () || lhs.imaginary () != rhs.imaginary());
}

// ----------------------------------------------------------------------------

//
// NOTE: Mathematically speaking complex numbers have no natural ordering.
//       I define the following operators so there is a way to sort them for
//       practical reasons.
//

template<class T>
inline bool operator < (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (lhs.real () < rhs.real () ||
            (lhs.real() == rhs.real () && lhs.imaginary () < rhs.imaginary()));
}

// ----------------------------------------------------------------------------

template<class T>
inline bool operator <= (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (lhs.real () < rhs.real () ||
            (lhs.real() == rhs.real() && lhs.imaginary() <= rhs.imaginary()));
}

// ----------------------------------------------------------------------------

template<class T>
inline bool operator > (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (lhs.real () > rhs.real () ||
            (lhs.real() == rhs.real () && lhs.imaginary () > rhs.imaginary()));
}

// ----------------------------------------------------------------------------

template<class T>
inline bool operator >= (const Complex<T> &lhs, const Complex<T> &rhs)  {

    return (lhs.real () > rhs.real () ||
            (lhs.real() == rhs.real() && lhs.imaginary() >= rhs.imaginary()));
}

} // namespace hmma

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
