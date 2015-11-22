xm1=[];
xm2=[];
xs1=[];
xs2=[];
ym1=[];
ym2=[];
ys1=[];
ys2=[];
for i = 5:5:50
  x=load(["res",num2str(i)]);
  x1=[];
  x2=[];
  y1=[];
  y2=[];
  for j=0:20:180
	for k=1:2:19
	x1=[x1,x(j+k,1)];
	x2=[x2,x(j+k+1,1)];
	end
 end
xm1=[xm1,mean(x1)];
xm2=[xm2,mean(x2)];
xs1=[xs1,std(x1,1)];
xs2=[xs2,std(x2,1)];
end
time=5:5:50;
errorbar(time,xm1,xs1,'g');
hold on;
errorbar(time,xm2,xs2,'b');
xlabel("Number of Vertices");
ylabel("Running time (us)");
