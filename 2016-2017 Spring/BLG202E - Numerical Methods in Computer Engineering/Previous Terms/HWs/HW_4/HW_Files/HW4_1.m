%%Elif AK 150130009
syms x

f = inline((2.718182.^-x)*sin(x), 'x');
h = 0.3
xi = 0;
yn = 0; %% new approximate
yp = f(xi); %% previous approximate
sum = 0;

while xi < 3
xi = xi + h;    
sum = sum + 2*f(xi)
yp = yn;
end

sum = sum + f(3);
result = sum * h/2
