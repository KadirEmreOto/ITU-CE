% Example Usage: question2(0, 10, 10, 1e-8)
function x = question2(a, b, nprobe, atol)
    f = @(x)2*cosh(x/4)-x;      
    d = @(x)0.5*sinh(x/4)-1;            % derivative of f
    
    function root = newton(x0, atol)
        root = x0 - f(x0)/d(x0);
        
        while (abs(root-x0)>atol)
            x0 = root;
            root = x0 - f(x0)/d(x0);
        end
    end

    x = zeros(1, 2);
    t = 1;
    for i = a:(b-a)/nprobe:b
        if f(i) * f(i+(b-a)/nprobe) < 0
          
            x(t) = newton(i, atol);
            t = t+1;
        end
    end
end