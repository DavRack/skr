<h1 align="center"> Simple Key Remap </h1>

SKR is a low level, low latency way to remap keys to:

+ Other keys
+ Shell commands or scripts
+ Sequences of keys, commands and scripts (A.K.A Macros)
+ Function Layers

SKR sits very low in the keyboard stack just above the kernel so its
independent of desktop environments, window manager or graphical environments
in general, even works in tty.

### Table of Contents

+ [Installation](#installation)
+ [Configuration](#configuration)
  + [Key remap](#key-remap)
  + [Scripts](#scripts)
  + [Layers](#layers)
+ [Auto start](#auto-start-(systemd))


## Installation

First **install interception-tools** and git

#### On Arch Linux

Install the interception-tools from the aur

> example using [yay](https://github.com/Jguer/yay) AUR helper

```shell
yay -S git interception-tools

```

#### On Ubuntu 20.04

```shell
sudo apt install git libudev-dev libevdev-dev libyaml-cpp-dev cmake build-essential
git clone https://gitlab.com/interception/linux/tools.git
cd tools
mkdir build
cd build
cmake ..
make
sudo make install
```

### Install skr
docs/exampleConfig.txt
```shell
git clone https://github.com/DavRack/skr.git
cd skr
./install.sh
```

To uninstall skr:

```shell
./install.sh -u
```

## Configuration

> SKR is configured through **~/.config/skr/skr.config** you can find an
example config file [here](docs/exampleConfig.txt)

Once skr is installed  run:

```shell
skr --init
```

This command will prompt you to press a key. skr will find the path
of the keyboard used to press the key and will create a config file

### Editing the config file

First we need to define the keyboard path that skr will intercept:

> if you run **skr --init** the keyboard path will be already defined

```conf
KeyboardPath -> /dev/input/event3
```

#### Key remap

A key remap takes one key and convert its to another key and has this syntax:

```conf
[keyToRemap] -> [targetKey]
```

Example: Remap CapsLock to Esc

```conf
CapsLock -> Esc
```

skr takes its key names from /usr/include/linux/input-event-codes.h

Here's a key code table with all valid key names: [keycode table](docs/keyCodes.md)

#### Key swap

A Key Swap the function of two keys and has this syntax:

```conf
[Key1] <-> [Key2]
```

A key swap its equivalent to two remaps

```conf
[Key1] -> [Key2]

[Key2] -> [Key1]

```

Example: Swap Meta/Super ("Windows" key) with left alt:

```conf
META <-> ALT
```

#### Scripts

SKR can use any key to launch a script or shell command and has this syntax:

```conf
[KeyToRemap] -> Script=[your command or full path to a script]
```

Example: log memory usage to a file located in /tmp/memlog

```conf
[KeyToRemap] -> Script=free -h > /tmp/memlog
```

**Warning!** all commands launched from skr will be executed as root

#### Layers

A Layer is a set of additional key bindings achieved by
holding down a predefined key on the keyboard, its easier to understand
with an example:

Let's say you want to use **H J K L** as arrow keys (vim style), you can set
capsLock as a fnKey
so when you hold it down and press H,J,K or L this keys will act as arrow keys,
but when the fnKey is not press **H J K L** will work normally.

The syntax for layers is:

```shell
NewLayer -> [fnKey]
    # all remaps after NewLayer will activate when [fnKey] is hold down

    # remap 1

    # remap 2
    .
    .
    .


# you can define multiple layers
NewLayer -> [fnKey2]
    # all remaps after NewLayer will activate when [fnKey2] is hold down

    # remap 1

    # remap 2
    .
    .
    .
```

The H J K L as arrows example would look like this on the config file:

```shell
NewLayer -> CapsLock

    H -> LEFT
    J -> DOWN
    K -> UP
    L -> RIGHT
```

##### Some things to keep in mind about layers

+ When a Layer is defined its function key stops working as a normal key

+ You can define anything inside a layer: Remaps, Scripts and Macros

## Auto start (systemd)

To run skr on boot first we need to create a unit file at
``/etc/systemd/system/skr.service`` with the following content:

> replace your username where indicated
```systemd
[Unit]
Description=undervolt

[Service]
User=root
Environment=USER=putUserNameHere
Environment=SUDO_USER=putUserNameHere
ExecStart=/usr/bin/skr

[Install]
WantedBy=multi-user.target
```

Then enable the unit running:

```shell
sudo systemctl enable skr.service
```

this will start the service at boot. to start skr imediatly run:

```shell
sudo systemctl start skr.service
```
# Dependencies

+ [interception-tools](https://gitlab.com/interception/linux/tools)

+ sudo

### TODO

+ MD install
+ MD custom macros
+ Don't run non sudo commands as root
+ Automatic start
+ Remove dependencies
+ Install on Linux
