load('A.mat');
A;
AtA = A'*A;
total = sum(AtA(:));
average = total/4;
centeredAtA = AtA - average
M = [1;0]; %%Initial matrix
index = 1;
while index < 10 %%Iterate 10 times
iterationM = centeredAtA * M; %%A*v = v_next
n = norm(iterationM);
iterationM = iterationM / n; %%norm of iteration eigenvector
M = iterationM;
index = index + 1;
end
 iterationM
K = eig(centeredAtA); %%All eigenvalues
eigenValueWithSVD = K(2,1) %%dominant eigenvalue found in SVD
B = centeredAtA*iterationM; %%AX = lamda*X
x = B(1,1);
y = iterationM(1,1); 
eigenValueWithPIM = x / y %% dominant eigenvalue found in PIM
%% Both are equal

