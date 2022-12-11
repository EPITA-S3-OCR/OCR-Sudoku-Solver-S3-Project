# Compiler (gcc, clang llvm etc)
CC := gcc 

# The name of the build folder 
BUILD := build
# The name of the executable
TARGET := main

# Compiler and linker flags
CPPFLAGS =
CFLAGS = -std=c99 -Wall -Wextra -O0 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0 MagickWand`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image gtk+-3.0 MagickWand` -lm

# If source files are inside subdirectories, add the subdirectories here
INDIRS := $(addprefix -I, \
)

# Find all filenames with the .c extension
CFILES := $(shell find ./src -name "*.c")
# From the .c filenames convert their names to .o (object files) filenames and in the subfolder of the build
OFILES := $(CFILES:./%.c=$(BUILD)/%.o)
# From the .o filnames convert their names to .d (dependency files) filenames
DFILES := $(OFILES:%.o=%.d)

# Find all subdirectories
SRCDIRS := $(shell find ./src -type d -not \( -path '*build*' -or -path '*git*' -or -path '*vscode*' -or -path '*scripts*' \) )
# Create the Build folder and all subdirectories
$(shell mkdir -p $(SRCDIRS:%=$(BUILD)/%))

# Main target
all: $(TARGET)

# Linking
$(TARGET) : $(OFILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) 
# Compiling
$(BUILD)/%.o : %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

doc :
	doxygen doxygen/Doxyfile

clean-doc : 
	rm -rf doc 

# Cleaning executable and object files
clean :
	rm -rf $(BUILD) $(TARGET)

clean-output : 
	rm -rf output/results/*
	rm -rf output/steps/*
	rm -rf output/tiles/*
	rm -rf output/train/*
	rm -rf output/output*

# To tell make that all and clean are not files rules
.PHONY: all clean doc clean-doc clean-output

# Include dependency files
-include $(DFILES)

#END
