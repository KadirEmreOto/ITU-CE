
X = [1, 2, 3, 5];   
Y = [2, 1, 3, -4];

a = [];
b = [];
bound = 5;

plot(X, Y, 'O');
hold on

for i = 1 : 0.1 : bound
    a = [a i];
    b = [b newton(X, Y, i)]; % newton: implemented in newton.m
end

plot(a, b);
hold off
