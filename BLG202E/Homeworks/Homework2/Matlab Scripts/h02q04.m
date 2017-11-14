function h02q04()
    X = imread('cute.jpg');
    G = rgb2gray(X);
    D = im2double(G);
    
    [U,S,V] = svd(D);
    
    for r = 10 : 5 : 30
       reduced = U(:,1:r)*S(1:r,1:r)*V(:,1:r)';
       figure, imshow(reduced);
    end
end