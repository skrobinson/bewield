bewield
=======

bewield is a serial remote control for the BenQ MW632ST projector.
While bewield may work with other BenQ models, it has not been tested
on any other projectors.  bewield is unlikely to work with any
non-BenQ projectors.


Commands
--------

A list of commands can be seen with the `--list-commands` argument.


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
