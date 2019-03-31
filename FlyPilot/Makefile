#Compiler and Linker
CC                 := g++
AR                 := ar
MAKE               := make
CFLAGS             := -std=gnu++14 -ggdb3 -O0 -Wall -Werror

#Top level
TLD                := .
GTEST              := ../libs/gtest.o

#external lib
PTREE_STATIC_LIB   := ../../PropertyTree/clientlib/client.a ../../PropertyTree/clientlib/common.a

#TARGET
AUTOPILOT_TARGET      := bin/autopilot

#The Directories, Source, Includes, Objects, Binary and Resources
AUTOPILOT_SRCDIR         := src
AUTOPILOT_UTDIR          := tests/ut

AUTOPILOT_INCDIR         := .
AUTOPILOT_TESTINCDIR     := $(AUTOPILOT_INCDIR) ../gtest
AUTOPILOT_UT_LD          := pthread m
AUTOPILOT_LD          := pthread m

TESTFLAG			  :=

ifneq ($(strip $(TEST)),)
TESTFLAG := --gtest_filter="$(TEST)"
endif

## TARGET DEBUG #########################################################################

BUILDDIR              := build/normal
BUILDDIR_GCOV         := build/gcov

AUTOPILOT_INCDIR_GCC     := $(addprefix -I, $(AUTOPILOT_INCDIR))
AUTOPILOT_TESTINCDIR_GCC := $(addprefix -I, $(AUTOPILOT_TESTINCDIR))
AUTOPILOT_UT_LD_GCC 	  := $(addprefix -l, $(AUTOPILOT_UT_LD))
AUTOPILOT_LD_GCC 	  := $(addprefix -l, $(AUTOPILOT_UT_LD))
AUTOPILOT_SOURCES        := $(shell find $(AUTOPILOT_SRCDIR) -type f -name *.cpp -not -name main.cpp)
AUTOPILOT_TEST_SOURCES   := $(shell find $(AUTOPILOT_UTDIR) -type f -name *.cpp)
AUTOPILOT_OBJECTS        := $(addprefix $(BUILDDIR)/, $(AUTOPILOT_SOURCES:.cpp=.cpp.o))
AUTOPILOT_OBJECTS_GCOV   := $(addprefix $(BUILDDIR_GCOV)/, $(AUTOPILOT_SOURCES:.cpp=.cpp.o))
AUTOPILOT_TESTS_OBJECTS  	   := $(addprefix $(BUILDDIR)/, $(AUTOPILOT_TEST_SOURCES:.cpp=.cpp.o))
AUTOPILOT_TESTS_OBJECTS_GCOV  := $(addprefix $(BUILDDIR_GCOV)/, $(AUTOPILOT_TEST_SOURCES:.cpp=.cpp.o))

AUTOPILOT_DEPS = $(AUTOPILOT_OBJECTS:%.o=%.d)
AUTOPILOT_TEST_DEPS = $(AUTOPILOT_TESTS_OBJECTS:%.o=%.d)

-include $(AUTOPILOT_DEPS)
-include $(AUTOPILOT_TEST_DEPS)

## TARGET GTEST #########################################################################

$(GTEST):
	@echo "Building gtest..."
	$(MAKE) -C ../gtest all

## PTREE
$(PTREE_STATIC_LIB):
	@echo "Make sure that clientlib is built."

## TARGET autopilot #########################################################################

autopilot: $(PTREE_STATIC_LIB) $(AUTOPILOT_OBJECTS)
	@mkdir -p $(AUTOPILOT_TARGET)
	@echo Linking $(AUTOPILOT_TARGET)/autopilot
	@$(CC) $(CFLAGS) $(AUTOPILOT_INCDIR_GCC) -g -c $(AUTOPILOT_SRCDIR)/main.cpp -o $(BUILDDIR)/$(AUTOPILOT_SRCDIR)/main.cpp.o
	@$(CC) -g $(AUTOPILOT_OBJECTS) $(AUTOPILOT_LD_GCC) $(PTREE_STATIC_LIB) $(BUILDDIR)/$(AUTOPILOT_SRCDIR)/main.cpp.o -o $(AUTOPILOT_TARGET)/autopilot

$(AUTOPILOT_OBJECTS): $(BUILDDIR)/%.cpp.o : %.cpp
	@mkdir -p $(@D)
	@echo "Building SOURCE" $@
	@$(CC) $(CFLAGS) $(AUTOPILOT_INCDIR_GCC) -MMD -g -c $(patsubst $(BUILDDIR)/%.cpp.o,%.cpp,$@) -o $@

## TARGET autopilot UT #########################################################################

autopilot_ut: $(GTEST) $(PTREE_STATIC_LIB) $(AUTOPILOT_OBJECTS) $(AUTOPILOT_TESTS_OBJECTS)
	@mkdir -p $(AUTOPILOT_TARGET)
	@echo Linking $(AUTOPILOT_TARGET)/autopilot_ut
	@$(CC) -g $(GTEST) $(AUTOPILOT_UT_LD_GCC) $(AUTOPILOT_OBJECTS) $(AUTOPILOT_TESTS_OBJECTS) $(PTREE_STATIC_LIB) -o $(AUTOPILOT_TARGET)/autopilot_ut

autopilot_ut_run: autopilot_ut
	$(AUTOPILOT_TARGET)/autopilot_ut $(TESTFLAG)

autopilot_ut_valgrind_run: autopilot_ut
	valgrind --leak-check=full --show-leak-kinds=all -v $(AUTOPILOT_TARGET)/autopilot_ut $(TESTFLAG)

autopilot_ut_valgrind_run_detailed: autopilot_ut
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -v $(AUTOPILOT_TARGET)/autopilot_ut $(TESTFLAG)

$(AUTOPILOT_TESTS_OBJECTS): $(BUILDDIR)/%.cpp.o : %.cpp
	@mkdir -p $(@D)
	@echo "Building TEST" $@
	@$(CC) $(CFLAGS) $(AUTOPILOT_TESTINCDIR_GCC) -MMD -g -c $(patsubst $(BUILDDIR)/%.cpp.o,%.cpp,$@) -o $@

clean_autopilot_ut:
	echo cleaning ut
	find $(BUILDDIR)/$(AUTOPILOT_UTDIR) -type f -name *.cpp.o -exec rm {} \;

clean_autopilot: clean_autopilot_ut
	echo cleaning autopilot
	find $(BUILDDIR)/$(AUTOPILOT_SRCDIR) -type f -name *.cpp.o -exec rm {} \;

clean_gcov:
	echo cleaning GCOV
	find $(BUILDDIR_GCOV) -type f -name *.cpp.o -exec rm {} \;
	find $(BUILDDIR_GCOV) -type f -name *.gcda -exec rm {} \;
	find $(BUILDDIR_GCOV) -type f -name *.gcno -exec rm {} \;

clean: clean_client_ut clean_client clean_autopilot_ut clean_autopilot clean_gcov
	echo All cleaned
