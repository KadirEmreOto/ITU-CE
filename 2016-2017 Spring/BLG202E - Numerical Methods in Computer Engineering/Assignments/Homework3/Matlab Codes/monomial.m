function y = monomial(x_set, y_set, x)
    n = length(x_set);
    vandermonde = zeros(n, n);
    
    for i = 1:n
        for j = 0:n-1
            vandermonde(i, j+1) = x_set(i) ^ j; % fill vandermonde matrix
        end
    end
    
    c = inv(vandermonde) * y_set' % calculate constant variables
    y = 0;
    for i = 0:n-1
       y = y + (x ^ i)*c(i+1);    % determine value of f(x)
    end
end