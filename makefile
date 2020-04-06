#Copyright 2019 Sosthène Guédon
#
#This file is part of peertube-viewer.
#
#   peertube-viewer is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, version 3 of the License
#
#   peertube-viewer is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with peertube-viewer.  If not, see <https://www.gnu.org/licenses/>.

.PHONY: clean run test debug all install test-catch-v1

.ONESHELL:
.DELETE_ON_ERROR:
.RECIPEPREFIX= >
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

SRC_DIR = src
CC=g++
CFLAGS+= -std=c++17 -Wall 
test-catch-v1: CFLAGS+=-DCATCH_VERSION_1


LIBS=-lcurl -lboost_program_options  -lreadline -ltinfo -lboost_regex

ODIR=obj
OBJ_DIR_UI=obj/ui
OBJ_DIR_UTILS=obj/utils
OBJ_DIR_BACKEND=obj/backend
OBJ_DIR_BACKEND_NETWORK=obj/backend/network
OBJ_DIR_TEST=obj/tests
MAKEDEPS_DIR=obj/makedeps
MAKEDEPS_DIR_UI=obj/makedeps/ui
MAKEDEPS_DIR_UTILS=obj/makedeps/utils
MAKEDEPS_DIR_BACKEND=obj/makedeps/backend
MAKEDEPS_DIR_BACKEND_NETWORK=obj/makedeps/backend/network
MAKEDEPS_DIR_TEST=obj/makedeps/tests

DIRS=$(MAKEDEPS_DIR_UI) $(MAKEDEPS_DIR_UTILS) $(MAKEDEPS_DIR_BACKEND) $(MAKEDEPS_DIR_BACKEND_NETWORK) $(MAKEDEPS_DIR_TEST) $(OBJ_DIR_UI) $(OBJ_DIR_UTILS) $(OBJ_DIR_BACKEND) $(OBJ_DIR_BACKEND_NETWORK) $(OBJ_DIR_TEST)

#_SRC_DIRS= $(SRC_DIR) $(OBJ_DIR_UI) $(OBJ_DIR_UTILS) $(OBJ_DIR_BACKEND) $(OBJ_DIR_BACKEND_NETWORK) $(OBJ_DIR_TEST)
#SRC_DIRS:= $(patsubst $(ODIR)/%,$(SRC_DIR)/%,$(_SRC_DIRS))

_OBJ:= $(shell find $(SRC_DIR) -name *.cpp)
OBJ:= $(patsubst $(SRC_DIR)/%.cpp,$(ODIR)/%.o,$(_OBJ))
DEPS:= $(patsubst $(SRC_DIR)/%.cpp,$(MAKEDEPS_DIR)/%.deps,$(_OBJ))

release: CFLAGS+= -O2 -z relro -z now
release: peertube-viewer 

TESTABLE_OBJ:= $(filter-out $(ODIR)/%/main.o,$(OBJ))
MAIN_OBJ:= $(filter-out $(ODIR)/%/tests.o,$(OBJ))

peertube-viewer: $(MAIN_OBJ)
> @echo linking peertube-viewer
> $(CC) -o $@ $(MAIN_OBJ) $(CFLAGS) $(LIBS)

testExec: $(TESTABLE_OBJ)
> @echo linking the test executable
> $(CC) -o $@ $(TESTABLE_OBJ) $(CFLAGS) $(LIBS)

$(DIRS):
> @mkdir -p $(DIRS)


$(MAKEDEPS_DIR)/%.deps: $(SRC_DIR)/%.cpp | $(DIRS)
> @echo generating include deps for $<
> @g++ -MF $@  -MM $(CFLAGS) -I$(SRC_DIR)  $<
> @echo "> \$$(CC) -o \$$@  -c \$$< \$$(CFLAGS) -I\$$(SRC_DIR)"  >> $@ 2>&1
> @sed -i $@ -e 's+^.*\.o+$@+g'
> @sed -i $@ -e 's+$(MAKEDEPS_DIR)+$$(ODIR)+g'
> @sed -i $@ -e 's+\.deps+.o+g'



all: peertube-viewer testExec


 
run: peertube-viewer
> ./peertube-viewer

debug: peertube-viewer
> @echo TEST EXECUTABLE LINKED
> -./peertube-viewer Mastodon 2>debug
> @echo ERRORS:
> @cat debug
> @rm debug

clean:
> rm -rf testExec peertube-viewer $(ODIR)

check:
> cppcheck . 

test: testExec | $(DIRS)
> ./testExec

test-catch-v1: test

install: peertube-viewer
> install -Dm755 peertube-viewer $(DESTDIR)/usr/bin/peertube-viewer
> install -Dm755 peertube-viewer.1 $(DESTDIR)/usr/share/man/man1/peertube-viewer.1
> gzip $(DESTDIR)/usr/share/man/man1/peertube-viewer.1

include $(DEPS)
