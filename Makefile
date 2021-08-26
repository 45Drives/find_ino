TARGET = dist/find_ino # program name
CC = g++ # or gcc if C
CFLAGS = -std=c++11 -Wall -Wextra -Isrc/incl # -D<macro name>=<value> to define macro
LIBS = # e.g.: -lboost_filesystem to link boost filesystem

CFLAGS += $(EXTRA_CFLAGS) # allow additional cflags passed with `make EXTRA_CFLAGS="-flag1 -flag2"`
LIBS += $(EXTRA_LIBS) # allow additional link flags passed with `make EXTRA_LIBS="-flag1 -flag2"`

SOURCE_FILES := $(shell find src/impl -name *.cpp)
OBJECT_FILES := $(patsubst src/impl/%.cpp, build/%.o, $(SOURCE_FILES))

ifeq ($(PREFIX),)
	PREFIX := /usr/local/bin
endif

default: $(TARGET)
all: default

static: LIBS:=-static $(LIBS)
static: default

$(TARGET): $(OBJECT_FILES)
	mkdir -p $(dir $@)
	$(CC) $(OBJECT_FILES) -Wall $(LIBS) -o $@

build/%.o: src/impl/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: clean-build clean-target

clean-build:
	-rm -rf build

clean-target:
	-rm -f $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)