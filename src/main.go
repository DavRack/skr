package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"os/exec"
	"reflect"
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
type KeyState int32
type KeyCodeList []KeyCode

type KeyEvent struct {
	time     syscall.Timeval
	keyCode  KeyCode
	keyState KeyState // press, held, released
}

// values for KeyEvent
var pressed KeyState = 1
var held KeyState = 2
var released KeyState = 0

type remap struct {
	trigger []uint16
	action  []uint16
	// blockKeys lets the user decide if the trigger keys will be blocked
	// when executing action
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

type Keyboard struct {
	name        string
	path        string
	ioReader    io.Reader
	pressedKeys KeyCodeList
	lastKey     KeyCode
}

var decided_action action
var pressedKeys []uint16

func main() {

	keyboard := Keyboard{}

	keyboard.name = "ASUSTeK Computer Inc. N-KEY Device"

	// create a process to read raw input data from interception tools
	_, keyboard.path = get_keyboard_path_from_name(keyboard.name)
	read_cmd := exec.Command("sudo", "intercept", keyboard.path)
	read_pipe, _ := read_cmd.StdoutPipe()
	read_cmd.Start()
	defer read_cmd.Wait()

	fmt.Println("skr")
	fmt.Println("Keyboard path", keyboard.path)

	keyboard.ioReader = bufio.NewReader(read_pipe)

	loop(keyboard)

	// for keyboard_exist(keyboard_path) {
	// 	// read event from keyboard
	// 	binary.Read(keyboard_in, binary.LittleEndian, &raw_input)

	// 	if raw_input.Type == keyEvent {
	// 		// if a key is pressed we ned to added to pressedKeys to perform
	// 		// the needed action, but if a key is released we also need to
	// 		// perform the key action, so we need to remove the key from
	// 		// pressedKeys after we perform such action
	// 		if raw_input.Value == keyPressed {
	// 			pressedKeys = get_press_keys(raw_input, pressedKeys)
	// 		}

	// 		fmt.Println(pressedKeys, raw_input.Value)
	// 		decided_action = decide_actions(pressedKeys, layers)
	// 		raw_keys, raw_scripts := get_raw_events(decided_action, raw_input)

	// 		execute_raw_keys(raw_keys)
	// 		execute_raw_scripts(raw_scripts)

	// 		pressedKeys = get_press_keys(raw_input, pressedKeys)
	// 	}
	// }
}
func execute_raw_keys(raw_keys []raw_key_action) {
	for _, raw_key := range raw_keys {
		fmt.Println("keys", raw_key)
	}
}
func execute_raw_scripts(raw_scripts []raw_script_action) {
	for _, raw_script := range raw_scripts {
		fmt.Println("script", raw_script)
	}
}

func keyboard_exist(keyboard_path string) bool {
	_, err := os.Stat(keyboard_path)
	return err == nil
}

func (keyboard Keyboard) get_press_keys(key_event KeyEvent) KeyCodeList {
	pressedKeys := keyboard.pressedKeys
	if key_event.keyState == pressed {
		if !keyboard.pressedKeys.contains(key_event.keyCode) {
			pressedKeys = append(keyboard.pressedKeys, key_event.keyCode)
		}

	} else if key_event.keyState == released {
		pressedKeys = keyboard.pressedKeys.delete(key_event.keyCode)

	}
	return pressedKeys
}

func decide_actions(pressKeys []uint16, layers []layer) action {
	// evaluate each layer to find remaps
	L := get_layer(pressKeys, layers)
	actions := L.actions

	for _, a := range actions {
		r := a.remap_action
		s := a.script_action

		if match_keypress_subset(pressKeys, s.trigger) {
			return a
		} else if match_keypress_subset(pressKeys, r.trigger) {
			return a
		}
	}

	// return an empty/default action in case of no match
	return action{}
}

func get_layer(pressKeys []uint16, layers []layer) layer {
	if len(layers) == 0 {
		return layer{}
	}
	if len(pressKeys) > 0 {
		// get the firs pressed key
		first_pressed_key := pressKeys[0]

		for _, l := range layers {
			if first_pressed_key != 0 && first_pressed_key == l.layer_key {
				return l
			}
		}
	}
	return layers[0]
}

// match_keypress_subset returns true if "subset" is a subset of "list"
func match_keypress_subset(list []uint16, subset []uint16) bool {
	if len(list) == 0 || len(subset) == 0 {
		return false
	}

	if len(list) < len(subset) {
		return false
	}

	con := len(list) - 1
	for i := len(subset) - 1; i > -1; i-- {
		if subset[i] != list[con] {
			return false
		}
		con--
	}
	return true
}

func (_action action) isEmpty() bool {
	return reflect.DeepEqual(_action, action{})
}

func (_remap remap) isEmpty() bool {
	return reflect.DeepEqual(_remap, remap{})
}

func get_raw_events(decided_action action, raw_input InputEvent) ([]raw_key_action, []raw_script_action) {

	if decided_action.isEmpty() {
		// if theres no selec
		return []raw_key_action{}, []raw_script_action{}

	} else if !decided_action.remap_action.isEmpty() {
		// if action is a remap action
		raw_key_actions := []raw_key_action{}

		var raw_action raw_key_action

		if len(decided_action.remap_action.action) > 1 {

		} else {
			raw_action = raw_key_action{
				decided_action.remap_action.action[0],
				raw_input.Value,
				0,
				0,
			}
			raw_key_actions = append(raw_key_actions, raw_action)
		}

		return raw_key_actions, []raw_script_action{}

	} else {
		// get raw_script_actions
		raw_script_actions := []raw_script_action{}

		var raw_action raw_script_action

		raw_action = raw_script_action{
			decided_action.script_action.action,
			0,
			0,
		}
		raw_script_actions = append(raw_script_actions, raw_action)
		return []raw_key_action{}, raw_script_actions
	}
}
