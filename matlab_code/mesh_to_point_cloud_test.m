
% %  Copyright (c) 2013
% %      Oliver van Kaick <ovankaic@gmail.com>
% 
% function [ points, normals,face_to_points_ids ] = mesh_to_point_cloud_test( vertices, faces )
% 
%     F = size(faces,1);
%     face_to_points_ids = zeros(F,1);
%     points = zeros(F,3);
%     normals = zeros(F,3);
% 
%     for i = 1:F
%        points(i,:) = sum(faces.v(i,:),:) / 3;
% 
%        vector1 = vertices(faces(i,2),:) - vertices(faces(i,1),:);
%        vector2 = vertices(faces(i,3),:) - vertices(faces(i,2),:);
% 
%        normals(i,:) = cross(vector1, vector2);
%        normals(i,:) = normals(i,:) / norm(normals(i,:));
%        face_to_points_ids(i) = i;
%     end
% 
% end
% 
