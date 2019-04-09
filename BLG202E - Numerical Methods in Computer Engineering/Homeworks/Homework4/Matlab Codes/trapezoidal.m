function C = trapezoidal(func, a, b, n)
    C = 0;              % answer
    d = (b - a) / n;    % find the lenght of every segment
    
    for i = 0 : n - 1
        C = C + d*(func(a + i*d) + func(a + (i+1)*d)) / 2;
    end
end