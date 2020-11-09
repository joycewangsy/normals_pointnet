clc;clear;close all;
addpath('./jjcao_common/');
addpath('./geod');
addpath('./sample_mesh-master');
vis = 1;
root_dir = '../DB';
file_dir = '/all';
save_dir = '../saliency_50use_models';
addpath('./npy-matlab-master/npy-matlab');  
% file_dir = '../mug2';
% save_dir = '../saliency_new_models_from_mug'
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
saliency_map_file = [root_dir,file_dir,'/',files_names(i).name(1:end-4),'_saliency_blend.txt'];
s = load(saliency_map_file);
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

[new_points, new_normals, points_ids] = add_points_to_mesh(vertices, faces, face_centers, normals,100000);
%[new_points, new_normals, curv,points_ids] = sample_mesh(vertices, faces, 100000,0,{'uniform'});
s_f = sum(s(faces),2)/3;
new_saliency = s_f(points_ids);

if vis==1
    h = figure();
    subplot(1,3,1);
    cc = s_f(points_ids);
    s = ones(size(cc));
    scatter3(new_points(:,1),new_points(:,2),new_points(:,3),s,cc);
    axis off;
    axis equal;
    subplot(1,3,2);
    options.face_vertex_color=s;
    plot_mesh(vertices,faces,options);
    subplot(1,3,3);
    options = struct;
    options.face_color = [254 204 102]/255;
    options.edge_color = [254 204 102]/255;
    plot_mesh(vertices,faces,options);
    axis off;
    axis equal;
end

save_name = files_names(i).name(1:end-4);
save_point_name = [save_dir,'/',save_name,'.xyz.npy'];
save_normal_name =  [save_dir,'/',save_name,'.normals.npy'];
save_saliency_name = [save_dir,'/',save_name,'.saliency.npy'];
fprintf(FILE,'%s\n',save_name);
writeNPY(new_points, save_point_name);
mat2txt([save_dir,'/',save_name,'.`4'],new_points);
writeNPY(new_normals, save_normal_name);
mat2txt([save_dir,'/',save_name,'.normals'],new_normals);
writeNPY(new_saliency, save_saliency_name);
mat2txt([save_dir,'/',save_name,'.saliency'],new_saliency);


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
