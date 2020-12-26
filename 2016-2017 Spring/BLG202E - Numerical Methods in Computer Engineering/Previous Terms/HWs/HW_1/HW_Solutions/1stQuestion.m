x0 = 1.2; %%Initial Value
i = -20:1:0; 
h = 10.^i; %%Desired value of h form 1e-20 to 1

syms x
derivative = inline(diff(sin(x), 'x')); %%derivative of sin(x) without approximation

f0 = cos((2*x0 + h) / 2);
f1 = sin(h/2);
approximationDerivativeWithoutCancellationError = (2.*f0.*f1)./h;
err1 = abs(approximationDerivativeWithoutCancellationError - derivative(x0));

approximationDerivativeWithCancellationError = (sin(x0 + h) - sin(x0)) ./ h;
err2 = abs(approximationDerivativeWithCancellationError - derivative(x0));

fprintf('h : %d\n\n', h')
fprintf('Approximation without cancellation error: %f\n\n', approximationDerivativeWithoutCancellationError')
fprintf('Approximation without cancellation error: %f\n\n', approximationDerivativeWithCancellationError')
