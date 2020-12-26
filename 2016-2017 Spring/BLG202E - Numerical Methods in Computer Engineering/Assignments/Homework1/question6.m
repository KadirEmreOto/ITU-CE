% Example Usage: question6(2, 1e-8)
function [root, n] = question6(x0, atol)
    e = exp(1.0);
    f = @(x) (x-1)^2 * e^x; 
    d = @(x) 2*(x-1) * e^x + (x-1)^2 * e^x; % derivative of f
    
    function [root, n] = newton(x0, atol)   % n: iteration count
        n = 0;
        root = x0 - f(x0)/d(x0);
        
        while (abs(root-x0)>atol)
            n = n + 1;
            x0 = root;
            root = x0 - f(x0)/d(x0);
        end
    end
    
    [root, n] = newton(x0, atol)              
end