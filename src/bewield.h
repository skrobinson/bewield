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


/* Map UI command string to protocol message.
 *
 * The order of entries is the order commands are displayed with
 * `--list-commands`, so keep to alphabetic order by pair.first.
 */
std::map<const std::string, const std::string> commands {
    std::make_pair("asource_hdmi1", "audiosour=hdmi"),
    std::make_pair("asource_hdmi2", "audiosour=hdmi2"),

    std::make_pair("audio_mute_off", "mute=off"),
    std::make_pair("audio_mute_on", "mute=on"),
    std::make_pair("audio_vol_down", "vol=-"),
    std::make_pair("audio_vol_up", "vol=+"),

    std::make_pair("blank_off", "blank=off"),
    std::make_pair("blank_on", "blank=on"),

    std::make_pair("power_off", "pow=off"),
    std::make_pair("power_on", "pow=on"),

    std::make_pair("query_audio_mute", "mute=?"),
    std::make_pair("query_audio_source", "audiosour=?"),
    std::make_pair("query_audio_volume", "vol=?"),
    std::make_pair("query_blank", "blank=?"),
    std::make_pair("query_model", "modelname=?"),
    std::make_pair("query_power", "pow=?"),
    std::make_pair("query_source", "sour=?"),

    std::make_pair("source_dp", "sour=dp"),
    std::make_pair("source_hdmi1", "sour=hdmi"),
    std::make_pair("source_hdmi2", "sour=hdmi2"),
    std::make_pair("source_rgb1", "sour=RGB"),
    std::make_pair("source_rgb2", "sour=RGB2"),
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
