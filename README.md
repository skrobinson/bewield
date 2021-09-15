bewield
=======

bewield is a serial remote control for the BenQ MW632ST projector.
While bewield may work with other BenQ models, it has not been tested
on any other projectors.  bewield is unlikely to work with any
non-BenQ projectors.


Help Output
-----------

```bash
Usage: bewield [options] command

Positional arguments:
command            	projector command [default: "query_model"]

Optional arguments:
-h --help          	shows help message and exits
-v --version       	prints version information and exits
-l --list-commands 	list commands and exit [default: false]
-p --port          	serial port [default: "/dev/ttyUSB0"]
--verbose          	show detailed status [default: false]
```


Commands
--------

A list of commands can be seen with the `--list-commands` argument.

| Command            | Outcome                       |
| :------------------| :---------------------------- |
| asource_hdmi1      | Use HDMI 1 audio source       |
| asource_hdmi2      | Use HDMI 2 audio source       |
| audio_mute_off     | Unmute audio                  |
| audio_mute_on      | Mute audio                    |
| audio_vol_down     | Lower volume                  |
| audio_vol_up       | Raise volume                  |
| blank_off          | Unblank screen                |
| blank_on           | Blank screen                  |
| power_off          | Turn off projector power      |
| power_on           | Turn on projector power       |
| query_audio_mute   | Returns mute status (ON/OFF)  |
| query_audio_source | Returns current audio source  |
| query_audio_volume | Returns current volume level  |
| query_blank        | Returns blank status (ON/OFF) |
| query_model        | Returns projector model       |
| query_power        | Returns power status (ON/OFF) |
| query_source       | Returns current video source  |
| source_dp          | Use DisplayPort video source  |
| source_hdmi1       | Use HDMI 1 video source       |
| source_hdmi2       | Use HDMI 2 video source       |
| source_rgb1        | Use RGB 1 video source        |
| source_rgb2        | Use RGB 2 video source        |
