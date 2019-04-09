
function [l, u] = mylu(matrix)
    % LU Decomposition: ( Complexity: O(n^3) )
    n = length(matrix);                 
    l = eye(n); % identity matrix to hold unit lower triangular matrix (ultm)
    u = matrix; %  copy of matrix to hold unit upper triangular matrix (uutm)
    for i = 1: n - 1
        for j = i+1: n % for every row 
            l(j, i) = u(j, i) / u(i, i);  % lij: elements of ultm
            for k = 1: n
                u(j, k) = u(j, k) - l(j, i) * u(i, k);  % adjust uutm
            end
        end
    end
end