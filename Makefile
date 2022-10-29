# Compiler (gcc, clang llvm etc)
CC := gcc 

# The name of the build folder 
BUILD := build
# The name of the executable
TARGET := main

# Compiler and linker flags
CPPFLAGS =
CFLAGS = -std=c99 -Wall -Wextra -O0 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

# If source files are inside subdirectories, add the subdirectories here
INDIRS := $(addprefix -I, \
)

# Find all filenames with the .c extension
CFILES := $(shell find . -name "*.c")
# From the .c filenames convert their names to .o (object files) filenames and in the subfolder of the build
OFILES := $(CFILES:./%.c=$(BUILD)/%.o)
# From the .o filnames convert their names to .d (dependency files) filenames
DFILES := $(OFILES:%.o=%.d)

# Find all subdirectories
SRCDIRS := $(shell find . -type d -not \( -path '*build*' -or -path '*git*' -or -path '*vscode*' -or -path '*scripts*' \) )
# Create the Build folder and all subdirectories
$(shell mkdir -p $(SRCDIRS:%=$(BUILD)/%))

# Main target
all: $(TARGET)

# Linking
$(TARGET) : $(OFILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) 
# Compiling
$(BUILD)/%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

# Cleaning executable and object files
clean :
	rm -rf $(BUILD) $(TARGET)

cleantest : 
	rm output/results/*
	rm output/steps/*
	rm output/tiles/*
	rm output/output.jpg
	rm output/*

# To tell make that all and clean are not files rules
.PHONY: all clean

# Include dependency files
-include $(DFILES)

#END
