# Set compiler according to hostname
MEXCC=/usr/local/MATLAB/R2012b/bin/mex
MATLABINC=/usr/local/MATLAB/R2012b/extern/include/ -fPIC

# Compilation targets
TARGETS=shortest_path_point.mex shortest_path_points.mex shortest_path_st.mex 

# Make rules
.PHONY: all clean

CFLAGS=-I./ -I${MATLABINC}

#%.o : %.c
#	$(CC) $(CFLAGS) -I./ -c $< -o $@ 

shortest_path_st.mex : %.c pq.o dijkstra_st.o
	$(MEXCC) $< pq.o dijkstra_st.o

%.mex : %.c pq.o dijkstra.o
	$(MEXCC) $< pq.o dijkstra.o

all: $(TARGETS)

clean:
	rm -f *.mexglx *.mexa64 *.o
