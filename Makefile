# Makefile for bewield
# Copyright 2021,2022 Scottsdale Community College
# Author: Sean Robinson <sean.robinson@scottsdalecc.edu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


BIN := bin
INC := include
LIB := lib

SOCAT := socat

CC := g++
CPPFLAGS := -I$(INC)
CXXFLAGS := -g -Wall -std=c++17 -fext-numeric-literals
LDFLAGS :=

# Names for the two ends of a virtual serial port for test running.
TEST_FAKE_PORT_A := port_a
TEST_FAKE_PORT_B := port_b

# Makefile helpers
VPATH := src

.DEFAULT_GOAL := help

.DELETE_ON_ERROR:

.PHONY: bewield clean fake_proj help realclean serial-pipe

$(BIN)/bewield: private LDFLAGS += $(LIB)/lineal.o

$(BIN)/bewield: bewield.cpp bewield.h $(INC)/argparse.hpp $(LIB)/lineal.o
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

$(BIN)/fake_proj: private LDFLAGS += $(LIB)/lineal.o

$(BIN)/fake_proj: fake_proj.cpp $(LIB)/lineal.o
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

$(LIB)/%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

bewield: $(BIN)/bewield

clean:
	$(RM) $(BIN)/*
	$(RM) -r $(LIB)/build

fake_proj: $(BIN)/fake_proj

help:
	@echo "bewield make targets:"
	@echo "  bewield - build bewield"
	@echo "  clean - remove ephemeral generated files (e.g. *.o)"
	@echo "  fake_proj - build test helper"
	@echo "  help - show this help message"
	@echo "  realclean - remove all generated files"
	@echo "  serial-pipe - create linked virtual serial ports for testing"
	@echo "  static-bewield - build statically linked bewield"

realclean: clean
	$(RM) -r $(LIB)/*.{d,o}

serial-pipe:
	$(SOCAT) -d -d PTY,raw,echo=0,link=$(TEST_FAKE_PORT_A) PTY,raw,echo=0,link=$(TEST_FAKE_PORT_B)

static-%: CXXFLAGS += -static

static-%: % ;
