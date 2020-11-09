file=dir('/Users/apple/Downloads/pcn-master/demo_data');
addpath('/Users/apple/Downloads/pcn-master/demo_data');
len=size(file,1);
for i =3:len
    new_points_t=load(file(i).name,'.txt');
    scatter3(new_points_t(:,1),new_points_t(:,2),new_points_t(:,3),'.');
end



    