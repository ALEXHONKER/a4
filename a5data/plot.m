xm1=[];
xm2=[];
xm3=[];
xs1=[];
xs2=[];
xs3=[];
ym1=[];
ym2=[];
ym3=[];
ys1=[];
ys2=[];
ys3=[];
for i = 4:2:16
  x=load(["res",num2str(i)]);
  x1=[];
  x2=[];
  x3=[];
  y1=[];
  y2=[];
  y3=[];
  for j=0:30:270
	sd1=x(j+1,2);
	for k=1:3:28
	x1=[x1,x(j+k,1)];
	x2=[x2,x(j+k+1,1)];
	x3=[x3,x(j+k+2,1)];
	y1=[y1,x(j+k,2)/sd1];
	y2=[y2,x(j+k+1,2)/sd1];
	y3=[y3,x(j+k+2,2)/sd1];
	end
 end
xm1=[xm1,mean(x1)];
xm2=[xm2,mean(x2)];
xm3=[xm3,mean(x3)];
xs1=[xs1,std(x1,1)];
xs2=[xs2,std(x2,1)];
xs3=[xs3,std(x3,1)];
ym1=[ym1,mean(y1)];
ym2=[ym2,mean(y2)];
ym3=[ym3,mean(y3)];
ys1=[ys1,std(y1,1)];
ys2=[ys2,std(y2,1)];
ys3=[ys3,std(y3,1)];
end
