
function x = forwardsub(A, B)
   % Forward substitution
   n = length(A);
   x = zeros(n:1);
   
   x(1,1) = B(1,1) / A(1, 1);
   for i = 2 : n
       x(i,1) = B(i,1);
       for j = 1 : i - 1
           x(i,1) = x(i,1) - A(i, j) * x(j,1);
       end
       x(i,1) = x(i,1) / A(i, i);
   end
end