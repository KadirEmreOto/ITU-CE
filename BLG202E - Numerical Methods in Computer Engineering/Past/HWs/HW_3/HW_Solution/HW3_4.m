syms x
syms h
fpp = inline((sin(1.2+h)-2*sin(1.2)+sin(1.2-h))/h^2, 'h'); %% approximate derivation
true_value = -sin(1.2) %% absolute derivative of sin(x)
k = [0:.5:8]
h = 10.^-k
result = fpp(h)
err = abs(true_value-result) %% absolute error
loglog(k,err)
   