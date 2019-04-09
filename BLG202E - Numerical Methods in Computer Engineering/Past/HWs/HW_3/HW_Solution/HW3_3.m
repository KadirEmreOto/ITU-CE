syms x
syms h
f = inline(2.718182.^x, 'x');
fpp = inline(1/(2*h.^3 )*(f(2*h)-f(-2*h)-2*(f(h)- f(-h))),'h'); %% approximate derivation
k = [1:1:9]
h = 10.^-k
result = fpp(h)
err = abs(1-result) %% absolute error
plot(k,err)
   