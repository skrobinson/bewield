/*
    lineal.h - serial input/output for saint
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


#ifndef LINEAL_H
#define LINEAL_H true

#include <string>
#include <termios.h>


/* Timeout, in milliseconds, for polling on read from the serial port.
 *
 * Another way to look at this: how often are writes allowed?
 * A value of { 1000 / 5 } will allow a write 5 times per second.
 */
constexpr int POLL_TIMEOUT { 1000 / 5 };

/* The following SERIAL_* values should match the settings for SERIAL_SPEED
 * and SERIAL_CONF in cecil.h.  Match the concepts, not the naive values.
 * For example, B9600 = 15, but means the serial port should run at 9600 bps.
 */

/* data size is 8 bits */
constexpr auto SERIAL_DATABITS { CSIZE & CS8 };

/* disable all parity bits */
constexpr auto SERIAL_PARITY { ~(PARENB | PARODD) };

/* Serial port speed, in baud.  See termios.h. */
constexpr speed_t SERIAL_SPEED { B9600 };

/* 1 stop bit */
constexpr auto SERIAL_STOPBITS { ~CSTOPB };


/* A minimally compatible interface like Arduino Serial, but for Linux. */
class Lineal {

    private:

        /* Begin with an invalid file descriptor. */
        int m_fd { -1 };

        /* Path to the serial port. */
        std::string m_serial;

    public:

        Lineal(std::string serial_name);

        int fd();
        ssize_t readBytes(char *buffer, std::size_t length);
        ssize_t write(const char *str, std::size_t size);

        /* Returns true if the internal file descriptor holds a valid value. */
        operator bool() const {
            return m_fd > -1;
        }

};


#endif
