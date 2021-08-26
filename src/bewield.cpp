/*
    bewield.cpp - remote control using BenQ serial protocol
    Copyright 2021 Scottsdale Community College
    Author: Sean Robinson <sean.robinson@scottsdalecc.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "bewield.h"
#include "lineal.h"

#include "argparse.hpp"

#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>


/* Returns an ArgumentParser object created from command line arguments. */
argparse::ArgumentParser read_args(const std::vector<std::string> arguments) {
    argparse::ArgumentParser program { "bewield" };

    program.add_argument("--verbose")
        .help("show detailed status")
        .default_value(false)
        .implicit_value(true);

    program.parse_args(arguments);

    return program;
}


int main(int argc, const char* argv[]) {
    argparse::ArgumentParser program;
    try {
        std::vector<std::string> args;
        std::copy(argv, argv + argc, std::back_inserter(args));

        program = read_args(args);
    } catch ( const std::runtime_error &e ) {
        std::cout << e.what() << std::endl;
        return EINVAL;
    }

    auto arg_verbose { program.get<bool>("--verbose") };

    return EXIT_SUCCESS;
}
