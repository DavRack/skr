package keyboard

import (
	"strings"
)

const noBlockDefault bool = false

func (keyEvent KeyEvent) Is(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCodes(key)
	if !ok {
		return
	}
	if len(keyCode) != 1 {
		return false
	}
	return keyEvent.KeyCode == keyCode[0]
}

func (keyboard *Keyboard) KeyRemap(_fromKeys interface{}, _toKeys interface{}, _blockKey ...bool) (executed bool) {
	fromKeys, ok := interfaceToKeyCodes(_fromKeys)
	toKeys, ok := interfaceToKeyCodes(_toKeys)
	if !ok {
		return
	}

	if keyboard.PressedKeys.EndsWithSubset(fromKeys) {
		keyboard.SomeActionExecuted = true
		if keyboard.PressedKeys.EndsWith(fromKeys) {
			keyboard.SendKeyComb(toKeys, keyboard.LastKey.KeyState)
			keyboard.BlockedEvents[fromKeys.ToString()] = []KeyEvent{}
		} else {
			if keyboard.LastKey.KeyState == KeyPressed {
				blockedEvents := keyboard.BlockedEvents[fromKeys.ToString()]
				blockedEvents = append(blockedEvents, keyboard.LastKey)
				keyboard.BlockedEvents[fromKeys.ToString()] = blockedEvents
			} else if keyboard.LastKey.KeyState == released {
				keyboard.ExecuteAll(keyboard.BlockedEvents[fromKeys.ToString()])
			}
		}
	}

	if keyboard.LastKey.KeyCode.In(fromKeys) {
		keyboard.SomeActionExecuted = true
	}

	return false
}

func (keyCode KeyCode) In(keyCodes KeyCodeList) bool {
	for _, _keyCode := range keyCodes {
		if keyCode == _keyCode {
			return true
		}
	}
	return false
}

func (keyCodes KeyCodeList) ToString() string {
	var keyNames []string

	for _, keyCode := range keyCodes {
		keyNames = append(keyNames, string(keyCode.KeyName()))
	}

	return strings.Join(keyNames, " + ")
}

func (keyboard *Keyboard) ExecuteAll(events []KeyEvent) {
	for _, event := range events {
		keyboard.Execute(event)
	}
}

func (pressedKeys KeyCodeList) EndsWithSubset(keys KeyCodeList) bool {
	if len(keys) == 0 {
		return false
	}

	// find fisrt matching key
	firstKey := keys[0]
	firstKeyIndex := -1
	for i, pressedKey := range pressedKeys {
		if pressedKey == firstKey {
			firstKeyIndex = i
		}
	}

	if firstKeyIndex == -1 {
		return false
	}

	for i := firstKeyIndex; i < len(pressedKeys); i++ {
		if pressedKeys[i] != keys[i-firstKeyIndex] {
			return false
		}
	}

	return true
}

func (pressedKeys KeyCodeList) EndsWith(keys KeyCodeList) bool {
	if len(keys) > len(pressedKeys) {
		return false
	}

	if len(keys) == 0 {
		return false
	}

	endsWithOffset := len(pressedKeys) - len(keys)

	for i := endsWithOffset; i < len(pressedKeys); i++ {
		if pressedKeys[i] != keys[i-endsWithOffset] {
			return false
		}
	}

	return true
}

func (keyboard *Keyboard) SendKeyComb(keys KeyCodeList, state KeyState) (excecuted bool) {

	if len(keys) == 0 {
		return false
	}

	if state == KeyPressed {
		// send all keys in press state
		for _, key := range keys {
			keyboard.PressKey(key)
		}
		return true
	}

	if state == KeyReleased {
		// send all keys in reverse order and released state
		for i := len(keys) - 1; i >= 0; i-- {
			key := keys[i]
			keyboard.ReleaseKey(key)
		}
		return true
	}

	return false
}

func (keyboard *Keyboard) ReleaseKeyComb(keys KeyCodeList) (excecuted bool) {
	// takes a list of keys eg: [29, 30] -> CTRL+A and executes all keyboard events
	// the list [29, 30] converts to keyboard events:
	// press(29), press(30), relese(30), relese(29)

	if len(keys) == 0 {
		return false
	}

	// send all keys in press state
	for _, key := range keys {
		keyboard.PressKey(key)
	}

	// send all keys in reverse order and released state
	for i := len(keys) - 1; i >= 0; i-- {
		key := keys[i]
		keyboard.ReleaseKey(key)
	}

	return true
}

func (pressedKeys KeyCodeList) StartsWith(layer KeyCodeList) bool {
	for j, key := range layer {
		if pressedKeys[j] != key {
			return false
		}
	}
	return true
}

func (pressedKeys KeyCodeList) Contains(key KeyCode) bool {
	for _, pressedKey := range pressedKeys {
		if pressedKey == key {
			return true
		}
	}
	return false
}

func (pressedKeys KeyCodeList) Delete(key KeyCode) KeyCodeList {
	output := KeyCodeList{}
	for _, pressedKey := range pressedKeys {
		if pressedKey != key {
			output = append(output, pressedKey)
		}
	}
	return output
}
func (pressedKeys KeyCodeList) Equals(keys KeyCodeList) bool {
	if len(pressedKeys) != len(keys) {
		return false
	}
	for i, pressedKey := range pressedKeys {
		if keys[i] != pressedKey {
			return false
		}
	}
	return true
}

func (keyboard Keyboard) GetPressKeys(key_event KeyEvent) KeyCodeList {
	pressedKeys := keyboard.PressedKeys
	if key_event.KeyState == pressed {
		if !keyboard.PressedKeys.Contains(key_event.KeyCode) {
			pressedKeys = append(keyboard.PressedKeys, key_event.KeyCode)
		}

	} else if key_event.KeyState == released {
		pressedKeys = keyboard.PressedKeys.Delete(key_event.KeyCode)

	}
	return pressedKeys
}
