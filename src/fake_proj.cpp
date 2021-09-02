/*
    fake_proj.cpp - fake projector serial interface
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

#include <map>
#include <memory>
#include <termios.h>
#include <utility>


/* Map UI protocol command message to response message. */
std::map<const std::string, const std::string> responses {
    std::make_pair("pow=off", "POW=OFF"),
    std::make_pair("pow=on", "Block item"),  // error return

    // query
    std::make_pair("modelname=?", "MODELNAME=MW632ST"),
};


/* Constants to change runtime behavior. */
const std::string port { "port_b" };


int main(int argc, const char* argv[]) {

    std::unique_ptr<Lineal> serial;
    try {
        serial = std::make_unique<Lineal>(port);
    } catch ( const std::system_error &e ) {
        std::cout << e.what() << std::endl;
        return EINVAL;
    }

    // Flush erroneous, pending IO before continuing.
    tcflush(serial->fd(), TCIOFLUSH);

    return EXIT_SUCCESS;
}
