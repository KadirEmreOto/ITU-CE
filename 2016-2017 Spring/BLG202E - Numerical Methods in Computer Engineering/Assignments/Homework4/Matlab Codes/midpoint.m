function C = midpoint(func, a, b, n)
    d = (b - a) / n;                        % find the lenght of every segment
    C = 0;                                  % answer
    
    for i = 1 : n
        C = C + func(a + (i-0.5)*d);
    end    
    
    C = C * d;
end