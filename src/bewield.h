/*
    bewield.h - remote control using BenQ serial protocol
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


#ifndef BEWIELD_H
#define BEWIELD_H true

#include <algorithm>
#include <map>
#include <string>
#include <utility>


/* Map UI command string to protocol message. */
std::map<const std::string, const std::string> commands {
    std::make_pair("power_off", "pow=off"),
    std::make_pair("power_on", "pow=on"),

    std::make_pair("query_model", "modelname=?"),
};


/* Carriage Return byte used as leading and trailing value. */
constexpr char CR { 0x0d };

/* The two following characters delimit the bounds of a serial message. */
const std::string PREFIX { '*' };
const std::string SUFFIX { '#' };

// default serial port
const std::string DEFAULT_DEVICE { "/dev/ttyUSB0" };


/* Returns a new string with carriage returns placed with new-lines.  */
const std::string cook(const std::string msg) {
    auto slate { msg.substr() };
    std::replace(slate.begin(), slate.end(), '\r', '\n');
    return slate;
}

#endif
