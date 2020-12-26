
function x = backwardsub(A, B)
   % Backward substitution
   n = length(A);
   x = zeros(n:1);
   
   x(n, 1) = B(n, 1) / A(n, n);
   for i = n-1 : -1 : 1
       x(i, 1) = B(i, 1);
       for j = i+1 : n
           x(i, 1) = x(i, 1) - A(i, j) * x(j, 1);
       end
       x(i, 1) = x(i, 1) / A(i, i);
   end
end