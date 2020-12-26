f = @(x)3*x^2 + 25*x + 0.2;         % given equation
I = @(x)x^3 + 12.5*x^2 + 0.2*x;     % indefinite integral of f

a = 0;                              % lower bound
b = 2;                              % upper bound
R = I(b) - I(a);                    % real value

X = [];                             % axis -> segment count (n)
Y = [];                             % ordinate -> relative error (E)

for n = 1 : 5
    C = trapezoidal(f, a, b, n);    % implemented in trapezoidal.m
    
    E = abs(C - R) / R * 100;       % calculate error
    X = [X n];                      
    Y = [Y E];
end

figure
plot(X, Y)                         % plot
title('Graph of Relative Errors of Trapezoidal Rule')
xlabel('n: segment count') % x-axis label
ylabel('E: relative error (%)') % y-axis label
