package main

import (
	"bufio"
	"os"
	"os/exec"
	"syscall"
)

type EventType uint16
type KeyCode uint16
type KeyCodeList []KeyCode
type KeyName string
type KeyState int32

type InputEvent struct {
	Time  syscall.Timeval // time in seconds since epoch
	Type  EventType       // event type
	Code  KeyCode         // keycode
	Value KeyState        // key state: press, held or released
}

// values from evdev
var key_event EventType = 1  // event.type
var keyPressed KeyState = 1  // key state
var keyReleased KeyState = 0 // key state

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
	ioReader            *bufio.Reader
	ioWriter            *bufio.Writer
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

	write_cmd := exec.Command("uinput", "-d", keyboard.path)
	write_pipe, _ := write_cmd.StdinPipe()
	write_cmd.Start()
	defer write_cmd.Wait()
	keyboard.ioWriter = bufio.NewWriter(write_pipe)

	read_cmd := exec.Command("intercept", "-g", keyboard.path)
	read_pipe, _ := read_cmd.StdoutPipe()
	read_cmd.Start()
	defer read_cmd.Wait()
	keyboard.ioReader = bufio.NewReader(read_pipe)

	if keyboard.useConfigFile {
	} else {
		loop(&keyboard)
	}
}

func (keyboard Keyboard) exist() bool {
	_, err := os.Stat(keyboard.path)
	return err == nil
}
