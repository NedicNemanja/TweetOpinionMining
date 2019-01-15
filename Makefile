INCLUDE_DIR = ./include
SOURCE_DIR = ./source
OBJECT_DIR = ./object

CC = g++
CFLAGS=-I$(INCLUDE_DIR) -g -fmax-errors=1 -lm
OUT = recommendation

_DEPS = Cluster.hpp ClusterSpace.hpp CosineSimilarity.hpp ErrorCodes.hpp HashTable.hpp HypercubeEuclidean.hpp LSH.hpp LSHEuclidean.hpp Metric.hpp myvector.hpp ParsingCSV.hpp ReadInput.hpp Tweet.hpp User.hpp utility.hpp WriteOutput.hpp
#pattern matching from  _DEPS to include directory
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = main.o Cluster.o ClusterSpace.o CosineSimilarity.o HashTable.o HypercubeEuclidean.o LSH.o LSHEuclidean.o Metric.o myvector.o ParsingCSV.o ReadInput.o Tweet.o User.o utility.o WriteOutput.o
#same pattern matching principe
OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJ))

.PHONY: clean

############goals#######################

#general rule for all object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#default goal
$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

count:
	wc $(SOURCE_DIR)/*.cpp $(DEPS)

clean:
	rm -f $(OBJECT_DIR)/*.o ./recommendation
