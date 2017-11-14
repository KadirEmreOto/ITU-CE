%%Elif AK 150130009
syms x

f = inline(1+(2.718182.^-x)*sin(8*x^(2/3)), 'x');
m = [2,4,8,16,32,60,70,100];
h = 2./m;
i = 1;
first = 0;
second = 0 + h;
mid_first = (first + second) / 2;
result = 0;
mid = 0;
while i < 9
    mid = mid_first(i);
        while mid < 2 
        result = result + f(mid);
        mid = mid + h(i);
        end
    result = result * h(i);
    results(i) = result;
    mid = 0;
    result = 0;
    i = i + 1
end
results
plot(results)
