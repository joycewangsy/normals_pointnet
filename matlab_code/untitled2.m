%new_points=load('/Users/apple/Downloads/teapot/teapot8_z.xyz');
[new_points,faces,normals,uv]= read_obj('/Users/apple/saliency_map/DB/all/water_bottle.obj');
cc = load('/Users/apple/saliency_map/DB/all/water_bottle_saliency_blend.txt');
s = ones(size(cc));
scatter3(new_points(:,1),new_points(:,2),new_points(:,3),s,cc);
axis off;
axis equal;