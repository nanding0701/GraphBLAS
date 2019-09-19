function C = zeros (varargin)
%ZEROS an all-zero matrix, the same type as G.
% C = zeros (m, n, 'like', G) or C = zeros ([m n], 'like', G) returns
% an m-by-n matrix with no entries, of the same type as G.
%
% See also gb/ones, gb/false, gb/true.

% SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
% http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

G = varargin {end} ;
if (nargin == 4)
    if (~isequal (varargin {3}, 'like'))
        error ('gb:error', 'usage: zeros (m, n, ''like'', G)') ;
    end     %#ok<UNRCH>
    m = varargin {1} ;
    n = varargin {2} ;
elseif (nargin == 3)
    if (~isequal (varargin {2}, 'like'))
        error ('gb:error', 'usage: zeros ([m n], ''like'', G)') ;
    end     %#ok<UNRCH>
    mn = varargin {1} ;
    m = mn (1) ;
    n = mn (2) ;
else
    error ('gb:error', 'usage: zeros (m, n, ''like'', G)') ;
end

C = gb (m, n, gb.type (G)) ;
