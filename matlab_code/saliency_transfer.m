clear;clc;close all;
vis = 0;
addpath('./sample_mesh-master');
source_file = '../mydatas/use50_mug_z.off';
target_file = '../mydatas/target_file';
source_f_file = '../mydatas/use50_mug_z._out.h5';
%target_f_file = '../mydatas/1ae1ba5dfb2a085247df6165146d5bbd_z._out.h5';
source_file_saliency = '../mydatas/use50_mug_saliency_blend.txt';


%% load source mesh and saliency map
s = load(source_file_saliency);
[vertices,faces,normals]= read_off(source_file);
vertices = normalize_mesh(vertices);
%     %% load source mesh and saliency map
%     s = load(source_file_saliency);
%     [vertices,faces,normals]= read_off(source_file);
%     vertices = normalize_mesh(vertices);

%% source mesh data to face centers
[face_centers, normals,face_ids] = mesh_to_point_cloud(vertices, faces);
% [new_points, new_normals, points_ids] = add_points_to_mesh(vertices, faces, face_centers, normals,100000);
s_f = sum(s(faces),2)/3;
% new_saliency = s_f(points_ids);
new_saliency = s_f;
%% source features .h5
source_f = h5read(source_f_file,'/out_features');
source_f = source_f';
source_points =  h5read(source_f_file,'/sample_points');
source_points = source_points';
source_points = normalize_mesh(source_points);

%% find neighbor points ,instead, we could use k-nn methods?? (radius small, may empty cell occur??)
points_saliency = [];
[in_s_indices,in_s_dists] = rangesearch(face_centers,source_points,0.1,'Distance','chebychev');
for i =1:length(in_s_indices)
    %points_saliency(i) = mean(new_saliency(in_s_indices{i}));
    points_saliency(i) = sum(new_saliency(in_s_indices{i}).*(1./in_s_dists{i}'))/sum(1./in_s_dists{i});

% use dist weight mean??
end

files_names = dir([target_file,'/','*.','off']);
l = length(files_names);

for i=1
    %% target features .h5
    target_f_file = [target_file,'/',files_names(i).name(1:end-4),'._out.h5'];
    target_f = h5read(target_f_file,'/out_features');
    target_f = target_f';
    target_points =  h5read(target_f_file,'/sample_points');
    target_points = target_points';
    target_points = normalize_mesh(target_points);

    %% load target mesh data and obtain face centers
    [vertices_t,faces_t,normals_t]= read_off([target_file,'/',files_names(i).name]);
    vertices_t = normalize_mesh(vertices_t);
    [face_centers_t, normals_t,face_ids_t] = mesh_to_point_cloud(vertices_t, faces_t);
    %[new_points_t, new_normals_t, points_ids_t] = add_points_to_mesh(vertices_t, faces_t, face_centers_t, normals_t,20000);
    [new_points_t, new_normals_t, curv,points_ids_t] = sample_mesh(vertices_t, faces_t, 100000,0,{'uniform'});
    l = size(new_points_t,1);
    % new_saliency = zeros(l);

    %% find k neighbor features or ¾àÀë
    target_saliency = [];
    for j =1:size(target_f,1)
        [Idx,D] = knnsearch(source_f,target_f(j,:),'k',4,'Distance','chebychev');
        target_saliency(j) = mean(points_saliency(Idx)); 
        %target_saliency(i) = sum(points_saliency(Idx).*(D))/sum(D);
     end

    %% find neighbor points ,instead, we could use k-nn methods?? (radius small, may empty cell occur??) k½üÁÚ
    new_points_t_saliency = [];

    [in_s_indices_t,in_s_dists_t] = rangesearch(target_points,new_points_t,0.2,'Distance','chebychev');
    for k =1:length(in_s_indices_t)
        %new_points_t_saliency(k) = mean(target_saliency(in_s_indices_t{k})); % use dist weight mean??
        new_points_t_saliency(k)  =  sum(target_saliency(in_s_indices_t{k}).*(1./(in_s_dists_t{k})))/sum(1./(in_s_dists_t{k}));
        
    
    end

    %  for m =1:length(new_points_t)
    %      [n_Idx,n_D] = knnsearch(target_points,new_points_t(m,:),'k',4,'Distance','chebychev');
    % 
    %      new_points_t_saliency(m) = mean(target_saliency(n_Idx)); 
    %      %new_points_t_saliency(m) = sum(target_saliency(n_Idx).*(n_D))/sum(n_D);
    %  end

    %%
    if vis==1
        h = figure();
        subplot(1,4,1);
        cc = new_saliency;
        s = ones(size(cc));
        scatter3(face_centers(:,1),face_centers(:,2),face_centers(:,3),s,cc);
        axis off;
        axis equal;
%         subplot(1,4,2);
%         cc = points_saliency;
%         s = ones(size(cc));
%         scatter3(source_points(:,1),source_points(:,2),source_points(:,3),s,cc);
%         axis off;
%         axis equal;
%         subplot(1,4,3);
%         cc = target_saliency;
%         s = ones(size(cc));
%         scatter3(target_points(:,1),target_points(:,2),target_points(:,3),s,cc);
%         axis off;
%         axis equal;
        subplot(1,4,4);
        cc = new_points_t_saliency;
        s = ones(size(cc));
        scatter3(new_points_t(:,1),new_points_t(:,2),new_points_t(:,3),s,cc);
        axis off;
        axis equal;
    end
end







