clc;clear;close all;
addpath('./jjcao_common/');
addpath('./geod');
addpath('./sample_mesh-master');
vis = 1;
root_dir = '../dataset';
file_dir = '/mesh_files';
save_dir = '../dataset';
addpath('./npy-matlab-master/npy-matlab');  

%% make model_dir and .txt files
new_dir = save_dir;
mkdir(new_dir);
% system(new_dir_cmd);

if ~exist([new_dir '/' 'train_files.txt'])
    FILE  = fopen([new_dir '/' 'train_files.txt'],'w');
else
    FILE  = fopen([new_dir '/' 'train_files.txt'],'a');
end

%%
files_names = dir([root_dir,file_dir,'/','*.','obj']);
l = length(files_names);

for i =1:l
mesh_file = [root_dir,file_dir,'/',files_names(i).name];
[vertices,faces,normals,uv]= read_obj(mesh_file);
%normals = vertices(:,4:6);
%vertices = vertices(:,1:3);

%% normalize mesh
% c = mean(vertices);
% shifted = vertices-c;
% max_norm = max(sqrt(sum(shifted.^2,2)));
% vertices = shifted/max_norm;
vertices = normalize_mesh(vertices);

%% mesh data to face centers
[face_centers, normals,face_ids] = mesh_to_point_cloud(vertices, faces);

%[new_points, new_normals, points_ids] = add_points_to_mesh(vertices, faces, face_centers, normals,2048);
[new_points, new_normals, curv,points_ids] = sample_mesh(vertices, faces, 2048,0,{'uniform'});
step = 0.1;
%outside points
query_outside_point_sets = new_points+step*new_normals;
out_direct=zeros(2048,1);

%inside points
query_inside_point_sets=new_points-step*new_normals;
in_direct=ones(2048,1);

query_point_sets=[query_outside_point_sets;query_inside_point_sets];
directions=[out_direct;in_direct];

save_name = files_names(i).name(1:end-4);
save_point_name = [save_dir,'/','point_sets','/',save_name,'.xyz.npy'];
save_normal_name =  [save_dir,'/','normals','/',save_name,'.normals.npy'];
save_query_point_sets_name = [save_dir,'/','query_point_sets','/',save_name,'.xyz.npy'];
save_query_point_direction_name = [save_dir,'/','query_point_direction','/',save_name,'.direct.npy'];

if vis==1
    h = figure();
    %subplot(1,3,1);
    %cc = s_f(points_ids);
    %s = ones(size(cc));
    scatter3(new_points(:,1),new_points(:,2),new_points(:,3),'.');
    axis equal;
    %axis off;
    %axis equal;
    %subplot(1,3,2);
    %options.face_vertex_color=s;
    %plot_mesh(vertices,faces,options);
    %subplot(1,3,3);
    %options = struct;
    %options.face_color = [254 204 102]/255;
    %options.edge_color = [254 204 102]/255;
    %plot_mesh(vertices,faces,options);
    %axis off;
    %axis equal;
end


fprintf(FILE,'%s\n',save_name);
writeNPY(new_points, save_point_name);
mat2txt([save_dir,'/','point_sets','/',save_name,'.xyz'],new_points);

writeNPY(new_normals, save_normal_name);
mat2txt([save_dir,'/','normals','/',save_name,'.normals'],new_normals);

writeNPY(query_point_sets, save_query_point_sets_name);
mat2txt([save_dir,'/','query_point_sets','/',save_name,'.xyz'],query_point_sets);

writeNPY(directions, save_query_point_direction_name);
mat2txt([save_dir,'/','query_point_direction','/',save_name,'.direct'],directions);



end
%%
 
% %% get k-nn patches on the mesh use the 10-nn points to build a sparse graph
% [gr, point_adj, max_elen, outliers] = build_graph(new_points, 10);
% V = new_points;
% N = gr;
% %% samples centers generation
% n_samples = 1000;
% selected = farthest_points(new_points, n_samples, []);
% ind_select = find(selected==1);
% 
% %% patch generation
% patch_radius=0.1;
% for j=1:length(ind_select)
%     disp(j);
%     [D, P] =  point_shortest_path(V, N, ind_select(j));  % the distances on the graph use Dijkstra's algorithm
%     near_ind = find(D<patch_radius);
%     near_points = new_points(near_ind,:);
%     scatter3(near_points(:,1),near_points(:,2),near_points(:,3),'.');
%     hold on;  
% end
