
To compile in Unix:
run the Makefile

To compile in Windows:
setup your C compiler in Matlab using 'mex -setup'
run:
mex -v -I. shortest_path_point.c dijkstra.c pq.c
mex -v -I. shortest_path_points.c dijkstra.c pq.c
mex -v -I. shortest_path_points.c dijkstra.c pq.c
