SHELL := /bin/bash

# app name
name = app
name_release = $(name).release
name_debug = $(name).debug
name_test = $(name).test
name_profraw = $(name).profraw
name_profdata = $(name).profdata
name_coverage_txt = $(name).coverage.txt
name_coverage_html = $(name).coverage.html
src_path = ./src

# compiler config
CC = clang
CFLAGS = -std=iso9899:1990 -ansi -Weverything -Wall -Wextra -Werror -Wpedantic -I./includes
LDFLAGS = -I./includes -lm

# sources and entry point
src = $(shell find $(src_path) -name *.c ! -name *_test.c ! -name main.c)
src_main = $(shell find $(src_path) -name main.c)
obj = $(src:.c=.o)
obj_main = $(src_main:.c=.o)

# files to clean up
trash = $(obj) $(obj_main)

# default build
all: release

# release target
release: CFLAGS += -O3
release: $(name_release)
$(name_release): $(obj_main) $(obj)
	$(CC) $(LDFLAGS) -o $(name_release) $^

# debug target
debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: $(name_debug)
$(name_debug): $(obj_main) $(obj)
	$(CC) $(LDFLAGS) -o $(name_debug) $^

# make object files from source code
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# remove old build files
clean:
	rm -rf $(trash)

.ONESHELL:
