%%Elif AK 150130009
clear;
to = 0; %%integrated from 0
tf = 100; %%integrated to 100
yo = [80 30]; %%initial value
options = odeset('RelTol', 1e-5);
[t y] = ode45('y1y2',[to tf],yo,options); 
plot(y(:,1),y(:,2))
ylabel('prey')
xlabel('predator')
