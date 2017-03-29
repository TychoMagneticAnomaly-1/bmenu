![Version](https://img.shields.io/badge/Version-0.2.0-green.svg)

B-Menu
======

![Screenshot](http://www.brianbarto.info/extern/images/bmenu/bmenu.png)

What is B-Menu?
---------------

B-Menu is a minimalistic general purpose terminal menu written in C. It was designed to replace
terminal-based login managers such as CDM. However, it can easily serve as a simple terminal menu for
any situation where you would like to execute a command from a set of menu selections.

B-Menu has zero dependencies and it is intentionally feature-minimal. This keeps it easy to install, 
configure, and run. 

Note that bmenu uses ANSI/VT100 terminal escape sequences to draw the menu
in the terminal window. Most modern terminal programs support these sequences
so this should not be an issue for most users. If yours does not, the menu
may not render properly in your terminal window.

Installation
------------
To install this project from source, you will need to have the tools `git`,
`gcc`, and `make` to download and build it. Install them from your package
manager if they are not already installed.

Once you have the necessary tools installed, follow these instructions:

```
git clone https://github.com/bartobri/bmenu.git
cd ./bmenu/src
make
sudo make install
```

This will build and install `bmenu` in to the /usr/local/bin directory.

Configuring The Menu
--------------------

By default, b-menu looks for menu options in $HOME/.bmenu. This file
should consist of one menu option and one command on each line, seperated by a colon.

Example:

```
Clear Screen:/usr/bin/clear
Dir Listing:/usr/bin/ls -l
```

Command Line Options
--------------------

Use the `-c` option to override the default menu file path:
```
bmenu -c /full/path/to/menu/file
```

Use the `-t` option to override the default menu prompt:
```
bmenu -t 'Choose an Option:'
```

Set Up B-Menu as a Login Manager
--------------------------------

To run b-menu when you login, place the following code in `$HOME/.bash_profile`:

```
if [[ "$(tty)" == '/dev/tty1' ]]; then
    [[ -n "$BMENU_SPAWN" ]] && return
    export BMENU_SPAWN=1
    # Avoid executing bmenu when X11 has already been started.
    [[ -z "$DISPLAY$SSH_TTY$(pgrep xinit)" ]] && exec bmenu
fi
```

If you do this, it is best to include a menu option to launch a shell. Add the following to your menu
options file:

```
Shell:/bin/bash --login
```

License
-------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU 
General Public License as published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.  See [LICENSE](LICENSE) for more details.
