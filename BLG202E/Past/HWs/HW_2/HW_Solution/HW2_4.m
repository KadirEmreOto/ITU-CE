r1 = 3;
r2 = 10;
r3 = 20;
r4 = 100;
colormap('gray')
imageMatrix = imread('sample.png');
figure(1)
image(imageMatrix);
imageMatrix = double(imageMatrix);
[U,S,V] = svd(imageMatrix);
figure(2)
colormap('gray')
image(U(:,1:r1)*S(1:r1,1:r1)*V(:,1:r1)')
figure(3)
colormap('gray')
image(U(:,1:r2)*S(1:r2,1:r2)*V(:,1:r2)')
figure(4)
colormap('gray')
image(U(:,1:r3)*S(1:r3,1:r3)*V(:,1:r3)')
figure(5)
colormap('gray')
image(U(:,1:r4)*S(1:r4,1:r4)*V(:,1:r4)')
