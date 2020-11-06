function test169
%TEST169 C<M>=A+B with C sparse, M hyper, A and B sparse

% SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
% http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

rng ('default') ;

fprintf ('test169:\n') ;

n = 10 ;
C = GB_spec_random (n, n, 0.5, 1, 'double') ;
C.sparsity = 2 ;    % sparse

M = GB_spec_random (n, n, 0.02, 1, 'double') ;
M.sparsity = 1 ;    % hypersparse

A = GB_spec_random (n, n, 0.5, 1, 'double') ;
A.sparsity = 2 ;    % sparse

B = GB_spec_random (n, n, 0.5, 1, 'double') ;
B.sparsity = 2 ;    % sparse

desc = struct ('mask', 'complement') ;

GrB.burble (1) ;
C1 = GB_spec_Matrix_eWiseAdd (C, M, [ ], 'plus', A, B, desc) ;
C2 = GB_mex_Matrix_eWiseAdd  (C, M, [ ], 'plus', A, B, desc) ;
GB_spec_compare (C1, C2) ;
GrB.burble (0) ;

fprintf ('test169: all tests passed\n') ;
