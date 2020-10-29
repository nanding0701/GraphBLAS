//------------------------------------------------------------------------------
// gb_get_sparsity: determine the sparsity of a matrix result 
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// gb_get_sparsity determines the sparsity of a result matrix C, which may be
// computed from one or two input matrices A and B.  The following rules are
// used, in order:

// (1) GraphBLAS operations of the form C = GrB.method (Cin, ...) use the
//      sparsity of Cin for the new matrix C.

// (2) If the sparsity is determined by the descriptor to the method, then that
//      determines the sparsity of C.

// (3) If both A and B are present and both matrices, the sparsity of C is
//      A_sparsity | B_sparsity

// (4) If A is present, then the sparsity of C is A_sparsity.

// (5) If B is present, then the sparsity of C is B_sparsity.

// (6) Otherwise, the global default sparsity is used for C.

// for GxB_SPARSITY can be any sum or bitwise OR of these 4 values:
// GxB_HYPERSPARSE 1   // store matrix in hypersparse form
// GxB_SPARSE      2   // store matrix as sparse form (compressed vector)
// GxB_BITMAP      4   // store matrix as a bitmap
// GxB_FULL        8   // store matrix as full; all entries must be present

// the default is to store the matrix in any form:
// GxB_AUTO_SPARSITY 15

#include "gb_matlab.h"

GxB_Format_Value gb_get_sparsity        // 1 to 15
(
    GrB_Matrix A,                       // may be NULL
    GrB_Matrix B,                       // may be NULL
    int sparsity_default                // may be 0
)
{

    int sparsity = GxB_AUTO_SPARSITY ;
    int A_sparsity = 0, B_sparsity = 0 ;
    GrB_Index nrows, ncols ;

    //--------------------------------------------------------------------------
    // get the sparsity of the matrices A and B
    //--------------------------------------------------------------------------

    if (A != NULL)
    {
        OK (GrB_Matrix_nrows (&nrows, A)) ;
        OK (GrB_Matrix_ncols (&ncols, A)) ;
        if (nrows > 1 || ncols > 1)
        {
            OK (GxB_Matrix_Option_get (A, GxB_SPARSITY, &A_sparsity)) ;
            printf ("A sparsity: %d\n", A_sparsity) ;
        }
    }

    if (B != NULL)
    {
        OK (GrB_Matrix_nrows (&nrows, B)) ;
        OK (GrB_Matrix_ncols (&ncols, B)) ;
        if (nrows > 1 || ncols > 1)
        {
            OK (GxB_Matrix_Option_get (B, GxB_SPARSITY, &B_sparsity)) ;
            printf ("B sparsity: %d\n", B_sparsity) ;
        }
    }

    //--------------------------------------------------------------------------
    // determine the sparsity of C
    //--------------------------------------------------------------------------

    if (sparsity_default != 0)
    { 
        // (2) the sparsity is defined by the descriptor to the method
        sparsity = sparsity_default ;
        printf ("(2) %d\n", sparsity) ;
    }
    else if (A_sparsity > 0 && B_sparsity > 0)
    {
        // (3) C is determined by the sparsity of A and B
        sparsity = A_sparsity | B_sparsity ;
        printf ("(3) %d\n", sparsity) ;
    }
    else if (A_sparsity > 0)
    {
        // (4) get the sparsity of A
        sparsity = A_sparsity ;
        printf ("(4) %d\n", sparsity) ;
    }
    else if (B_sparsity > 0)
    {
        // (5) get the sparsity of B
        sparsity = B_sparsity ;
        printf ("(5) %d\n", sparsity) ;
    }
    else
    {
        // (6) use the default sparsity
        sparsity = GxB_AUTO_SPARSITY ;
        printf ("(6) %d\n", sparsity) ;
    }

    return (sparsity) ;
}
