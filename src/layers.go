package main

import (
	"reflect"
	"sort"
)

func (layers Layers) Len() int {
	return len(layers)
}
func (layers Layers) Less(i, j int) bool {
	return len(layers[i]) < len(layers[j])
}
func (layers Layers) Swap(i, j int) {
	s1 := layers[i]
	s2 := layers[j]

	layers[j] = s1
	layers[i] = s2
}

func (keyboard Keyboard) getActiveLayer() (isActiveLayer KeyCodeList) {
	for _, layer := range keyboard.layers {
		if keyboard.isActiveLayer(layer) {
			return layer
		}
	}
	return KeyCodeList{}
}

func (keyboard Keyboard) isActiveLayer(hotKeys KeyCodeList) (isActive bool) {
	sort.Sort(keyboard.layers)
	for i := len(keyboard.layers) - 1; i >= 0; i-- {
		if len(keyboard.pressedKeys) < len(keyboard.layers[i]) {
			continue
		}
		if keyboard.pressedKeys.startsWith(keyboard.layers[i]) {
			if reflect.DeepEqual(hotKeys, keyboard.layers[i]) {
				return true
			}
			return false
		}
	}
	return false
}

func (keyboard *Keyboard) createLayer(hotKeys ...KeyCode) KeyCodeList {
	for _, layer := range keyboard.layers {
		if reflect.DeepEqual(layer, hotKeys) {
			return hotKeys
		}
	}
	keyboard.layers = append(keyboard.layers, hotKeys)
	return hotKeys
}

func (keyboard *Keyboard) blockLayerKeys() (keyBlocked bool) {

	for _, layer := range keyboard.layers {
		if len(layer) < len(keyboard.pressedKeys) {
			continue
		}

		if layer.startsWith(keyboard.pressedKeys) {
			keyboard.executeDefaulAction = false
			return true
		}
	}

	return
}
