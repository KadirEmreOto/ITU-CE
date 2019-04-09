%%Elif AK 150130009
syms x
h = 1;
xi = 0;
xs = 4;
yn = 0; %% new approximate
yp = f(xi); %% previous approximate
sum = 0;


f = inline((2.718182.^2*x)*x, 'x');
%%Isimp = h/3*(f(xi) + 2*f(xi+2*h) + 4*f(xi + h) + 4*f(xi+3*h)+f(xs))
Isimp = h/3*(f(0) + 2*f(2) + 4*f(1) + 4*f(3)+f(4))

fun = @(x) exp(2*x).*x;
ExactIntegral = integral(fun, 0, 4)
yu = 5256
error = abs(ExactIntegral - yu)/ExactIntegral


