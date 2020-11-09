clc;
clear ;
close all;
addpath('../DB/use_water_bottle');
path = '../DB/use_water_bottle';
save_dir = '../DB/all';
mkdir(save_dir);
file = dir(path);
len=size(file,1);
s=[ ];
n_s=[ ];
for i=3:1:len
    s(:,i-2)=load(file(i).name,'.txt');
end
s=sum(s,2)/(len-2);
FILE  = fopen([save_dir '/water_bottle_saliency_blend.txt' ],'a');
fprintf(FILE,'%f\n',s);
fclose(FILE);
