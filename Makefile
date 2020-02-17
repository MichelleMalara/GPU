
# Specify the compiler
CC=gcc

# Compilation options
OMP_FLAGS= -lOpenCL -lm -lSDL2

# Directory with common headers
COMMON_DIR=./

# Compile all executable
all: maillageGPU.o mainTemps.o

maillageGPU.o: maillageGPU.c hedge.c maillage.c matriceTriangle.c listIndiceList.c listIndice.c listPoint.c point.c
	$(CC) $^ $(CCFLAGS) $(OMP_FLAGS) $(LIBS) -I $(COMMON_DIR) -o $@

mainTemps.o: mainTemps.c hedge.c maillage.c matriceTriangle.c listIndiceList.c listIndice.c listPoint.c point.c
	$(CC) $^ $(CCFLAGS) $(OMP_FLAGS) $(LIBS) -I $(COMMON_DIR) -o $@

#pi: pi.c
#	$(CC) $^ $(CCFLAGS)  $(OMP_FLAGS) $(LIBS) -I $(COMMON_DIR) -o $@
#
#mandel: mandel.c
#	$(CC) $^ $(CCFLAGS)  $(OMP_FLAGS) $(LIBS) -I $(COMMON_DIR) -o $@

# Remove non sources files
clean:
	rm -f hello pi mandel
	rm -f *.o *~
