INCLUDE_DIR = ./include
SOURCE_DIR = ./source
OBJECT_DIR = ./object

CC = g++
CFLAGS=-I$(INCLUDE_DIR) -O3 -fmax-errors=1
OUT = recommendation

_DEPS = ErrorCodes.hpp myvector.hpp ParsingCSV.hpp ReadInput.hpp Tweet.hpp utility.hpp
#pattern matching from  _DEPS to include directory
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = main.o myvector.o ParsingCSV.o ReadInput.o Tweet.o utility.o
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
