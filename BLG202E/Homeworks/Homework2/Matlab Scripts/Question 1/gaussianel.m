
function [A, B] = gaussianel(A, B)
   % Gaussian elimination
   n = length(A);
   
   for k = 1 : n-1
       for i = k+1 : n
           l = A(i, k) / A(k, k);
           for j = k : n
               A(i, j) = A(i, j) - l*A(k, j);
           end
           B(i) = B(i) - l*B(k);
       end
   end
end