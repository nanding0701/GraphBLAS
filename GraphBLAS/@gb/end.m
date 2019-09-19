function i = end (G, k, ndims)
%END Last index in an indexing expression for a GraphBLAS matrix.

% SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
% http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

if (ndims == 1)
    if (~isvector (G))
        error ('gb:unsupported', 'Linear indexing not supported') ;
    end
    i = length (G) ;
elseif (ndims == 2)
    s = size (G) ;
    i = s (k) ;
else
    error ('gb:unsupported', '%dD indexing not supported', ndims) ;
end
