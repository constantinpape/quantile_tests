SHELL = /bin/sh

#### System Configuration #####

CC = g++
CDEBUG = -g3 -Wall
CFLAGS = -std=c++0x -O2

INC_LOCAL = ~/Work/software/inst/include
INC_USER  =  /usr/include

ALL_FLAGS = -I $(INC_LOCAL) -I $(INC_USER) $(CFLAGS)

# paths
prefix := .

#### End of system configuration section ####

# target test_quantiles
test_quantiles: $(prefix)/test_quantiles.cpp
	$(CC) $(ALL_FLAGS) -o $(prefix)/$@ $^ 

