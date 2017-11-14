%%Elif AK 150130009
function y1y2 =y1y2(t,y)
 y1y2(1) = .25*y(1) - .01*y(1)*y(2);
 y1y2(2) = -y(2) +.01*y(1)*y(2);
 y1y2 = [y1y2(1) y1y2(2)]'; 
end