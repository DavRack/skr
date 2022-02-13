package main

import (
	"encoding/binary"
)

const noBlockDefault bool = false

func (keyEvent KeyEvent) is(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	return keyEvent.keyCode == keyCode
}

func (keyboard *Keyboard) keyRemap(_fromKey interface{}, _toKey interface{}, _blockKey ...bool) (executed bool) {
	fromKey, ok := interfaceToKeyCode(_fromKey)
	toKey, ok := interfaceToKeyCode(_toKey)
	if !ok {
		return
	}

	blockKey := true
	if len(_blockKey) > 0 {
		blockKey = _blockKey[0]
	}

	if keyboard.lastKey.keyCode == fromKey {
		remapedKey := keyboard.lastKey
		remapedKey.keyCode = toKey
		keyboard.execute(remapedKey)
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

func loop(keyboard *Keyboard) {
	var raw_input InputEvent
	for keyboard.exist() {
		// read event from keyboard
		binary.Read(keyboard.ioReader, binary.LittleEndian, &raw_input)
		// fmt.Printf("time1: %d, time2: %d, Type: %d, Code: %d, Value: %d\n",
		// 	raw_input.Time.Sec, raw_input.Time.Usec, raw_input.Type, raw_input.Code, raw_input.Value)

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

			blockCurrentKey := skrConfig(keyboard)

			if blockCurrentKey {
				keyboard.executeDefaulAction = false
			}

			if keyboard.executeDefaulAction {
				keyboard.execute(keyEvent)
			}

			keyboard.pressedKeys = keyboard.get_press_keys(keyEvent)
		}
	}
}
