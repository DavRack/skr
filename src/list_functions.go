package main

func remove_layer_at(list []layer, index int) []layer {
	return append(list[:index], list[index+1:]...)
}

// remove_uint16_at deletes element at index i
func (pressedKeys KeyCodeList) remove_at(index int) KeyCodeList {
	if index >= 0 && index < len(pressedKeys) {
		return append(pressedKeys[:index], pressedKeys[index+1:]...)
	}
	return pressedKeys
}

func (pressedKeys KeyCodeList) contains(value KeyCode) bool {
	for i := 0; i < len(pressedKeys); i++ {
		if pressedKeys[i] == value {
			return true
		}
	}
	return false
}

// delete_uint16 deletes the first instance of value in list
// and returns a new list with those values
func (pressedKeys KeyCodeList) delete(key KeyCode) KeyCodeList {
	for i := 0; i < len(pressedKeys); i++ {
		if pressedKeys[i] == key {
			return pressedKeys.remove_at(i)
		}
	}
	return pressedKeys
}
