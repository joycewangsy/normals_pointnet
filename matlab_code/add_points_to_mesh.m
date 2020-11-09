
%  Copyright (c) 2013
%      Noa Fish <noafish@post.tau.ac.il>

% adds points to mesh to make it more dense.
% larger faces have a higher probability of being chosen.

function [points, normals,ids] = add_points_to_mesh(vertices, faces, points, normals, num)

areas = compute_area(vertices, faces);

cum_areas = cumsum(areas);
cum_areas = cum_areas - cum_areas(1);
cum_areas = cum_areas / cum_areas(end);

n = size(faces,1);
add = num-n;
ids = [1:1:n];
ids = ids';
for i=1:add
    r = rand(2,1);
    
    index1 = binarysearch(cum_areas,r(1));
    vert1 = faces(index1,:);
    a1 = vertices(vert1(1),:);
    b1 = vertices(vert1(2),:);
    c1 = vertices(vert1(3),:);
    r2 = rand(2,1);
    p1 = (1-sqrt(r2(1)))*a1 + sqrt(r2(1))*(1-r2(2))*b1 + sqrt(r2(1))*r2(2)*c1;

    points = [points; p1];
    normals = [normals; normals(index1,:)];
    ids = [ids;index1];
end



end