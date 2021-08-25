/*
    lineal.cpp - serial input/output for saint
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


#include "lineal.h"

#include <fcntl.h>
#include <iostream>
#include <string>
#include <system_error>
#include <termios.h>


// Wrap unistd to protect against compiler's confusion about Lineal class
// functions vs included functions.
namespace unistd {
    #include <unistd.h>
}


/* A wrapper around a Linux serial port.
 *
 * `serial_name` is the serial port path in the file system.
 */
Lineal::Lineal(std::string serial_name)
    : m_serial { serial_name }
{
    m_fd = open(m_serial.c_str(), O_RDWR | O_NOCTTY);
    if ( m_fd < 0 ) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                std::string("serial port open failed"));
    }

    // Configure the serial port via C functions.
    termios flags;
    if ( tcgetattr(m_fd, &flags) != 0 ) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                std::string("serial port configuration failed"));
    }
    if ( cfsetspeed(&flags, SERIAL_SPEED) != 0 ) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                std::string("serial port configuration failed"));
    }
    // Set saint's serial port configuration to match cecil.
    flags.c_cflag |= (flags.c_cflag & SERIAL_DATABITS);
    flags.c_cflag &= SERIAL_PARITY & SERIAL_STOPBITS;
    // Use noncanonical mode to avoid waiting for newline.
    flags.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // Set non-blocking access with 10 decisecond (1 sec) timeout.
    flags.c_cc[VMIN] = 0;
    flags.c_cc[VTIME] = 10;

    if ( tcsetattr(m_fd, TCSANOW, &flags) != 0 ) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                std::string("serial port configuration failed"));
    }
}

int Lineal::fd() {
    return m_fd;
}

/* Returns the quantity of bytes read from the serial port.
 *
 * The returned quantity `-1` indicates an error.
 *
 * The returned bytes are stuffed in `buffer` and `length` is the number of
 * bytes expected to be read.
 */
ssize_t Lineal::readBytes(char *buffer, std::size_t length) {
    return unistd::read(m_fd, buffer, length);
}

/* Returns the quantity of bytes written to the serial port.
 *
 * The returned quantity `-1` indicates an error.
 *
 * The bytes to be written are in `str` and `size` is the number of bytes
 * to write.
 */
ssize_t Lineal::write(const char *str, std::size_t size) {
    if ( str == nullptr ) {
        return 0;
    }
    return unistd::write(m_fd, str, size);
}
