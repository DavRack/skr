package keyboard

import "syscall"

// Key godoc
type Key uint16

// Keys godoc
type Keys []Key

// KeyEvent event.type has multiple values, when ther's a key event event.type = 1
const KeyEvent uint16 = 1

// Press godoc
const Press int32 = 1

// Released godoc
const Released int32 = 0

// PressedKeys godoc
var PressedKeys Keys

// InputEvent godoc
type InputEvent struct {
	Time  syscall.Timeval // time in seconds since epoch
	Type  uint16          // event type
	Code  uint16          // keycode
	Value int32           // key state: press, held or released
}

// State godoc
type State struct {
	RawInput  InputEvent
	PressKeys Keys
}
