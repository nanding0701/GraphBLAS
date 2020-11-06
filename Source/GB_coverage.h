//------------------------------------------------------------------------------
// GB_coverage.h: for coverage tests in Tcov
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// These global values are visible only from the GraphBLAS/Tcov tests.

#ifndef GB_COVERAGE_H
#define GB_COVERAGE_H

#ifdef GBCOVER
#define GBCOVER_MAX 20000
GB_PUBLIC int64_t GB_cov [GBCOVER_MAX] ;
GB_PUBLIC int GB_cover_max ;
#endif

#endif
