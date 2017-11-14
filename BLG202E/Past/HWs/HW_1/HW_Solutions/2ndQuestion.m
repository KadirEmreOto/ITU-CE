syms x
x0 = 50; %%Initial x
xn = 0; %%Next value of x
i = 0; %%Iteration step
err = 1; %% Absolute relative approximate error
fprintf('Initial x = %.20f\n', x0)
while err > 10^-2
f = inline((40*x^1.5)-875*x+35000, 'x'); %%f(x)
derivative = inline(diff((40*x^1.5)-875*x+35000, 'x')); %%f'(x)
xn = x0 - f(x0)/derivative(x0); %%Newton-Raphson Method Iteration Formula
err = (xn-x0)/xn*100;
x0 = xn;
i = i +1;
fprintf('Iteration %d: x = %.20f, err = %.20f\n', i, xn, err)
end


