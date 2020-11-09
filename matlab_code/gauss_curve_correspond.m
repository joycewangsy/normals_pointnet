clc
clear n_vertices n_faces n_normals n_uv n_guass;
addpath('./jjcao_code-head/toolbox/jjcao_mesh');
addpath('./jjcao_code-head/toolbox/jjcao_mesh/feature');
addpath('./jjcao_code-head/toolbox/jjcao_mesh/smoothing');

file_dir = '../DB/all';
file_name = [file_dir,'/mug.obj'];
saliency_map_file=[file_name(1:end-4),'_saliency_blend.txt'];
newobj_dir = '../1/mug';
save_dir='../mug2';
[vertices,faces,normals,uv]=read_obj(file_name);
vertices=normalize_mesh(vertices);
o_guass=compute_gaussian_curvature(vertices,faces);
s = load(saliency_map_file);
new_files_names = dir([newobj_dir,'/','*.','obj']);
n_l = length(new_files_names);

% for i =1:l
%     new_mesh_file = [newobj_dir,'/',new_files_names(i).name];
%     [n_vertices,n_faces,n_normals,n_uv]=read_obj(new_mesh_file);
%     n_guass=compute_gaussian_curvature(n_vertices,n_faces);
%     
%     
% end
[n_vertices,n_faces,n_normals,n_uv]=read_obj('../1/mug/mug_c_5.obj');
for j=1:3
    n_normals(:,j)=n_vertices(:,j+3);
end
n_vertices(:,4)=[];n_vertices(:,4)=[];n_vertices(:,4)=[];
n_vertices = normalize_mesh(n_vertices);

format long g
n_guass=compute_gaussian_curvature(n_vertices,n_faces);
n_guass=perform_mesh_smoothing(n_faces,n_vertices,n_guass)+6;
u=1;
n_sailency=zeros(length(n_guass),1);

    for k=1:length(n_guass)
        C=abs(n_guass(k)-o_guass(1));
        for l=1:length(o_guass)-1
            if abs(n_guass(k)-o_guass(l+1))<=C
                        C=abs(n_guass(k)-o_guass(l+1));
                        u=l+1;
            end

        end
        i_c(k,:)=u;
        %o_guass(u)=10000;
        n_sailency(k)=s(u);
    end

    
mkdir(save_dir);
%name=strcat(files_names(i).name(1:end-4),'.txt');
name=strcat('mug_c_5','_saliency_blend.txt');
new_sailency  = fopen([save_dir '/' name],'a');
fprintf(new_sailency,'%f\n',n_sailency);
fclose(new_sailency);







    
            
            
            
            
            


