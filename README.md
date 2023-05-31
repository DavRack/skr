<h1 align="center"> Simple Key Remap </h1>

SKR is a low level, low latency way to remap keys. Some of the features of **SKR** are:

+ Remap keys
+ Remap keys to run a shell command or script
+ Remap keys to run a sequences of keys, commands, and scripts (A.K.A Macro) (needs documentation but it's there)
+ Function Layers
+ Multiple keyboard support

SKR sits very low in the keyboard stack just above the kernel. Thus, it is
independent of display protocol, desktop environment, or window manager.
It even works in tty.

### Table of Contents

+ [Installation](#installation)
  + [Arch Linux](#arch-linux)
  + [Ubuntu 20.04](#ubuntu-20.04)
  + [Fedora 32](#fedora-32)
  + [Uninstall](#uninstall-skr)
+ [Configuration](#configuration)
  + [Key remap](#key-remap)
  + [Scripts](#scripts)
  + [Layers](#layers)
+ [Auto start](#auto-start-systemd)


## Installation

#### Arch Linux

Install skr from the aur with the package **skr-git**

> example using [yay](https://github.com/Jguer/yay) AUR helper

```shell
yay -S skr-git

```

#### Ubuntu 20.04

First **install interception-tools** and git

```shell
sudo apt install git libudev-dev libevdev-dev libyaml-cpp-dev cmake build-essential
git clone https://gitlab.com/interception/linux/tools.git
cd tools
mkdir build
cd build
cmake ..
make
sudo make install
cd
```

Install skr
```shell
git clone https://github.com/DavRack/skr.git
cd skr
./install.sh
```

#### Fedora 32

First **install interception-tools** and git

```shell
sudo dnf install git cmake yaml-cpp-devel libevdev-devel systemd-devel gcc-c++ @development-tools
git clone https://gitlab.com/interception/linux/tools.git
cd tools
mkdir build
cd build
cmake ..
make
sudo make install
cd
```

Install skr

```shell
git clone https://github.com/DavRack/skr.git
cd skr
./install.sh
```

### Uninstall skr

```shell
./install.sh -u
```

## Configuration

> SKR is configured through **~/.config/skr/skr.config**. You can find an
example config file [here](docs/exampleConfig.txt)

Once skr is installed, run:

```shell
skr --init
```

This command will prompt you to press a key. skr will find the path
of the keyboard used to press the key and will create a config file

### Editing the config file

First we need to define the keyboard path for skr to intercept:

> if you ran **skr --init**, the keyboard path will have already been defined

```conf
KeyboardPath -> /dev/input/event3
```

#### Key remap

A key remap makes a key behave like it is another. It uses the following syntax:

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

A key swap remaps two keys to each other. It uses the following syntax:

```conf
[Key1] <-> [Key2]
```

This is equivalent to the following remaps

```conf
[Key1] -> [Key2]

[Key2] -> [Key1]

```

Example: Swap Meta/Super ("Windows" key) with left alt:

```conf
META <-> ALT
```

#### Scripts

SKR can map a key to a script or shell command. It uses the following syntax:

```conf
[KeyToRemap] -> Script=[your command or full path to a script]
```

Example: log memory usage to a file located in /tmp/memlog

```conf
[KeyToRemap] -> Script=free -h > /tmp/memlog
```

**Warning!** all commands launched from skr will be executed as root

#### Layers

A layer is a set of additional key bindings that can be reached by holding down
a key. It is easier to understand with an example:

Let's say you want to use **H J K L** as arrow keys (vim style). You can use
capsLock as your fnKey
When you hold it down and press H, J, K, or L they will act as arrow keys.
When the fnKey is not held, **H J K L** will work normally.

The syntax for layers is:

```shell
NewLayer -> [fnKey]
    # all remaps after NewLayer will only activate when [fnKey] is held down

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

The HJKL example would be written like this:

```shell
NewLayer -> CapsLock

    H -> LEFT
    J -> DOWN
    K -> UP
    L -> RIGHT
```

##### Some things to keep in mind about layers

+ When a Layer is defined, its function key stops working as a normal key

+ You can define anything inside a layer: remaps, scripts, and macros

## Auto start (systemd)

To run skr on boot first we need to create a unit file at
``/etc/systemd/system/skr@.service`` with the following content:

```systemd
[Unit]
Description=Low-Level key remapping

[Service]
User=root
Environment=USER=root
Environment=SUDO_USER=%i
ExecStart=/usr/bin/skr

[Install]
WantedBy=multi-user.target
```

Then enable the unit by running:

> replace your username where indicated

```shell
sudo systemctl enable skr@username.service
```

This will start the service at boot. To start skr immediately, run:

```shell
sudo systemctl enable --now skr@username.service
```
# Dependencies

+ [interception-tools](https://gitlab.com/interception/linux/tools)

+ sudo

### TODO

+ MD custom macros
+ Don't run non sudo commands as root
+ Remove dependencies
