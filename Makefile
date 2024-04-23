CC := g++
CFLAGS := -Wall -I./include

SRCDIR := src
INCDIR := include
BUILDDIR := bin
TARGET := main

# List the source files
SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))

# Main target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

# Compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the project
clean:
	@rm -rf $(BUILDDIR) $(TARGET)

.PHONY: clean