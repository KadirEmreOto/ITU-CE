syms x
x0 = 2; %%Initial x
xn = 0; %%Next value of x
i = 0; %%Iteration step
err = 1; %% Absolute relative approximate error
fprintf('Initial x = %f\n', x0)
while err > 10^-2
f = inline((x+10)^(1/4), 'x'); %%g(x)
xn = f(x0); %%Fixed Point Iteration
err = (xn-x0)/xn*100;
err = abs(err);
x0 = xn;
i = i +1;
fprintf('Iteration %d: x = %.20f, err = %.20f\n', i, xn, err)
end