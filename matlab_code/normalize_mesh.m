function  [new_vertices]= normalize_mesh(vertices)
c = mean(vertices);
shifted = vertices-c;
max_norm = max(sqrt(sum(shifted.^2,2)));
new_vertices = shifted/max_norm;
end