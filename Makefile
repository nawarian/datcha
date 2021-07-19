# Command utils
RMDIR=rm -rf
MKDIR=mkdir -p

# Flags
LIBS=lua raylib
CFLAGS=`pkg-config --cflags $(LIBS)` -W -Wall
LDFLAGS=`pkg-config --libs $(LIBS)`

# Compiler
CC=clang

# Build
SRCDIR=src
BUILDDIR=build
OUTPUT=datcha

OBJS=$(patsubst $(SRCDIR)%.c, $(BUILDDIR)%.o, $(wildcard $(SRCDIR)/*.c))

all: $(BUILDDIR) $(BUILDDIR)/$(OUTPUT)

clean:
	$(RMDIR) $(BUILDDIR)
.PHONY: clean

$(BUILDDIR)/$(OUTPUT): $(OBJS)
	$(CC) $< -o $(BUILDDIR)/$(OUTPUT) $(LDFLAGS)

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run, tests and debug
run: $(BUILDDIR) $(BUILDDIR)/$(OUTPUT)
	$(BUILDDIR)/$(OUTPUT)
.PHONY: run

