package main

import "syscall"

// InputEvent godoc
type InputEvent struct {
	Time  syscall.Timeval // time in seconds since epoch
	Type  uint16          // event type
	Code  uint16          // keycode
	Value int32           // key state: press, held or released
}

// values from evdev
const keyEvent uint16 = 1   // event.type
const keyPressed int32 = 1  // key state
const keyReleased int32 = 0 // key state

type remap struct {
	trigger      []uint16
	action       []uint16
	blockKeys    bool
	keyEmulation bool
}

type script struct {
	trigger   []uint16
	action    string
	blockKeys bool
}

type action struct {
	script_action script
	remap_action  remap
}

type layer struct {
	layer_key uint16
	onTap     action
	actions   []action
}

type raw_key_action struct {
	code       uint16
	state      int32
	pre_delay  float32
	post_delay float32
}

type raw_script_action struct {
	script     string
	pre_delay  float32
	post_delay float32
}

var decided_action action
var pressedKeys []uint16
var rawInput InputEvent
