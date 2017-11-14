% lets create a vector
A=[2, 4 , 5 ,9 ,4 ,7, 9]
l=length(A)%length for vector
[s1 s2]=size(A); % size for any array

% now let create another vector A2 such that contains last 3 elements of A
A2=A(l-2:l);
%or
A2=A(end-2:end);
%now change 3rd element of A with 8
A(3)=8;
%access the elements of vector A
bb = A(5);
cc = A(2);
%now sort the A vector
A=sort(A);
%initially A is a row vector let change A to a column vector with transpose
Ac=A';
%% Create matrices
B1=zeros(3,4);
%
B2=ones(3,4);
%
B3=5*eye(3,3);
%
B=diag(A);
diag(B)
%
C=[1 2 3 7;6 2 9 1;2 5 1 8];
D = [1 3 5 7; 2 4 6 8; 1 4 7 10];

%Access the elements of matrices with two indices
cc = C(2,3)
dd = D(2,4)
%Access rows of matrices
ee = C(3, :)
ff = D(2, :)
%Access columns of matrices
gg = C(:, 1);
hh = D(:, 2);

% now using reshape function change the size of C matrix
C1=reshape(C,[4,3])%% what is the linear indexing in matlab?
%change the 2nd row of C1 with first 3 elements of A vector
C1(2,1:3)=A(1:3)
%change 3 elements the 3rd column of D with first 3 elements of A vector
D(1:3, 3) = A(1:3)
%make D a column vector
E = C(:);



%% some functions
s1=sum(C1); %% sum column elements
s3=sum(C1,2);% sum row element
s2=sum(sum(C1));
m1=mean(C1);  %find the averages
m1_2=mean(C1,2);  %find the averages
m2=mean(mean(C1));

%%indexing
% find the elements larger than mean
index = (C1>m2); % gives location of elements with ones (logical)
C2 = C1; 
C2(index) = 20;  %set those elements to 20 in matrix C2
% find the elements smaller than or equal to mean
index2 = (C1<=m2); % gives location of elements with ones (logical) 
C2(index2) = 10;  %set those elements to 10 in matrix C2

K=double(C1>m2); %convert logical to double
Numb=sum(sum(K));  %gives the number of elements

%% decrease the entries that are larger than m2 with 2
Cd=C1-K*2;

%if statements
if(D(2,3) == 4)
	D(2,3) = 5;
elseif(D(2,3) == 3)
	D(2,3) = 4; 
else
	D(2,3) = 3;
end

if(D(3,3) < 4)
	D(3,3) = 5;
else
    D(3,3) = 2; 
end

%for statement
V=[2, 4 , 5 ,6 ,9 ,7, 11];
lv = length(V);
%square every element of vector V
for i=1:lv
	V(i) = V(i)^2; 
end

