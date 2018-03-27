COMPILER = gcc
COMPILER_FLAGS = --std=c99 -w -Wall -lm
LINKER_FLAGS = -lSDL2 -lSDL2_image
APPLE_LFLAGS = -framework SDL2 -framework SDL2_image
TARGET = shards

all: $(TARGET).c
	# $(COMPILER) $(TARGET).c $(COMPILER_FLAGS) $(APPLE_LFLAGS) -o $(TARGET)

# linux: $(TARGET).c
	# $(COMPILER) $(TARGET).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)

# apple: $(TARGET).c
	$(COMPILER) ./$(TARGET).c $(COMPILER_FLAGS) $(APPLE_LFLAGS) -o $(TARGET)
