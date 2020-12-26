f1 = @(x)4 / (1 + x^2);
f2 = @(x)sqrt(x);

trapezoidal(f1, 0, 1, 10)
simpson(f1, 0, 1, 10)
midpoint(f1, 0, 1, 10)

trapezoidal(f2, 0, 1, 10)
simpson(f2, 0, 1, 10)
midpoint(f2, 0, 1, 10)
