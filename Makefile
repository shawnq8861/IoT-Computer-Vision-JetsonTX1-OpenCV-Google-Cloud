CC = g++
CC_FLAGS = -Wall
LD_FLAGS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lueye_api
INPUT_FILES = acquireModifySaveImage.cpp
OUTPUT_FILE = acquireModifySaveImage

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(INPUT_FILES)
	$(CC) $(CC_FLAGS) $(INPUT_FILES) $(LD_FLAGS) -o $(OUTPUT_FILE)

clean:
	rm $(OUTPUT_FILE)
