function y = newton(x_set, y_set, x)
    y = 0;
    n = length(x_set);
    
    for k = 1 : n
       p = 1;
       for i = 1 : k-1
          p = p * (x - x_set(i));
       end
       y = y + p * divided_difference(x_set, y_set, x_set(1:k));
       % divided_difference: implemented in divided_difference.m
    end
end