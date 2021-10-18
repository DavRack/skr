package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"os/exec"
	"syscall"
)

type InputEvent struct {
	Time  syscall.Timeval // time in seconds since epoch
	Type  uint16          // event type
	Code  uint16          // keycode
	Value int32           // key state: press, held or released
}

// values from evdev
var key_event uint16 = 1  // event.type
var keyPressed int32 = 1  // key state
var keyReleased int32 = 0 // key state

type KeyCode uint16
type KeyCodeList []KeyCode
type KeyName string
type KeyState int32
type Layers []KeyCodeList

type Key interface {
	keyCode() KeyCode
	keyName() KeyName
}

type KeyEvent struct {
	time     syscall.Timeval
	keyCode  KeyCode
	keyState KeyState // press, held, released
}

// values for KeyEvent
var pressed KeyState = 1
var held KeyState = 2
var released KeyState = 0

type Keyboard struct {
	executeDefaulAction bool
	ioReader            io.Reader
	lastKey             KeyEvent
	layers              Layers
	name                string
	path                string
	pressedKeys         KeyCodeList
	useConfigFile       bool
}

func main() {

	keyboard := initConfig()

	// create a process to read raw input data from interception tools
	_, keyboard.path = get_keyboard_path_from_name(keyboard.name)
	read_cmd := exec.Command("sudo", "intercept", keyboard.path)
	read_pipe, _ := read_cmd.StdoutPipe()
	read_cmd.Start()
	defer read_cmd.Wait()

	fmt.Println("skr")
	fmt.Println("Keyboard path", keyboard.path)

	keyboard.ioReader = bufio.NewReader(read_pipe)

	if keyboard.useConfigFile {
	} else {
		loop(keyboard)
	}
}

func (keyboard Keyboard) exist() bool {
	_, err := os.Stat(keyboard.path)
	return err == nil
}
