function x = h02q01()
   A = [1 -1 3; 1 1 0; 3 -2 1];
   B = [2; 4; 1];
   
   [A, B] = gaussianel(A, B);   % implemented in gaussianel.m 
   x = backwardsub(A, B);       % implemented in backwardsub.m
end