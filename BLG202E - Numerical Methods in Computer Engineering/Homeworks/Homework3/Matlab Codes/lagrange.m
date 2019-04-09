function y = lagrange(x_set, y_set, x)
    n = length(x_set);
    y = 0;                                      
    for i = 1 : n
        p = 1;                                  % lagrange product
        for j = 1 : n
            if i ~= j
                p = p * (x - x_set(j));         % x  - xj
                p = p / (x_set(i) - x_set(j));  % xi - xj
            end
        end
        y = y + p * y_set(i);                   % determine y = f(x)
    end 
end