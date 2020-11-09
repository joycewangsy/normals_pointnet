
%  Copyright (c) 2013
%      Oliver van Kaick <ovankaic@gmail.com>

% 
% Compute the graph distance and path from one point to all other points
% in a point cloud
%
% [D, P] = point_shortest_path(V, N, source)
%
% Input:
%   - V: input point cloud
%   - N: point neighborhood relations
%   - source: source point in the range [1..number of points]
%
% Output:
%   - D: distances. D(i) is the distance from the source to point i
%   - P: path. P(i) is the predecessor of face i. To get the path from
%   source to point i, use the function mesh_get_path()
%
% The distance is approximated by the graph distance between points.
% The path and distance are computed with Dijkstra's algorithm.
%
% See also mesh_get_path
%
function [D, P] = point_shortest_paths(V, N, sources)

    [D, P] = shortest_path_points(V, N, sources);
end
