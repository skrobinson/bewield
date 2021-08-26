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
#include <iostream>
#include <memory>
#include <string>
#include <vector>


/* Returns an ArgumentParser object created from command line arguments. */
argparse::ArgumentParser read_args(const std::vector<std::string> arguments) {
    argparse::ArgumentParser program { "bewield" };

    program.add_argument("command")
        .default_value(std::string { "query_model" })
        .help("projector command");

    program.add_argument("-l", "--list-commands")
        .help("list commands and exit")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("-p", "--port")
        .help("serial port")
        .default_value(DEFAULT_DEVICE);

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

    if ( program.get<bool>("--list-commands") ) {
        std::cout << "bewield commands:" << std::endl;
        for ( const auto &[cmd, _unused] : commands ) {
            std::cout << "  " << cmd << std::endl;
        }
        return EXIT_SUCCESS;
    }

    auto arg_cmd { program.get("command") };
    auto arg_port { program.get("--port") };
    auto arg_verbose { program.get<bool>("--verbose") };

    if ( arg_verbose ) {
        std::cout << "opening " << arg_port << std::endl;
    }

    std::unique_ptr<Lineal> serial;
    try {
        serial = std::make_unique<Lineal>(arg_port);
    } catch ( const std::system_error &e ) {
        std::cout << e.what() << std::endl;
        return EINVAL;
    }

    if ( arg_verbose ) {
        std::cout << arg_port << " ready" << std::endl;
    }

    return EXIT_SUCCESS;
}
