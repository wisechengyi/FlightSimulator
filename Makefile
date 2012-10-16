######################################################################
# File   : Makefile - automates building of the project.             #
# Author : Tim Sally                                                 #
######################################################################

MAT = ../../newmat

CXX        = g++
CXX_FLAGS  = -g -ansi -pedantic -Wall -Waddress -std=c++0x
LD_FLAGS   = -lglut -lGLU  -L$(MAT) -lnewmat


# - Files 
SRCS       = $(shell list=`/bin/ls . | grep -e "\.cpp"`; \
                          for i in $$list; do echo $$i; done)
OBJS       = $(SRCS:.cpp=.o)
TARGETS    = fly

.PHONY: $(TARGETS)

# --- Suffix Rules 

.SUFFIXES: 
.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $< $(LD_FLAGS)

# --- Targets
all:	newmatlib $(TARGETS) 

fly: $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $^ $(LD_FLAGS)
newmatlib:
	cd ../../newmat && make 
clean:
	rm -f fly *.o
clear:
	rm -f fly *.o
	cd newmat && make clean
