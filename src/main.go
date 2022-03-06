package main

import (
	"os"
	"syscall"
)

type EventType uint16
type KeyCode uint16
type KeyCodeList []KeyCode
type KeyName string
type KeyState int32

type KeyboardEvent struct {
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
	IO                  KeyboardIO
	lastKey             KeyEvent
	layers              Layers
	name                string
	path                string
	pressedKeys         KeyCodeList
	useConfigFile       bool
}

func main() {

	keyboard := initConfig()
	keyboard.IO = initKeyboardStdinIO(keyboard.name)

	if keyboard.useConfigFile {
	} else {
		loop(&keyboard, skrConfig)
	}
}

func (keyboard Keyboard) exist() bool {
	_, err := os.Stat(keyboard.path)
	return err == nil
}
