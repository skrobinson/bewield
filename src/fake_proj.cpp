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

#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <termios.h>
#include <thread>
#include <utility>


/* Map UI protocol command message to response message. */
std::map<const std::string, const std::string> responses {
    std::make_pair("mute=off", "MUTE=OFF"),
    std::make_pair("mute=on", "MUTE=ON"),
    std::make_pair("vol=-", "VOL=-"),
    std::make_pair("vol=+", "VOL=+"),

    std::make_pair("blank=off", "BLANK=OFF"),
    std::make_pair("blank=on", "BLANK=ON"),

    std::make_pair("pow=off", "POW=OFF"),
    std::make_pair("pow=on", "Block item"),  // error return

    // query
    std::make_pair("mute=?", "MUTE=WHAT"),
    std::make_pair("vol=?", "VOL=0"),
    std::make_pair("blank=?", "BLANK=OFF"),
    std::make_pair("modelname=?", "MODELNAME=MW632ST"),
    std::make_pair("pow=?", "POW=OFF"),
    std::make_pair("sour=?", "SOUR=HDMI"),

    // change source
    std::make_pair("sour=dp", "SOUR=DP"),
    std::make_pair("sour=hdmi", "SOUR=HDMI"),
    std::make_pair("sour=hdmi2", "SOUR=HDMI2"),
    std::make_pair("sour=RGB", "SOUR=RGB"),
    std::make_pair("sour=RGB2", "Unsupported item"),  // error return
};


/* Constants to change runtime behavior. */
constexpr bool debug { false };
const std::string port { "port_b" };


/* Returns the full protocol string received.
 *
 * Because recv is reading a virtual serial port, it does not do any error
 * handling, including for incomplete messages.
*/
const std::string recv(Lineal &device) {
    char buffer[ 32 ];

    auto ret { device.readBytes(buffer, sizeof(buffer)) };

    if ( ret < 0 ) {
        std::cout << "Read failed for response" << std::endl;
        throw std::runtime_error("Fatal error while reading from bewield.");
    }
    if ( debug && (ret > 0) ) {
        std::cout << "read " << ret << " bytes" << std::endl;
        std::string msg { buffer };
        std::cout << cook(msg) << std::endl;
    }

    return std::string(buffer);
}


/* Sends a message to bewield through a virtual serial port and returns the
 * quantity of sent bytes.
 *
 * To simulate a noisy serial line, send "transmits" partial random-length
 * messages with random delays between "packets".
 */
std::size_t send(Lineal &device, const std::string response) {

    // Random seed source
    std::random_device rd;
    // Random number generator
    std::mt19937 gen(rd());
    // Functions returning random ints in different ranges.
    std::uniform_int_distribution<> random_delay { 20, 500 };
    std::uniform_int_distribution<> random_size { 1, 20 };

    std::size_t bytes_sent { 0 };
    std::size_t start { 0 };

    while ( start < response.length() ) {
        std::chrono::duration<int, std::milli> delay { random_delay(gen) };
        std::this_thread::sleep_for(delay);
        if ( debug ) {
            std::cout << "delayed: " << delay.count() << " ms" << std::endl;
        }

        auto random_length { random_size(gen) };
        auto partial_msg { response.substr(start, random_length) };
        start += random_length;

        auto ret { device.write(partial_msg.c_str(), partial_msg.length()) };
        if ( ret < 0 ) {
            std::cout << "Write failed for '" << partial_msg << "'" << std::endl;
            throw std::runtime_error("Fatal error while writing to bewield.");
        } else {
            bytes_sent += ret;
        }
        if ( debug ) {
            std::cout << "wrote " << ret << " bytes" << std::endl;
            std::cout << cook(partial_msg) << std::endl;
        }
    }

    return bytes_sent;
}


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

    while ( true ) {
        std::string cmd { recv(*serial) };
        auto start { cmd.rfind(PREFIX, std::string::npos) + 1 };
        auto end { cmd.rfind(SUFFIX, std::string::npos) };
        std::string cooked_cmd { cmd.substr(start, end-start) };

        if ( debug ) {
            std::cout << "cooked_cmd: " << cooked_cmd << std::endl;
        }

        std::string response;
        try {
            response = "  >*" + cooked_cmd + "#\r\r*" + responses.at(cooked_cmd) + "#\r";
            if ( debug ) {
                std::cout << "response: " << cook(response) << std::endl;
            }
        } catch ( const std::out_of_range &e ) {
            continue;
        }

        if ( ! cmd.empty() ) {
            if ( send(*serial, response) < response.length() ) {
                throw std::runtime_error("Fake response not fully sent.");
            }
        }
    }

    return EXIT_SUCCESS;
}
