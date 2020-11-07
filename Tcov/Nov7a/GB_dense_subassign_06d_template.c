//------------------------------------------------------------------------------
// GB_dense_subassign_06d_template: C<A> = A where C is dense or bitmap
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

{

    //--------------------------------------------------------------------------
    // get C and A
    //--------------------------------------------------------------------------

    ASSERT (!GB_ZOMBIES (A)) ;
    ASSERT (GB_JUMBLED_OK (A)) ;
    ASSERT (!GB_PENDING (A)) ;

    const int64_t  *GB_RESTRICT Ap = A->p ;
    const int64_t  *GB_RESTRICT Ah = A->h ;
    const int64_t  *GB_RESTRICT Ai = A->i ;
    const int8_t   *GB_RESTRICT Ab = A->b ;
    const GB_ATYPE *GB_RESTRICT Ax = (GB_ATYPE *) A->x ;
    const int64_t avlen = A->vlen ;
    const bool A_is_bitmap = GB_IS_BITMAP (A) ;
    const bool A_is_dense = GB_as_if_full (A) ;
    const int64_t anz = GB_NNZ_HELD (A) ;

    GB_CTYPE *GB_RESTRICT Cx = (GB_CTYPE *) C->x ;
    int8_t   *GB_RESTRICT Cb = C->b ;
    const int64_t cvlen = C->vlen ;
    const bool C_is_bitmap = GB_IS_BITMAP (C) ;

    //--------------------------------------------------------------------------
    // C<A> = A
    //--------------------------------------------------------------------------

    int64_t p ;
    int64_t cnvals = C->nvals ;     // for C bitmap

    if (A_is_dense)
    {   GB_cov[1168]++ ;
// covered (1168): 2929

        //----------------------------------------------------------------------
        // A is dense: all entries present
        //----------------------------------------------------------------------

        if (C_is_bitmap)
        {   GB_cov[1169]++ ;
// covered (1169): 780

            //------------------------------------------------------------------
            // C is bitmap, A is dense
            //------------------------------------------------------------------

            if (Mask_struct)
            {
GB_GOTCHA ;                // C<A,struct>=A with C bitmap, A dense
                #pragma omp parallel for num_threads(nthreads) schedule(static)
                for (p = 0 ; p < anz ; p++)
                {   GB_cov[1170]++ ;
// NOT COVERED (1170):
                    // Cx [p] = Ax [p]
                    GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                    Cb [p] = 1 ;
                }
                cnvals = anz ;
            }
            else
            {
                // C<A>=A with C bitmap, A dense
                #pragma omp parallel for num_threads(nthreads) schedule(static)\
                    reduction(+:cnvals)
                for (p = 0 ; p < anz ; p++)
                {
                    if (GB_AX_MASK (Ax, p, asize))
                    {   GB_cov[1171]++ ;
// covered (1171): 20619
                        // Cx [p] = Ax [p]
                        GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                        cnvals += (Cb [p] == 0) ;
                        Cb [p] = 1 ;
                    }
                }
            }

        }
        else
        {

            //------------------------------------------------------------------
            // C is hypersparse, sparse, or full, with all entries present
            //------------------------------------------------------------------

            if (Mask_struct)
            {
GB_GOTCHA ;                // C<A,struct>=A with C sparse/hyper/full
                #pragma omp parallel for num_threads(nthreads) schedule(static)
                for (p = 0 ; p < anz ; p++)
                {   GB_cov[1172]++ ;
// NOT COVERED (1172):
                    // Cx [p] = Ax [p]
                    GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                }
            }
            else
            {
                // C<A>=A with C sparse/hyper/full
                #pragma omp parallel for num_threads(nthreads) schedule(static)
                for (p = 0 ; p < anz ; p++)
                {
                    if (GB_AX_MASK (Ax, p, asize))
                    {   GB_cov[1173]++ ;
// covered (1173): 63525
                        // Cx [p] = Ax [p]
                        GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                    }
                }
            }
        }

    }
    else if (A_is_bitmap)
    {

        //----------------------------------------------------------------------
        // A is bitmap
        //----------------------------------------------------------------------

        if (C_is_bitmap)
        {

            //------------------------------------------------------------------
            // C is bitmap, A is bitmap
            //------------------------------------------------------------------

            if (Mask_struct)
            {
                // C<A,struct>=A with A and C bitmap
                #pragma omp parallel for num_threads(nthreads) schedule(static)\
                    reduction(+:cnvals)
                for (p = 0 ; p < anz ; p++)
                {
                    if (Ab [p])
                    {   GB_cov[1174]++ ;
// covered (1174): 1062
                        // Cx [p] = Ax [p]
                        GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                        cnvals += (Cb [p] == 0) ;
                        Cb [p] = 1 ;
                    }
                }
            }
            else
            {
GB_GOTCHA ;           // C<A>=A with A and C bitmap
                #pragma omp parallel for num_threads(nthreads) schedule(static)\
                    reduction(+:cnvals)
                for (p = 0 ; p < anz ; p++)
                {
                    if (Ab [p] && GB_AX_MASK (Ax, p, asize))
                    {   GB_cov[1175]++ ;
// NOT COVERED (1175):
                        // Cx [p] = Ax [p]
                        GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                        cnvals += (Cb [p] == 0) ;
                        Cb [p] = 1 ;
                    }
                }
            }

        }
        else
        {

            //------------------------------------------------------------------
            // C is hypersparse, sparse, or full, with all entries present
            //------------------------------------------------------------------

            if (Mask_struct)
            {
                // C<A,struct>=A with A bitmap, and C hyper/sparse/full
                // this method is used by LAGraph_bfs_parent when q is
                // a bitmap and pi is full.
                #pragma omp parallel for num_threads(nthreads) schedule(static)
                for (p = 0 ; p < anz ; p++)
                {
                    // Cx [p] = Ax [p]
                    if (Ab [p])
                    {   GB_cov[1176]++ ;
// covered (1176): 3261
                        GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                    }
                }
            }
            else
            {
GB_GOTCHA ;                // C<A>=A with A bitmap, and C hyper/sparse/full
                #pragma omp parallel for num_threads(nthreads) schedule(static)
                for (p = 0 ; p < anz ; p++)
                {
                    if (Ab [p] && GB_AX_MASK (Ax, p, asize))
                    {   GB_cov[1177]++ ;
// NOT COVERED (1177):
                        // Cx [p] = Ax [p]
                        GB_COPY_A_TO_C (Cx, p, Ax, p) ;
                    }
                }
            }
        }

    }
    else
    {

        //----------------------------------------------------------------------
        // A is hypersparse or sparse; C is dense or a bitmap
        //----------------------------------------------------------------------

        int taskid ;
        #pragma omp parallel for num_threads(nthreads) schedule(dynamic,1) \
            reduction(+:cnvals)
        for (taskid = 0 ; taskid < ntasks ; taskid++)
        {

            // if kfirst > klast then taskid does no work at all
            int64_t kfirst = kfirst_slice [taskid] ;
            int64_t klast  = klast_slice  [taskid] ;

            //------------------------------------------------------------------
            // C<A(:,kfirst:klast)> = A(:,kfirst:klast)
            //------------------------------------------------------------------

            for (int64_t k = kfirst ; k <= klast ; k++)
            {

                //--------------------------------------------------------------
                // find the part of A(:,k) to be operated on by this task
                //--------------------------------------------------------------

                int64_t j = GBH (Ah, k) ;
                int64_t pA_start, pA_end ;
                GB_get_pA (&pA_start, &pA_end, taskid, k,
                    kfirst, klast, pstart_slice, Ap, avlen) ;

                // pC points to the start of C(:,j) if C is dense or bitmap
                int64_t pC = j * cvlen ;

                //--------------------------------------------------------------
                // C<A(:,j)> = A(:,j)
                //--------------------------------------------------------------

                if (Mask_struct)
                {
                    if (C_is_bitmap)
                    {
                        // C<A,struct>=A with C bitmap, A sparse
                        GB_PRAGMA_SIMD_VECTORIZE
                        for (int64_t pA = pA_start ; pA < pA_end ; pA++)
                        {   GB_cov[1178]++ ;
// covered (1178): 742
                            int64_t p = pC + GBI (Ai, pA, avlen) ;
                            // Cx [p] = Ax [pA]
                            GB_COPY_A_TO_C (Cx, p, Ax, pA) ;
                            cnvals += (Cb [p] == 0) ;
                            Cb [p] = 1 ;
                        }
                    }
                    else
                    {
                        // C<A,struct>=A with C full, A sparse
                        GB_PRAGMA_SIMD_VECTORIZE
                        for (int64_t pA = pA_start ; pA < pA_end ; pA++)
                        {   GB_cov[1179]++ ;
// covered (1179): 4492
                            int64_t p = pC + GBI (Ai, pA, avlen) ;
                            // Cx [p] = Ax [pA]
                            GB_COPY_A_TO_C (Cx, p, Ax, pA) ;
                        }
                    }
                }
                else
                {
                    if (C_is_bitmap)
                    {
GB_GOTCHA ;                        // C<A,struct>=A with C bitmap, A sparse
                        GB_PRAGMA_SIMD_VECTORIZE
                        for (int64_t pA = pA_start ; pA < pA_end ; pA++)
                        {
                            if (GB_AX_MASK (Ax, pA, asize))
                            {   GB_cov[1180]++ ;
// NOT COVERED (1180):
                                int64_t p = pC + GBI (Ai, pA, avlen) ;
                                // Cx [p] = Ax [pA]
                                GB_COPY_A_TO_C (Cx, p, Ax, pA) ;
                                cnvals += (Cb [p] == 0) ;
                                Cb [p] = 1 ;
                            }
                        }
                    }
                    else
                    {
GB_GOTCHA ;                        // C<A,struct>=A with C dense, A sparse
                        GB_PRAGMA_SIMD_VECTORIZE
                        for (int64_t pA = pA_start ; pA < pA_end ; pA++)
                        {
                            if (GB_AX_MASK (Ax, pA, asize))
                            {   GB_cov[1181]++ ;
// NOT COVERED (1181):
                                int64_t p = pC + GBI (Ai, pA, avlen) ;
                                // Cx [p] = Ax [pA]
                                GB_COPY_A_TO_C (Cx, p, Ax, pA) ;
                            }
                        }
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    // log the number of entries in the C bitmap
    //--------------------------------------------------------------------------

    if (C_is_bitmap)
    {   GB_cov[1182]++ ;
// covered (1182): 840
        C->nvals = cnvals ;
    }
}
