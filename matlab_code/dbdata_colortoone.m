clc;
clear color;
close all;
vis = 0;
root_dir = '../contactdb_contactmaps/use';
%file_dir = strcat('/full', j ,'_use');
file_dir = '/full34_use';
save_dir = '../DB/use/full34use';
new_dir = save_dir;
%new_dir_cmd= ['mkdir' new_dir];
mkdir(new_dir);
files_names = dir([root_dir,file_dir,'/','*.','ply']);
l = length(files_names);
for i =1:l
mesh_file = [root_dir,file_dir,'/',files_names(i).name];
p=pcread(mesh_file);
vertices = p.Location;
normals=p.Normal;
p=pcread(mesh_file);
color=(double(p.Color(:,1))/255);
name=strcat(files_names(i).name(1:end-4),'.txt');
FILE  = fopen([new_dir '/' name],'a');
fprintf(FILE,'%f\n',color);
fclose(FILE);
if vis==1
    h = figure();
    s_results=load([new_dir '/' name]);
    s=ones(size(s_results));
    scatter3(vertices(:,1),vertices(:,2),vertices(:,3),s,s_results,'filled');
%     subplot(1,3,1);
%     s = ones(size(cc));
%     scatter3(new_points(:,1),new_points(:,2),new_points(:,3),s,cc);
%     axis off;
%     axis equal;
%     subplot(1,3,2);
 
end
end


