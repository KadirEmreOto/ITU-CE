function  h02q02()
    % (part a): calling mylu function implemented for lu decomposition
    A = [1 2 4; 3 8 14; 2 6 13];
    b1 = [3; 13; 4];
    b2 = [6; 24; 15];
    b3 = [-1; -5; -4];
    
    % (part b): calculating x vectors with given b vectors
    [l, u] = mylu(A);        % LU decomposition (calculated for ones)
    
    y1 = forwardsub(l, b1); % implemented in forwardsub.m
    x1 = backwardsub(u, y1) % implemented in backwardsub.m
    
    y2 = forwardsub(l, b2);
    x2 = backwardsub(u, y2)
    
    y3 = forwardsub(l, b3);
    x3 = backwardsub(u, y3)
end