Execute the following:
n = input(’Problem size = ’);
S = rand(n,n);
f = rand(n,1);
ntry = 50;
tic
for i = 1:ntry
x = S\f;
end
toc
tic
for i = 1:ntry
Sinv = inv(S);
x = Sinv*f;
end
toc
tic
[L,U] =lu(S);
for i = 1:ntry
x = U\(L\f);
end
toc