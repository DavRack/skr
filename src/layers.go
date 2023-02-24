package main

// import (
// 	"reflect"
// 	"sort"
// )

// type Layer struct {
// 	fnKeys         KeyCodeList
// 	name           string
// 	actionExecuted bool
// }

// type Layers []Layer

// func (layer Layer) len() int {
// 	return len(layer.fnKeys)
// }

// func (layers Layers) Len() int {
// 	return len(layers)
// }
// func (layers Layers) Less(i, j int) bool {
// 	return layers[i].len() < layers[j].len()
// }
// func (layers Layers) Swap(i, j int) {
// 	s1 := layers[i]
// 	s2 := layers[j]

// 	layers[j] = s1
// 	layers[i] = s2
// }

// func (keyboard Keyboard) getActiveLayer() (activeLayer Layer) {
// 	for _, layer := range keyboard.layers {
// 		if keyboard.isActiveLayer(layer) {
// 			return layer
// 		}
// 	}
// 	return Layer{}
// }

// func (keyboard Keyboard) isActiveLayer(layer Layer) (isActive bool) {
// 	sort.Sort(keyboard.layers)
// 	for i := len(keyboard.layers) - 1; i >= 0; i-- {
// 		if len(keyboard.pressedKeys) < keyboard.layers[i].len() {
// 			continue
// 		}
// 		if keyboard.pressedKeys.startsWith(keyboard.layers[i].fnKeys) {
// 			if reflect.DeepEqual(keyboard.layers[i], layer) {
// 				return true
// 			}
// 			return false
// 		}
// 	}
// 	return false
// }

// func (keyboard *Keyboard) newLayer(name string, keys ...interface{}) Layer {
// 	hotKeys, ok := interfacesToKeyCodes(keys)
// 	if !ok || len(name) == 0 {
// 		return Layer{}
// 	}
// 	newLayer := Layer{
// 		name:   name,
// 		fnKeys: KeyCodeList{},
// 	}
// 	if len(hotKeys) == 0 {
// 		keyboard.layers = append(keyboard.layers, newLayer)
// 		return newLayer
// 	}
// 	for _, layer := range keyboard.layers {
// 		if reflect.DeepEqual(layer, hotKeys) {
// 			return layer
// 		}
// 	}
// 	newLayer.fnKeys = hotKeys
// 	keyboard.layers = append(keyboard.layers, newLayer)
// 	return newLayer
// }

// func (keyboard *Keyboard) blockLayerKeys() (keyBlocked bool) {
// 	for _, layer := range keyboard.layers {
// 		if layer.len() < len(keyboard.pressedKeys) {
// 			continue
// 		}

// 		if layer.fnKeys.startsWith(keyboard.pressedKeys) {
// 			keyboard.executeDefaulAction = false
// 			return true
// 		}
// 	}

// 	return
// }
