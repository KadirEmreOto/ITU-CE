function C = simpson(func, a, b, n)
    d = (b - a) / n;                        % find the lenght of every segment
    C = func(a) + func(b) + 4*func(b-d);    % answer
    
    for k = 1 : n / 2 - 1
        C = C + 4*(func(a + (2*k - 1)*d)) + 2*(func(a + 2*k*d));
    end    
    
    C = C * d / 3;
end