%%Elif AK 150130009
s = [.00005,.0001,.0005,.001];
h = [.00005,.0001,.0005,.001]*pi;
N = 1./[.00005,.0001,.0005,.001];
N = N/2;
y0 = 1; 

result_forward = [0,0,0,0];
i = 1;
while i < 5
    y = y0; t = 0;
    for k=1:N(i)
    y = y + h(i)*(-1000*(y - cos (t)) - sin (t));
    t = t + h(i);
    end
    result_forward(i) = y;
    i
    i = i + 1;
end

plot(result_forward)
i = 1;
while i < 5
    fprintf('Approximation Result for h = %i: %i\n', s(i), result_forward(i))
    i = i + 1;
end
