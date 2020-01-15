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
* Neither the name of Hossein Moein and/or the Matrix nor the
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

#include <iostream>
#include <time.h>

#include <BaseMathOperators.h>
#include <MathOperators.h>

// #include <Eigen/Dense>

#include <Complex.h>
#include <Matrix.h>

using namespace hmma;

// ----------------------------------------------------------------------------

int main (int argCnt, char *argVctr [])  {

    std::cout.precision (2);

    std::cout << "\nTesting the Symmetric Matrix ...\n" << std::endl;

    SDMatrix smat (8, 8);
    int      count = 0;

    for (SDMatrix::size_type r = 0; r < 8; ++r)
        for (SDMatrix::size_type c = 0; c < 8; ++c)
            smat (r, c) = ++count;

    SDMatrix smat_tran;

    smat.transpose (smat_tran);

    std::cout << "Original Matrix:" << std::endl;
    smat.dump (std::cout) << std::endl;
    std::cout << "Is Symmetric Matrix: " << smat.is_symmetric () << std::endl;
    std::cout << "Transposed Matrix:" << std::endl;
    smat_tran.dump (std::cout) << std::endl;

    std::cout << "\nTesting the Row Iterator:" << std::endl;
    for (SDMatrix::row_const_iterator riter = smat.row_begin ();
         riter != smat.row_end (); ++riter)
        std::cout << *riter << "  ";
    std::cout << std::endl;

    std::cout << "\nTesting the Column Iterator:" << std::endl;
    for (SDMatrix::col_const_iterator citer = smat.col_begin ();
         citer != smat.col_end (); ++citer)
        std::cout << *citer << "  ";
    std::cout << std::endl;

    std::cout << "\n-------" << std::endl;
    std::cout << "\n\nTesting the Dense Matrix ...\n" << std::endl;

    {
        DDMatrix dmat (8, 3);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 3; ++j)
                dmat (i, j) = ++count;

        dmat.dump (std::cout);

        const   DDMatrix::ColumnVector   cv = dmat.get_column (1);
        const   DDMatrix::RowVector      rv = dmat.get_row (4);

        std::cout << "Column 1:\n";
        for (DDMatrix::ColumnVector::const_iterator citer = cv.begin ();
             citer != cv.end (); ++citer)
            std::cout << *citer << "\n";

        std::cout << "\nRow 4:\n";
        for (DDMatrix::RowVector::const_iterator citer = rv.begin ();
             citer != rv.end (); ++citer)
            std::cout << *citer << "   ";
        std::cout << std::endl;

        std::cout << "\nTesting the column iterator ...\n" << std::endl;
        for (DDMatrix::col_const_iterator cciter = dmat.col_begin ();
             cciter != dmat.col_end (); ++cciter)
            std::cout << *cciter << "  ";
        std::cout << std::endl;

        std::cout << "\nTesting the Row Iterator:" << std::endl;
        for (DDMatrix::row_const_iterator rciter = dmat.row_begin ();
             rciter != dmat.row_end (); ++rciter)
            std::cout << *rciter << "  ";
        std::cout << std::endl;

        std::cout << "\nTesting the transpose methods ...\n" << std::endl;

        DDMatrix dmat_tran;

        dmat.transpose (dmat_tran);
        std::cout << "Original Matrix:" << std::endl;
        dmat.dump (std::cout) << std::endl;
        std::cout << "Transposed Matrix:" << std::endl;
        dmat_tran.dump (std::cout) << std::endl;

        std::cout << "\nTesting the set row/column methods ...\n" << std::endl;

        std::vector<double> vec;

        for (DDMatrix::size_type i = 0; i < 3; ++i)
            vec.push_back (111.1);
        dmat.set_row (vec.begin (), 7);

        vec.clear ();
        for (DDMatrix::size_type i = 0; i < 8; ++i)
            vec.push_back (111.1);
        dmat.set_column (vec.begin (), 2);

        dmat.dump (std::cout);
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting the transpose operator ...\n" << std::endl;

        DDMatrix dmat (8, 3);
        DDMatrix one_mat (3, 8);

        int  count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 3; ++j)  {
                dmat (i, j) = ++count;
                one_mat (j, i) = 1.0;
            }

        const   DDMatrix dmat_tran = one_mat + ~ dmat;
        const   DDMatrix dmat_tran2 = ~ dmat + one_mat;
        const   DDMatrix dmat_tran3 = one_mat + ~ dmat + one_mat;

        std::cout << "Original Matrix:" << std::endl;
        dmat.dump (std::cout) << std::endl;
        std::cout << "Transposed Matrix:" << std::endl;
        dmat_tran.dump (std::cout) << std::endl;
        dmat_tran2.dump (std::cout) << std::endl;
        dmat_tran3.dump (std::cout) << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting Iterators ...\n" << std::endl;

        DDMatrix dmat (3, 8);

        int  count = 0;

        for (DDMatrix::size_type i = 0; i < 3; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat (i, j) = ++count;

        DDMatrix dmat2 (7, 10);

        dmat2 = dmat;
        dmat2.dump (std::cout);

        std::cout << "\nTesting the Row Iterator:" << std::endl;
        for (DDMatrix::row_iterator riter = dmat2.row_begin ();
             riter != dmat2.row_end (); ++riter)
            std::cout << *riter << "  ";
        std::cout << std::endl;

        const   DDMatrix::ColumnVector   cv = dmat2.get_column (5);
        const   DDMatrix::RowVector      rv = dmat2.get_row (2);

        std::cout << "Column 5:\n";
        for (DDMatrix::ColumnVector::const_iterator citer = cv.begin ();
             citer != cv.end (); ++citer)
            std::cout << *citer << "\n";

        std::cout << "\nRow 2:\n";
        for (DDMatrix::RowVector::const_iterator citer = rv.begin ();
             citer != rv.end (); ++citer)
            std::cout << *citer << "   ";
        std::cout << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting Iterators (2) ...\n" << std::endl;

        DDMatrix dmat (8, 8);

        int  count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat (i, j) = ++count;

        const   DDMatrix dmat2 = dmat;

        dmat2.dump (std::cout);

        std::cout << "\nTesting the Row Iterator:" << std::endl;
        for (DDMatrix::row_iterator riter = dmat.row_begin ();
             riter != dmat.row_end (); ++riter)
            std::cout << *riter << "  ";
        std::cout << std::endl;

        const   DDMatrix::ColumnVector   cv = dmat2.get_column (5);
        const   DDMatrix::RowVector      rv = dmat2.get_row (6);

        std::cout << "Column 5:\n";
        for (DDMatrix::ColumnVector::const_iterator citer = cv.begin ();
             citer != cv.end (); ++citer)
            std::cout << *citer << "\n";

        std::cout << "\nRow 6:\n";
        for (DDMatrix::RowVector::const_iterator citer = rv.begin ();
             citer != rv.end (); ++citer)
            std::cout << *citer << "   ";
        std::cout << std::endl;

        const   int pre = std::cout.precision (8);

        std::cout << "\nTesting determinant calculation ...\n" << std::endl;
        std::cout << "Determinant:  " << dmat.determinant () << std::endl;
        std::cout << "Is singular?  " << dmat.is_singular () << std::endl;
        std::cout.precision (pre);
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting determinant calculation ...\n" << std::endl;

        const   int     pre = std::cout.precision (8);
        DDMatrix dmat (3, 3);
        DDMatrix dmat_dev (3, 3);
        DDMatrix dmat_null (3, 3);

        // 2*X + 3*Y + 2*Z = 13
        // 3*X + 2*Y + 3*Z = 17
        // 4*X - 2*Y + 2*Z = 12
        //
        // [2  3  2]   [X]   [13]
        // [3  2  3] * [Y] = [17]
        // [4  2  2]   [Z]   [12]

        dmat (0, 0) = 2.0;
        dmat (0, 1) = 3.0;
        dmat (0, 2) = 2.0;

        dmat (1, 0) = 3.0;
        dmat (1, 1) = 2.0;
        dmat (1, 2) = 3.0;

        dmat (2, 0) = 4.0;
        dmat (2, 1) = 2.0;
        dmat (2, 2) = 2.0;

        std::cout << "Original Matrix:" << std::endl;
        dmat.dump (std::cout) << std::endl;

        std::cout << "Determinant:  " << dmat.determinant () << std::endl;
        std::cout << "Condition#:   " << dmat.condition () << std::endl;
        std::cout << "Is singular?  " << dmat.is_singular () << std::endl;

        DDMatrix adj;

        dmat.adjoint (adj);
        std::cout << "The Adjoint matrix:" << std::endl;
        adj.dump (std::cout) << std::endl;

        dmat.scale (Multiplies<double>(), 10.0);
        std::cout << "Determinant after multiplied by 10:  "
                  << dmat.determinant () << std::endl;
        std::cout << "Condition# after multiplied by 10:   "
                  << dmat.condition () << std::endl;
        std::cout << "Is singular?                         "
                  << dmat.is_singular () << "\n" << std::endl;

        std::cout << "Original dmat:" << std::endl;
        dmat.dump (std::cout) << std::endl;

        DDMatrix tmp_mtx = dmat + dmat_null;

        dmat_dev = (dmat + dmat_null) / tmp_mtx;
        std::cout << "After dmat / dmat:" << std::endl;
        dmat_dev.dump (std::cout) << std::endl;

        dmat (0, 0) = 5.0;
        dmat (0, 1) = -3.0;
        dmat (0, 2) = 2.0;

        dmat (1, 0) = 1.0;
        dmat (1, 1) = 0.0;
        dmat (1, 2) = 6.0;

        dmat (2, 0) = 3.0;
        dmat (2, 1) = 1.0;
        dmat (2, 2) = -2.0;

        auto            dfut = dmat.determinant_async ();
        const double    deter = dfut.get();

        std::cout << "Determinant:  " << deter << std::endl;
        std::cout << "Condition#:   " << dmat.condition () << std::endl;
        std::cout << "Is singular?  " << dmat.is_singular () << std::endl;

        dmat.resize (4, 4);
        dmat (0, 0) = 1.0;
        dmat (0, 1) = 2.0;
        dmat (0, 2) = -1.0;
        dmat (0, 3) = 0.0;

        dmat (1, 0) = -2.0;
        dmat (1, 1) = 1.0;
        dmat (1, 2) = 3.0;
        dmat (1, 3) = 1.0;

        dmat (2, 0) = 3.0;
        dmat (2, 1) = 0.0;
        dmat (2, 2) = -2.0;
        dmat (2, 3) = 2.0;

        dmat (3, 0) = 1.0;
        dmat (3, 1) = 2.0;
        dmat (3, 2) = 3.0;
        dmat (3, 3) = 4.0;
        std::cout << "Determinant:  " << dmat.determinant () << std::endl;
        std::cout << "Is singular?  " << dmat.is_singular () << std::endl;

        std::cout.precision (pre);
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting Dense Matrix Expression math ...\n"
                  << std::endl;

        DDMatrix dmat1 (8, 8);
        DDMatrix dmat2 (8, 8);

        int  count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)  {
                dmat1 (i, j) = ++count;
                dmat2 (i, j) = 10.0;
            }

        std::cout << "Before Addition(1):" << std::endl;
        dmat1.dump (std::cout) << std::endl;

        const   DDMatrix dmat3 =
            (dmat1 + dmat2) - dmat2 + (dmat1 + dmat2);

        std::cout << "After Addition(2):" << std::endl;
        dmat3.dump (std::cout) << std::endl;

        std::cout << "Before Addition:" << std::endl;
        dmat1.dump (std::cout) << std::endl;
        dmat2.dump (std::cout) << std::endl;

        const   DDMatrix dmat4 = dmat1 + dmat2;

        std::cout << "After Addition(3):" << std::endl;
        dmat4.dump (std::cout) << std::endl;

        count = 0;
        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat1 (i, j) = ++count;
        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat2 (i, j) = --count;

        std::cout << "Original Matrices:" << std::endl;
        dmat1.dump (std::cout) << std::endl;
        dmat2.dump (std::cout) << std::endl;

        dmat2 += dmat1;
        std::cout << "After dmat2 += dmat1;:" << std::endl;
        dmat2.dump (std::cout) << std::endl;

        count = 0;
        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat1 (i, j) = ++count;
        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat2 (i, j) = --count;

        dmat2 -= dmat1;
        std::cout << "After dmat2 -= dmat1;:" << std::endl;
        dmat2.dump (std::cout) << std::endl;

        const   DDMatrix dmat5 =
            (dmat1 + dmat2) - dmat2 + (dmat1 + dmat2);

        std::cout << "Original Matrix:" << std::endl;
        dmat5.dump (std::cout) << std::endl;
        std::cout << "dmat2 += (dmat1 + dmat2) - dmat2 + (dmat1 + dmat2);:"
                  << std::endl;
        dmat2 += (dmat1 + dmat2) - dmat2 + (dmat1 + dmat2);
        dmat2.dump (std::cout) << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting matrix multiplication ...\n" << std::endl;

        {
            DDMatrix m1 (1, 4);
            DDMatrix m2 (4, 1);
            int      count = 0;

            std::cout << "\t[1 X 4] * [4 X 1] = [1 X 1]\n" << std::endl;
            for (DDMatrix::size_type i = 0; i < 1; ++i)
                for (DDMatrix::size_type j = 0; j < 4; ++j)
                    m1 (i, j) = ++count;

            count = 0;
            for (DDMatrix::size_type i = 0; i < 4; ++i)
                for (DDMatrix::size_type j = 0; j < 1; ++j)
                    m2 (i, j) = ++count;

            const   DDMatrix m3 = m1 * m2;

            std::cout << "Original Matrices:" << std::endl;
            m1.dump (std::cout) << std::endl;
            m2.dump (std::cout) << std::endl;
            std::cout << "After m3 = m1 * m2:" << std::endl;
            m3.dump (std::cout) << std::endl;
        }

        {
            DDMatrix m1 (3, 3);
            DDMatrix m2 (3, 3);
            int      count = 0;

            std::cout << "\t[3 X 3] * [3 X 3] = [3 X 3]\n" << std::endl;
            for (DDMatrix::size_type i = 0; i < 3; ++i)
                for (DDMatrix::size_type j = 0; j < 3; ++j)
                    m1 (i, j) = ++count;

            count = 0;
            for (DDMatrix::size_type i = 0; i < 3; ++i)
                for (DDMatrix::size_type j = 0; j < 3; ++j)
                    m2 (i, j) = ++count;

            const   DDMatrix m3 = m1 * m2;

            std::cout << "Original matrices:" << std::endl;
            m1.dump (std::cout) << std::endl;
            m2.dump (std::cout) << std::endl;
            std::cout << "After multiplication:" << std::endl;
            m3.dump (std::cout) << std::endl;
        }

        {
            DDMatrix m1 (3, 2);
            DDMatrix zero_m1 (3, 2);
            DDMatrix m2 (2, 3);
            DDMatrix zero_m2 (2, 3);
            DDMatrix m3 (3, 3);
            int      count = 0;

            std::cout << "\t[3 X 2] * [2 X 3] = [3 X 3]\n" << std::endl;
            for (DDMatrix::size_type i = 0; i < 3; ++i)
                for (DDMatrix::size_type j = 0; j < 2; ++j)  {
                    m1 (i, j) = ++count;
                    zero_m1 (i, j) = 0.0;
                }

            count = 0;
            for (DDMatrix::size_type i = 0; i < 2; ++i)
                for (DDMatrix::size_type j = 0; j < 3; ++j)  {
                    m2 (i, j) = ++count;
                    zero_m2 (i, j) = 0.0;
                }

            m3 = m1 * m2;

            std::cout << "Original matrices:" << std::endl;
            m1.dump (std::cout) << std::endl;
            m2.dump (std::cout) << std::endl;
            std::cout << "After multiplication:" << std::endl;
            m3.dump (std::cout) << std::endl;

            std::cout << "\n-------" << std::endl;
            m3 = (m1 + zero_m1) * m2;
            m3.dump (std::cout) << std::endl;
            m3 = m1 * (m2 + zero_m2);
            m3.dump (std::cout) << std::endl;
            m3 = (m1 + zero_m1) * (m2 + zero_m2);
            m3.dump (std::cout) << std::endl;

            std::cout << "\n-------" << std::endl;

            DDMatrix m5 (3, 2);
            DDMatrix m6 (2, 3);

            count = 0;
            for (DDMatrix::size_type i = 0; i < 3; ++i)
                for (DDMatrix::size_type j = 0; j < 2; ++j)
                    m5 (i, j) = ++count;
            for (DDMatrix::size_type i = 0; i < 2; ++i)
                for (DDMatrix::size_type j = 0; j < 3; ++j)
                    m6 (i, j) = --count;

            std::cout << "Original matrices:" << std::endl;
            m5.dump (std::cout) << std::endl;
            m6.dump (std::cout) << std::endl;

            m5 *= m6;
            std::cout << "After m5 *= m6:" << std::endl;
            m5.dump (std::cout) << std::endl;
        }
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting comparison operators ...\n" << std::endl;

        DDMatrix dmat1 (8, 8);
        DDMatrix dmat2 (8, 8);
        DDMatrix dmat3 (3, 8);
        DDMatrix dmat4 (3, 8);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat1 (i, j) = ++count;
        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat2 (i, j) = --count;

        count = 0;
        for (DDMatrix::size_type i = 0; i < 3; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat3 (i, j) = ++count;
        for (DDMatrix::size_type i = 0; i < 3; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat4 (i, j) = --count;

        if ((dmat1 + dmat2) - dmat2 + (dmat1 + dmat2) !=
            (dmat1 + dmat2) - dmat2 + (dmat1 + dmat2))  {
            std::cout << "ERROR: Comparison faild (1)\n" << std::endl;
            return (EXIT_FAILURE);
        }

        if ((dmat1 + dmat2) - dmat2 + (dmat1 + dmat2) ==
            (dmat1 + dmat2) - dmat2 + (dmat1 + dmat1))  {
            std::cout << "ERROR: Comparison faild (2)\n" << std::endl;
            return (EXIT_FAILURE);
        }

        if ((dmat1 + dmat2) - dmat2 + (dmat1 + dmat2) !=
            (dmat1 + dmat2) - dmat2 + (dmat1 + dmat2) + (dmat1 - dmat1))  {
            std::cout << "ERROR: Comparison faild (3)\n" << std::endl;
            return (EXIT_FAILURE);
        }

        const   DDMatrix x = dmat3 * dmat2;
        const   DDMatrix y = dmat4 * dmat1;

        x.dump (std::cout) << std::endl;
        y.dump (std::cout) << std::endl;

        if (dmat3 * dmat2 == dmat4 * dmat1)  {
            std::cout << "ERROR: Comparison faild (4)\n" << std::endl;
            return (EXIT_FAILURE);
        }

        const   DDMatrix xx = dmat3 * dmat2 - x;
        const   DDMatrix yy = dmat4 * dmat1 - y;

        xx.dump (std::cout) << std::endl;
        yy.dump (std::cout) << std::endl;

        if (dmat3 * dmat2 - x != dmat4 * dmat1 - y)  {
            std::cout << "ERROR: Comparison faild (5)\n" << std::endl;
            return (EXIT_FAILURE);
        }

        if (x - dmat3 * dmat2 != dmat3 * dmat2 - x)  {
            std::cout << "ERROR: Comparison faild (6)\n" << std::endl;
            return (EXIT_FAILURE);
        }

        std::cout << "Comparison operator testing was successfull\n"
                  << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting invert method ...\n" << std::endl;

        const   int     pre = std::cout.precision (8);
        DDMatrix dmat1 (3, 3);
        DDMatrix dmat2 (3, 3);
        int      count = 0;

        dmat1 (0, 0) = 2.0;
        dmat1 (0, 1) = 3.0;
        dmat1 (0, 2) = 2.0;

        dmat1 (1, 0) = 3.0;
        dmat1 (1, 1) = 2.0;
        dmat1 (1, 2) = 3.0;

        dmat1 (2, 0) = 4.0;
        dmat1 (2, 1) = 2.0;
        dmat1 (2, 2) = 2.0;

        const   DDMatrix dmat3 = ! dmat1;

        std::cout << "The matrix:" << std::endl;
        dmat1.dump (std::cout) << std::endl;
        std::cout << "The inverted matrix:" << std::endl;
        dmat3.dump (std::cout) << std::endl;
        dmat2 = dmat1 * dmat3;
        std::cout << "The product:" << std::endl;
        dmat2.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting solve() method ...\n" << std::endl;

        DDMatrix dmat (3, 3);
        DDMatrix rhs (3, 1);

        std::cout << "2*X + 3*Y + 2*Z = 13\n";
        std::cout << "3*X + 2*Y + 3*Z = 17\n";
        std::cout << "4*X - 2*Y + 2*Z = 12\n\n";

        dmat (0, 0) = 2.0;
        dmat (0, 1) = 3.0;
        dmat (0, 2) = 2.0;

        dmat (1, 0) = 3.0;
        dmat (1, 1) = 2.0;
        dmat (1, 2) = 3.0;

        dmat (2, 0) = 4.0;
        dmat (2, 1) = -2.0;
        dmat (2, 2) = 2.0;
        std::cout << "LHS:\n";
        dmat.dump (std::cout) << std::endl;

        rhs (0, 0) = 13.0;
        rhs (1, 0) = 17.0;
        rhs (2, 0) = 12.0;
        std::cout << "RHS:\n";
        rhs.dump (std::cout) << std::endl;

        const   DDMatrix sol = dmat.solve_se (rhs);

        std::cout << "Solution:\n";
        sol.dump (std::cout) << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting rref() method ...\n" << std::endl;

        DDMatrix dmat (3, 3);
        unsigned    int rank;

        dmat (0, 0) = 2.0;
        dmat (0, 1) = 3.0;
        dmat (0, 2) = 2.0;

        dmat (1, 0) = 3.0;
        dmat (1, 1) = 2.0;
        dmat (1, 2) = 3.0;

        dmat (2, 0) = 4.0;
        dmat (2, 1) = 2.0;
        dmat (2, 2) = 2.0;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;
        dmat.rref (rank);
        std::cout << "Row Reduced Echelon Form with rank (" << rank << ")\n";
        dmat.dump (std::cout) << std::endl;

        DDMatrix dmat2 (8, 3);

        double  count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 3; ++j)
                dmat2 (i, j) = ++count;

        std::cout << "Original Matrix:\n";
        dmat2.dump (std::cout) << std::endl;
        dmat2.rref (rank);
        std::cout << "Row Reduced Echelon Form with rank (" << rank << ")\n";
        dmat2.dump (std::cout) << std::endl;

        DDMatrix dmat3 (8, 8);

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 8; ++j)
                dmat3 (i, j) = ::rand ();

        std::cout << "Original Matrix:\n";
        dmat3.dump (std::cout) << std::endl;
        dmat3.rref (rank);
        std::cout << "Row Reduced Echelon Form with rank (" << rank << ")\n";
        std::cout << "Rank: " << dmat3.rank () << "\n";
        dmat3.dump (std::cout) << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting mat_col_mean() method ...\n" << std::endl;

        DDMatrix dmat (8, 4);
        DDMatrix null_mat (1, 4);
        DDMatrix two_mat (4, 1);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 4; ++j)
                dmat (i, j) = ++count;

        for (DDMatrix::size_type i = 0; i < 4; ++i)
            for (DDMatrix::size_type j = 0; j < 1; ++j)
                two_mat (i, j) = 2.0;

        const   DDMatrix mean_mat = mat_col_mean (dmat) + null_mat;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;
        std::cout << "Null Matrix:\n";
        null_mat.dump (std::cout) << std::endl;
        std::cout << "Mean Matrix:\n";
        mean_mat.dump (std::cout) << std::endl;

        const   DDMatrix mean_mat2 = mat_col_mean (dmat) * two_mat;
        const   DDMatrix mean_mat3 =
            null_mat + mat_col_mean (dmat) * two_mat;

        std::cout << "One Matrix:\n";
        two_mat.dump (std::cout) << std::endl;
        std::cout << "Mean Matrix (2):\n";
        mean_mat2.dump (std::cout) << std::endl;
        mean_mat3.dump (std::cout) << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting covariance() method ...\n" << std::endl;

        const   int     pre = std::cout.precision (8);
        DDMatrix dmat (5, 4);

        dmat (0, 0) = 4.0;
        dmat (0, 1) = 2.0;
        dmat (0, 2) = 0.6;
        dmat (0, 3) = 3.0;

        dmat (1, 0) = 4.2;
        dmat (1, 1) = 2.1;
        dmat (1, 2) = 0.59;
        dmat (1, 3) = 3.2;

        dmat (2, 0) = 3.9;
        dmat (2, 1) = 2.0;
        dmat (2, 2) = 0.58;
        dmat (2, 3) = 2.89;

        dmat (3, 0) = 4.3;
        dmat (3, 1) = 2.1;
        dmat (3, 2) = 0.62;
        dmat (3, 3) = 3.298;

        dmat (4, 0) = 4.1;
        dmat (4, 1) = 2.2;
        dmat (4, 2) = 0.63;
        dmat (4, 3) = 3.098;

        const   DDMatrix cov = dmat.covariance ();
        const   DDMatrix corrl = dmat.correlation ();

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;
        std::cout << "Covariance Matrix:\n";
        cov.dump (std::cout) << std::endl;
        std::cout << "Correlation Matrix:\n";
        corrl.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    std::cout << "\n-------" << std::endl;

    {
        std::cout << "\nTesting is_normal() method ...\n" << std::endl;

        DDMatrix dmat (3, 3);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 3; ++i)
            for (DDMatrix::size_type j = 0; j < 3; ++j)
                dmat (i, j) = ++count;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout);
        std::cout << "Is Normal:     " << dmat.is_normal () << std::endl;
        std::cout << "Is Orthogonal: " << dmat.is_orthogonal ()
                  << "\n" << std::endl;

        dmat (0, 0) = 1;
        dmat (0, 1) = 1;
        dmat (0, 2) = 0;

        dmat (1, 0) = 0;
        dmat (1, 1) = 1;
        dmat (1, 2) = 1;

        dmat (2, 0) = 1;
        dmat (2, 1) = 0;
        dmat (2, 2) = 1;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout);
        std::cout << "Is Normal:     " << dmat.is_normal () << std::endl;
        std::cout << "Is Orthogonal: " << dmat.is_orthogonal ()
                  << "\n" << std::endl;
    }

    std::cout << "\n-------" << std::endl;

    {
        const   int pre = std::cout.precision (6);

        std::cout << "\nTesting eigen_space() for symmetric metrices ...\n"
                  << std::endl;

        SDMatrix dmat (10, 10);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 10; ++i)
            for (DDMatrix::size_type j = 0; j < 10; ++j)
                dmat (i, j) = ++count;

        DDMatrix eigenvals;
        DDMatrix eigenvecs;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;

        dmat.eigen_space (eigenvals, eigenvecs, true);
        std::cout << "Eigen Values:\n";
        eigenvals.dump (std::cout) << std::endl;
        std::cout << "Eigen Vectors:\n";
        eigenvecs.dump (std::cout) << std::endl;

        std::cout << "\n-------\n" << std::endl;

        SDMatrix dmat2 (5, 5);

        count = 0;
        for (DDMatrix::size_type i = 0; i < 5; ++i)
            for (DDMatrix::size_type j = 0; j < 5; ++j)
                dmat2 (i, j) = ++count;

        std::cout << "Original Matrix:\n";
        dmat2.dump (std::cout) << std::endl;

        auto    efut = dmat2.eigen_space_async(eigenvals, eigenvecs, true);

        efut.get();
        std::cout << "Eigen Values:\n";
        eigenvals.dump (std::cout) << std::endl;
        std::cout << "Eigen Vectors:\n";
        eigenvecs.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int     pre = std::cout.precision (6);

        std::cout << "\nTesting eigen_space() for nonsymmetric metrices ...\n"
                  << std::endl;

        DDMatrix dmat (10, 10);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 10; ++i)
            for (DDMatrix::size_type j = 0; j < 10; ++j)
                dmat (i, j) = ++count;

        DDMatrix eigenvals;
        DDMatrix eigenvecs;

        std::cout << "Original Matrix:\n";

        dmat.dump (std::cout) << std::endl;

        dmat.eigen_space (eigenvals, eigenvecs, true);
        std::cout << "Eigen Values:\n";
        eigenvals.dump (std::cout) << std::endl;
        std::cout << "Eigen Vectors:\n";
        eigenvecs.dump (std::cout) << std::endl;

        std::cout << "\n-------\n" << std::endl;

        DDMatrix dmat2 (5, 5);

        count = 0;
        for (DDMatrix::size_type i = 0; i < 5; ++i)
            for (DDMatrix::size_type j = 0; j < 5; ++j)
                dmat2 (i, j) = ++count;

        std::cout << "Original Matrix:\n";
        dmat2.dump (std::cout) << std::endl;

        dmat2.eigen_space (eigenvals, eigenvecs, false);
        std::cout << "Eigen Values:\n";
        eigenvals.dump (std::cout) << std::endl;
        std::cout << "Eigen Vectors:\n";
        eigenvecs.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    std::cout << "\n-------" << std::endl;

    {
        const   int pre = std::cout.precision (6);

        std::cout << "\nTesting svd() method ...\n" << std::endl;

        {
            DDMatrix dmat (8, 4);
            int      count = 0;

            for (DDMatrix::size_type i = 0; i < 8; ++i)
                for (DDMatrix::size_type j = 0; j < 4; ++j)
                    dmat (i, j) = ++count;

            DDMatrix U;
            DDMatrix S;
            DDMatrix V;

            std::cout << "Original Matrix:\n";
            dmat.dump (std::cout) << std::endl;

            dmat.svd (U, S, V);
            std::cout << "U:\n";
            U.dump (std::cout) << std::endl;
            std::cout << "S:\n";
            S.dump (std::cout) << std::endl;
            std::cout << "V:\n";
            V.dump (std::cout) << std::endl;

            const   DDMatrix A = U * S * ~ V;

            std::cout << "U * S * ~ V:\n";
            A.dump (std::cout) << std::endl;

            //if (dmat != U * S * ~ V)  {
            //    std::cout << "ERROR: Comparison faild for SVD equality (1)\n"
            //              << std::endl;
            //    return (EXIT_FAILURE);
            //}
        }

        std::cout << "\n-------\n" << std::endl;

        {
            DDMatrix dmat (4, 8);
            int      count = 0;

            for (DDMatrix::size_type i = 0; i < 4; ++i)
                for (DDMatrix::size_type j = 0; j < 8; ++j)
                    dmat (i, j) = ++count;

            DDMatrix U;
            DDMatrix S;
            DDMatrix V;

            std::cout << "Original Matrix:\n";
            dmat.dump (std::cout) << std::endl;

            dmat.svd (U, S, V);
            std::cout << "U:\n";
            U.dump (std::cout) << std::endl;
            std::cout << "S:\n";
            S.dump (std::cout) << std::endl;
            std::cout << "V:\n";
            V.dump (std::cout) << std::endl;

            const   DDMatrix A = U * S * ~ V;

            std::cout << "U * S * ~ V:\n";
            A.dump (std::cout) << std::endl;

            //if (dmat != U * S * ~ V)  {
            //    std::cout << "ERROR: Comparison faild for SVD equality (2)\n"
            //              << std::endl;
            //    return (EXIT_FAILURE);
            //}
        }

        std::cout << "\n-------\n" << std::endl;

        {
            DDMatrix dmat (8, 8);
            int      count = 0;

            for (DDMatrix::size_type i = 0; i < 8; ++i)
                for (DDMatrix::size_type j = 0; j < 8; ++j)
                    dmat (i, j) = ++count;

            DDMatrix U;
            DDMatrix S;
            DDMatrix V;

            std::cout << "Original Matrix:\n";
            dmat.dump (std::cout) << std::endl;

            dmat.svd (U, S, V);
            std::cout << "U:\n";
            U.dump (std::cout) << std::endl;
            std::cout << "S:\n";
            S.dump (std::cout) << std::endl;
            std::cout << "V:\n";
            V.dump (std::cout) << std::endl;

            const   DDMatrix A = U * S * ~ V;

            std::cout << "U * S * ~ V:\n";
            A.dump (std::cout) << std::endl;

            //if (dmat != U * S * ~ V)  {
            //    std::cout << "ERROR: Comparison faild for SVD equality (3)\n"
            //              << std::endl;
            //    return (EXIT_FAILURE);
            //}
        }

        std::cout << "\n-------\n" << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (6);

        std::cout << "\nTesting qrd() method ...\n" << std::endl;

        {
            DDMatrix dmat (8, 4);
            int      count = 0;

            for (DDMatrix::size_type i = 0; i < 8; ++i)
                for (DDMatrix::size_type j = 0; j < 4; ++j)
                    dmat (i, j) = ++count;

            DDMatrix Q;
            DDMatrix R;

            std::cout << "Original Matrix:\n";
            dmat.dump (std::cout) << std::endl;

            dmat.qrd (Q, R);
            std::cout << "Q:\n";
            Q.dump (std::cout) << std::endl;
            std::cout << "R:\n";
            R.dump (std::cout) << std::endl;

            const   DDMatrix A = Q * R;

            std::cout << "Q * R:\n";
            A.dump (std::cout) << std::endl;

            //if (dmat != Q * R)  {
            //    std::cout << "ERROR: Comparison faild for QRD equality (1)\n"
            //              << std::endl;
            //    return (EXIT_FAILURE);
            //}
        }

        {
            DDMatrix dmat (4, 8);
            int      count = 0;

            for (DDMatrix::size_type i = 0; i < 4; ++i)
                for (DDMatrix::size_type j = 0; j < 8; ++j)
                    dmat (i, j) = ++count;

            DDMatrix Q;
            DDMatrix R;

            std::cout << "Original Matrix:\n";
            dmat.dump (std::cout) << std::endl;

            dmat.qrd (Q, R);
            std::cout << "Q:\n";
            Q.dump (std::cout) << std::endl;
            std::cout << "R:\n";
            R.dump (std::cout) << std::endl;

            const   DDMatrix A = Q * R;

            std::cout << "Q * R:\n";
            A.dump (std::cout) << std::endl;

            //if (dmat != Q * R)  {
            //    std::cout << "ERROR: Comparison faild for QRD equality (2)\n"
            //              << std::endl;
            //    return (EXIT_FAILURE);
            //}
        }

        {
            DDMatrix dmat (8, 8);
            int      count = 0;

            for (DDMatrix::size_type i = 0; i < 8; ++i)
                for (DDMatrix::size_type j = 0; j < 8; ++j)
                    dmat (i, j) = ++count;

            DDMatrix Q;
            DDMatrix R;

            std::cout << "Original Matrix:\n";
            dmat.dump (std::cout) << std::endl;

            dmat.qrd (Q, R);
            std::cout << "Q:\n";
            Q.dump (std::cout) << std::endl;
            std::cout << "R:\n";
            R.dump (std::cout) << std::endl;

            const   DDMatrix A = Q * R;

            std::cout << "Q * R:\n";
            A.dump (std::cout) << std::endl;

            //if (dmat != Q * R)  {
            //    std::cout << "ERROR: Comparison faild for QRD equality (3)\n"
            //              << std::endl;
            //    return (EXIT_FAILURE);
            //}
        }

        std::cout << "\n-------\n" << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (6);

        std::cout << "\nTesting lud() method ...\n" << std::endl;

        DDMatrix dmat (8, 8);
        int      count = 0;

        for (DDMatrix::size_type i = 0; i < 8; ++i)
            for (DDMatrix::size_type j = 0; j < 4; ++j)
                dmat (i, j) = ++count;

        DDMatrix L;
        DDMatrix U;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;

        dmat.lud (L, U);
        std::cout << "L:\n";
        L.dump (std::cout) << std::endl;
        std::cout << "U:\n";
        U.dump (std::cout) << std::endl;

        const   DDMatrix A = L * U;

        std::cout << "L * U:\n";
        A.dump (std::cout) << std::endl;

        std::cout << "\n-------\n" << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (6);

        std::cout << "\nTesting chod() method ...\n" << std::endl;

        SDMatrix dmat (4, 4);

       // A nice symmetric positive-definite matrix
       //
        dmat (0, 0) = 1;
        dmat (0, 1) = 1;
        dmat (0, 2) = 1;
        dmat (0, 3) = 1;
        dmat (1, 1) = 5;
        dmat (1, 2) = 5;
        dmat (1, 3) = 5;
        dmat (2, 2) = 14;
        dmat (2, 3) = 14;
        dmat (3, 3) = 15;

        DDMatrix R;
        DDMatrix L;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;

        dmat.chod (R);
        dmat.chod (L, false);

        std::cout << "R:\n";
        R.dump (std::cout) << std::endl;

        const   DDMatrix A = ~R * R;

        std::cout << "~R * R:\n";
        A.dump (std::cout) << std::endl;

        std::cout << "\n-------\n" << std::endl;

        std::cout << "L:\n";
        L.dump (std::cout) << std::endl;

        const   DDMatrix AA = L * ~L;

        std::cout << "L * ~L:\n";
        AA.dump (std::cout) << std::endl;

        std::cout << "\n-------\n" << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (6);

        std::cout << "\nTesting 4 matrix multiplication ...\n" << std::endl;

        static  const   size_t  MAT_DEM = 7;

        DDMatrix dmat1 (MAT_DEM, MAT_DEM);
        DDMatrix dmat2 (MAT_DEM, MAT_DEM);
        DDMatrix dmat3 (MAT_DEM, MAT_DEM);
        DDMatrix dmat4 (MAT_DEM, MAT_DEM);

        for (DDMatrix::size_type i = 0; i < MAT_DEM; ++i)
            for (DDMatrix::size_type j = 0; j < MAT_DEM; ++j)  {
                dmat1 (i, j) = 1; // drand48 ();
                dmat2 (i, j) = 1; // drand48 ();
                dmat3 (i, j) = 1; // drand48 ();
                dmat4 (i, j) = 1; // drand48 ();
            }

        const   DDMatrix result = dmat1 * dmat2 * dmat3 * dmat4;

        result.dump (std::cout) << std::endl;

        std::cout << "\n-------\n" << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (8);

        std::cout << "\nSpecial eigen_space() test ...\n" << std::endl;

        DDMatrix dmat (8, 8);

        dmat (0, 0) = 0.185730748;
        dmat (0, 1) = 0.034972668;
        dmat (0, 2) = 0.053844413;
        dmat (0, 3) = 0.0575887;
        dmat (0, 4) = 0.04000943;
        dmat (0, 5) = 0.052731087;
        dmat (0, 6) = 0.053823234;
        dmat (0, 7) = 0.020049257;

        dmat (1, 0) = 0.034972668;
        dmat (1, 1) = 0.296874823;
        dmat (1, 2) = 0.068074652;
        dmat (1, 3) = 0.072808496;
        dmat (1, 4) = 0.050583298;
        dmat (1, 5) = 0.06666709;
        dmat (1, 6) = 0.022682625;
        dmat (1, 7) = 0.025347963;

        dmat (2, 0) = 0.053844413;
        dmat (2, 1) = 0.068074652;
        dmat (2, 2) = 0.703715511;
        dmat (2, 3) = 0.336290958;
        dmat (2, 4) = 0.233636275;
        dmat (2, 5) = 0.102641592;
        dmat (2, 6) = 0.034922489;
        dmat (2, 7) = 0.117078243;

        dmat (3, 0) = 0.0575887;
        dmat (3, 1) = 0.072808496;
        dmat (3, 2) = 0.336290958;
        dmat (3, 3) = 0.804989811;
        dmat (3, 4) = 0.24988311;
        dmat (3, 5) = 0.109779187;
        dmat (3, 6) = 0.037350965;
        dmat (3, 7) = 0.125219748;

        dmat (4, 0) = 0.04000943;
        dmat (4, 1) = 0.050583298;
        dmat (4, 2) = 0.233636275;
        dmat (4, 3) = 0.24988311;
        dmat (4, 4) = 0.388544305;
        dmat (4, 5) = 0.07626848;
        dmat (4, 6) = 0.025949375;
        dmat (4, 7) = 0.086995725;

        dmat (5, 0) = 0.052731087;
        dmat (5, 1) = 0.06666709;
        dmat (5, 2) = 0.102641592;
        dmat (5, 3) = 0.109779187;
        dmat (5, 4) = 0.07626848;
        dmat (5, 5) = 0.674915296;
        dmat (5, 6) = 0.034200406;
        dmat (5, 7) = 0.038219149;

        dmat (6, 0) = 0.053823234;
        dmat (6, 1) = 0.022682625;
        dmat (6, 2) = 0.034922489;
        dmat (6, 3) = 0.037350965;
        dmat (6, 4) = 0.025949375;
        dmat (6, 5) = 0.034200406;
        dmat (6, 6) = 0.078129114;
        dmat (6, 7) = 0.013003577;

        dmat (7, 0) = 0.020049257;
        dmat (7, 1) = 0.025347963;
        dmat (7, 2) = 0.117078243;
        dmat (7, 3) = 0.125219748;
        dmat (7, 4) = 0.086995725;
        dmat (7, 5) = 0.038219149;
        dmat (7, 6) = 0.013003577;
        dmat (7, 7) = 0.097569122;

        DDMatrix eigenvals;
        DDMatrix eigenvecs;

        std::cout << "Original Matrix:\n";

        dmat.dump (std::cout) << std::endl;

        dmat.eigen_space (eigenvals, eigenvecs, true);
        std::cout << "Eigen Values:\n";
        eigenvals.dump (std::cout) << std::endl;
        std::cout << "Eigen Vectors:\n";
        eigenvecs.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }


    {
        const   int pre = std::cout.precision (10);

        std::cout << "\nTesting matrix powers ...\n" << std::endl;

        DLDMatrix dmat (8, 8);

        for (DLDMatrix::size_type i = 0; i < 8; ++i)
            dmat (i, i) = i + 1;

        std::cout << "Original Matrix:\n";
        dmat.dump (std::cout) << std::endl;

        DLDMatrix p;

        dmat.power (p, 1.0 / 3.0, true);
        std::cout << "Third Root Matrix (p):\n";
        p.dump (std::cout) << std::endl;

        const   DLDMatrix a = p * p * p;

        std::cout << "p * p * p Matrix:\n";
        a.dump (std::cout) << std::endl;

        DLDMatrix dmat2 (4, 4);

       // A nice symmetric positive-definite matrix
       //
        dmat2 (0, 0) = 1.0;
        dmat2 (0, 1) = 0.5;
        dmat2 (0, 2) = 0.3333333333;
        dmat2 (0, 3) = 0.25;

        dmat2 (1, 0) = 0.5;
        dmat2 (1, 1) = 0.3333333333;
        dmat2 (1, 2) = 0.25;
        dmat2 (1, 3) = 0.2;

        dmat2 (2, 0) = 0.3333333333;
        dmat2 (2, 1) = 0.25;
        dmat2 (2, 2) = 0.2;
        dmat2 (2, 3) = 0.1667;

        dmat2 (3, 0) = 0.25;
        dmat2 (3, 1) = 0.2;
        dmat2 (3, 2) = 0.1667;
        dmat2 (3, 3) = 0.1429;

        std::cout << "Original Matrix:\n";
        dmat2.dump (std::cout) << std::endl;

        dmat2.power (p, 1.0 / 3.0, false);
        std::cout << "Third Root Matrix (p):\n";
        p.dump (std::cout) << std::endl;

        const   DLDMatrix b = p * p * p;

        if (b != p * p * p)  {
            std::cout << "ERROR: Comparison faild for Power equality (1)\n"
                      << std::endl;
            return (EXIT_FAILURE);
        }
        std::cout << "p * p * p Matrix:\n";
        b.dump (std::cout) << std::endl;

        DLDMatrix p3;

        p.power (p3, 3.0, false);
        //if (p3 != p * p * p)  {
        //    std::cout << "ERROR: Comparison faild for Power equality (2)\n"
        //              << std::endl;
        //    return (EXIT_FAILURE);
        //}
        std::cout << "p ^ 3 Matrix:\n";
        p3.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (10);

        std::cout << "\nSpecial eigen_space() test ...\n" << std::endl;

        DDMatrix dmat (5, 5, 0.0);

        dmat (0, 0) = 10.0; // Lamda on the diagonal
        dmat (1, 1) = 10.0; // Lamda on the diagonal
        dmat (2, 2) = 10.0; // Lamda on the diagonal
        dmat (3, 3) = 10.0; // Lamda on the diagonal
        dmat (4, 4) = 10.0; // Lamda on the diagonal

        dmat (1, 0) = 1.0;
        dmat (2, 1) = 1.0;
        dmat (3, 2) = 1.0;
        dmat (4, 3) = 1.0;

        std::cout << "Original Matrix:\n";

        dmat.dump (std::cout) << std::endl;

        DDMatrix eigenvals;
        DDMatrix eigenvecs;

        dmat.eigen_space (eigenvals, eigenvecs, true);
        std::cout << "Eigen Values:\n";
        eigenvals.dump (std::cout) << std::endl;
        std::cout << "Eigen Vectors:\n";
        eigenvecs.dump (std::cout) << std::endl;

        const   DDMatrix dmat2 = ! dmat;

        std::cout << "The inverted matrix:" << std::endl;
        dmat2.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (10);

        std::cout << "\nTesting 1X1 matrix arithmatic ...\n" << std::endl;

        DDMatrix posvec (1, 4);
        DDMatrix totretvec (1, 4);
        DDMatrix sigma (4, 4);
        DDMatrix dmat1 (1, 1);
        DDMatrix dmat2 (1, 1);

        dmat1 (0, 0) = 3.0;
        dmat2 (0, 0) = 3.0;
        posvec (0, 0) = 1.0;
        posvec (0, 1) = 2.0;
        posvec (0, 2) = 3.0;
        posvec (0, 3) = 4.0;
        totretvec (0, 0) = 1.0;
        totretvec (0, 1) = 1.0;
        totretvec (0, 2) = 1.0;
        totretvec (0, 3) = 1.0;

        sigma (0, 0) = 1.0;
        sigma (0, 1) = 1.0;
        sigma (0, 2) = 1.0;
        sigma (0, 3) = 1.0;
        sigma (1, 0) = 1.0;
        sigma (1, 1) = 1.0;
        sigma (1, 2) = 1.0;
        sigma (1, 3) = 1.0;
        sigma (2, 0) = 1.0;
        sigma (2, 1) = 1.0;
        sigma (2, 2) = 1.0;
        sigma (2, 3) = 1.0;
        sigma (3, 0) = 1.0;
        sigma (3, 1) = 1.0;
        sigma (3, 2) = 1.0;
        sigma (3, 3) = 1.0;

        const   DDMatrix mdmat = dmat1 * dmat2;
        const   DDMatrix admat = dmat1 + dmat2;

        mdmat.dump (std::cout) << std::endl;
        admat.dump (std::cout) << std::endl;

        const   DDMatrix dmat3 = posvec * ~totretvec - dmat1;

        dmat3.dump (std::cout) << std::endl;

        const   DDMatrix dmat4 = dmat1 * (posvec * sigma * ~posvec);

        dmat4.dump (std::cout) << std::endl;

        const   DDMatrix dmat5 =
            posvec * ~totretvec - dmat1 * (posvec * sigma * ~posvec);

        dmat5.dump (std::cout) << std::endl;

        std::cout.precision (pre);
    }

    {
        const   int pre = std::cout.precision (10);

        std::cout << "\nTesting Complex ...\n" << std::endl;

        Complex<double> c1(20, 5);
        Complex<double> c2(-3, -40);

        std::cout << "c1 = " << c1 << std::endl;
        std::cout << "c2 = " << c2 << std::endl;

        auto    filter_func = [](const Complex<double> &cpx) {
            return (cpx.real() > 0);
        };
        auto    trans_func = [](const Complex<double> &cpx) {
            const Complex<double>   local_c(cpx.real() * 10, cpx.imaginary());
            return (local_c);
        };

        std::cout << "c1 filtered = " << c1.filter(filter_func) << std::endl;
        std::cout << "c2 filtered = " << c2.filter(filter_func) << std::endl;
        std::cout << "c2 transformed = "
                  << c2.transform(trans_func) << std::endl;

        std::cout.precision (pre);
    }
    /*
    {
        std::cout << "\nTesting Eigen performace ...\n" << std::endl;

        const size_t    dem = 500;

        Eigen::MatrixXd emat(dem, dem);
        DDMatrix        hmat (dem, dem);
        int             count = 0;

        for (DDMatrix::size_type i = 0; i < dem; ++i)  {
            for (DDMatrix::size_type j = 0; j < dem; ++j)  {
                emat (i, j) = double(++count);
                hmat (i, j) = double(count);
            }
        }

        const time_t    hstart = time(nullptr);

        for (DDMatrix::size_type i = 0; i < 10; ++i)  {
            DDMatrix    result = hmat * hmat * hmat;

            result(3, 4) = 123.4;
        }

        const time_t    hend = time(nullptr);
        const time_t    estart = time(nullptr);

        for (DDMatrix::size_type i = 0; i < 10; ++i)  {
            Eigen::MatrixXd result = emat * emat * emat;

            result(3, 4) = 123.4;
        }

        const time_t    eend = time(nullptr);

        std::cout << "Eigen took: " << eend - estart << std::endl
                  << "Hossein Took: " << hend - hstart << std::endl;
    }
    */

    // {
    //     std::cout << "\nTesting read() ...\n" << std::endl;

    //     /*
    //     const size_t    dim = 100;
    //     DDMatrix        dmat (dim, dim);

    //     for (DDMatrix::size_type i = 0; i < dim; ++i)
    //         for (DDMatrix::size_type j = 0; j < dim; ++j)
    //             dmat (i, j) = ::drand48 ();
    //     dmat.write (std::cout);
    //     std::cout << std::endl;
    //     */

    //     DDMatrix    dmat;

    //     dmat.read("matrix_test_file.csv");
    //     std::cout << "0.396464773760=" << std::setprecision(12) << dmat(0, 0)
    //               << std::endl;
    //     std::cout << "0.867329613094=" << std::setprecision(12) << dmat(99, 99)
    //               << std::endl;
    //     std::cout << "0.471682027561=" << std::setprecision(12) << dmat(10, 88)
    //               << std::endl;
    //     std::cout << "0.306674009279=" << std::setprecision(12) << dmat(98, 23)
    //               << std::endl;
    //     std::cout << "0.992849997811=" << std::setprecision(12) << dmat(55, 55)
    //               << std::endl;
    // }

    return (EXIT_SUCCESS);
}

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
