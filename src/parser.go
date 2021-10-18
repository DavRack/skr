package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os/exec"
	"strings"
)

const noBlockDefault bool = false

func get_keyboard_path_from_name(name string) (error, string) {

	command := "sudo sed -n '/" + name + "/,/Handlers/p' /proc/bus/input/devices | grep -o 'event.*' -m 1"

	out, _ := exec.Command("bash", "-c", command).Output()
	if len(out) == 0 {
		return errors.New("Keyboard not found"), ""
	}
	input_event := strings.TrimSpace(string(out))

	return nil, "/dev/input/" + input_event

}

func pressKey(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: pressed,
	}
	keyEvent.execute()
	ok = true
	return
}

func releaseKey(key interface{}) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: released,
	}
	keyEvent.execute()
}

func (keyEvent KeyEvent) is(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	return keyEvent.keyCode == keyCode
}

func (keyEvent KeyEvent) execute() {
	state := ""
	if keyEvent.keyState == pressed {
		state = "keyPressed"
	}
	if keyEvent.keyState == released {
		state = "keyReleased"
	}
	if keyEvent.keyState == held {
		state = "keyHeld"
	}

	fmt.Println("KeyCode:", keyEvent.keyCode, "Event type:", state)
}

func (keyboard *Keyboard) keyRemap(fromKey KeyCode, toKey KeyCode, _blockKey ...bool) (executed bool) {
	blockKey := true
	if len(_blockKey) > 0 {
		blockKey = _blockKey[0]
	}

	if keyboard.lastKey.keyCode == fromKey {
		remapedKey := keyboard.lastKey
		remapedKey.keyCode = toKey
		remapedKey.execute()
		keyboard.executeDefaulAction = !blockKey
		return true
	}
	return false
}

func (pressedKeys KeyCodeList) startsWith(layer KeyCodeList) bool {
	for j, key := range layer {
		if pressedKeys[j] != key {
			return false
		}
	}
	return true
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

func loop(keyboard Keyboard) {
	var raw_input InputEvent
	for keyboard.exist() {
		// read event from keyboard
		binary.Read(keyboard.ioReader, binary.LittleEndian, &raw_input)

		if raw_input.Type == key_event {

			keyEvent := KeyEvent{
				time:     raw_input.Time,
				keyCode:  KeyCode(raw_input.Code),
				keyState: KeyState(raw_input.Value),
			}

			// if a key is pressed we ned to added to pressedKeys to perform
			// the needed action, but if a key is released we also need to
			// perform the key action, so we need to remove the key from
			// pressedKeys after we perform such action
			if raw_input.Value == keyPressed {
				keyboard.pressedKeys = keyboard.get_press_keys(keyEvent)
			}

			keyboard.executeDefaulAction = true
			keyboard.lastKey = keyEvent

			blockCurrentKey := skrConfig(&keyboard)

			if blockCurrentKey {
				keyboard.executeDefaulAction = false
			}

			if keyboard.executeDefaulAction {
				keyEvent.execute()
			}

			keyboard.pressedKeys = keyboard.get_press_keys(keyEvent)
		}
	}
}
