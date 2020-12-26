function a = divided_difference(x_set, y_set, x)
    n = length(x);
    m = length(x_set);
    
    a = 0;

    if n == 1
       for i = 1 : m
           if x_set(i) == x(1);
               a = y_set(i);
               break
           end
       end
    else
       f = divided_difference(x_set, y_set, x(1:n-1));
       s = divided_difference(x_set, y_set, x(2:n));
       
       a = (s - f) / (x(n) - x(1));
    end       
end