# Makefile for bewield
# Copyright 2021 Scottsdale Community College
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

CC := g++
CPPFLAGS := -I$(INC)
CXXFLAGS := -g -Wall -std=c++17 -fext-numeric-literals
LDFLAGS :=

# Makefile helpers
VPATH := src

.DEFAULT_GOAL := help

.DELETE_ON_ERROR:

.PHONY: bewield clean help realclean

$(BIN)/bewield: private LDFLAGS += $(LIB)/lineal.o

$(BIN)/bewield: bewield.cpp bewield.h $(INC)/argparse.hpp $(LIB)/lineal.o
	$(CC) -static $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

$(LIB)/%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

bewield: $(BIN)/bewield

clean:
	$(RM) $(BIN)/*
	$(RM) -r $(LIB)/build

help:
	@echo "bewield make targets:"
	@echo "  bewield - build bewield"
	@echo "  clean - remove ephemeral generated files (e.g. *.o)"
	@echo "  help - show this help message"
	@echo "  realclean - remove all generated files"

realclean: clean
	$(RM) -r $(LIB)/*.{d,o}
