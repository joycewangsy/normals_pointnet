/* Matrix element access function (using C indexing: 0..(m-1), 0..(n-1)) */
/* num_rows is the size of one column (number of rows in one column) */
#define el_indx(row, column, num_rows)\
    (((column) * (num_rows)) + (row))


void point_coord(const mxArray *vertices, int vertex, double center[3]);
void face_center(const mxArray *vertices, const mxArray *faces, int face, double center[3]);
double edge_length(double center1[3], double center2[3]);

void dijkstra(const mxArray *vertices, const mxArray *mNeigh, double source, double *dist, double *path);

void dijkstra_euclidean(const mxArray *vertices, const mxArray *faces, const mxArray *mNeigh, double source, double dest, double *dist, double *path);
