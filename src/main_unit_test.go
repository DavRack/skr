package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func _TestTable_get_keyboard_path_from_name(t *testing.T) {
	_, out := get_keyboard_path_from_name("Asus Keyboard")
	assert.Equal(t, "/dev/input/event5", out)

	err, out := get_keyboard_path_from_name("none")
	assert.Error(t, err)
}

func Test_startsWith(t *testing.T) {
	list1 := KeyCodeList{1, 2, 3, 4}
	list2 := KeyCodeList{1, 2, 3, 4}
	assert.Equal(t, true, list1.startsWith(list2))

	list1 = KeyCodeList{1, 2, 3, 4}
	list2 = KeyCodeList{1, 2}
	assert.Equal(t, true, list1.startsWith(list2))

	list1 = KeyCodeList{1, 5, 3, 4}
	list2 = KeyCodeList{1, 2}
	assert.Equal(t, false, list1.startsWith(list2))

	list1 = KeyCodeList{1}
	list2 = KeyCodeList{1}
	assert.Equal(t, true, list1.startsWith(list2))
}

func Test_isActiveEmptyLayer(t *testing.T) {
	keyboard := Keyboard{}
	baselayer := keyboard.newLayer("baselayer")
	keyboard.pressedKeys = KeyCodeList{15}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(baselayer),
	)
}

func Test_isActiveLayer(t *testing.T) {
	keyboard := Keyboard{}

	l1 := keyboard.newLayer("l1", 42, 54)
	l2 := keyboard.newLayer("l2", 57)
	l3 := keyboard.newLayer("l3", 57, 42, 54)
	l4 := keyboard.newLayer("l4", 57, 42)

	keyboard.pressedKeys = KeyCodeList{57}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(l2),
	)

	keyboard.pressedKeys = KeyCodeList{57, 79}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(l2),
	)

	keyboard.pressedKeys = KeyCodeList{42, 54}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(l1),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 54}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(l3),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 25}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(l4),
	)

	keyboard.pressedKeys = KeyCodeList{40, 54}
	assert.Equal(t,
		false,
		keyboard.isActiveLayer(l2),
	)
}

func Test_getActiveLayer(t *testing.T) {
	keyboard := Keyboard{}
	l1 := keyboard.newLayer("l1", 42, 54)
	l2 := keyboard.newLayer("l2", 57)
	l3 := keyboard.newLayer("l3", 57, 42, 54)
	l4 := keyboard.newLayer("l4", 57, 42)
	keyboard.pressedKeys = KeyCodeList{42, 54}
	assert.Equal(t,
		l1,
		keyboard.getActiveLayer(),
	)

	keyboard.pressedKeys = KeyCodeList{40, 54}
	assert.Equal(t,
		Layer{},
		keyboard.getActiveLayer(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 5}
	assert.Equal(t,
		l4,
		keyboard.getActiveLayer(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 54, 10}
	assert.Equal(t,
		l3,
		keyboard.getActiveLayer(),
	)
	keyboard.pressedKeys = KeyCodeList{57, 10}
	assert.Equal(t,
		l2,
		keyboard.getActiveLayer(),
	)
}

func Test_blockLayerKeys(t *testing.T) {
	keyboard := Keyboard{}
	keyboard.layers = Layers{
		{KeyCodeList{42, 54}, "l1", false},
		{KeyCodeList{57, 42, 54}, "l3", false},
		{KeyCodeList{57, 49, 54}, "l31", false},
		{KeyCodeList{57, 42}, "l4", false},
		{KeyCodeList{57}, "l2", false},
	}
	keyboard.pressedKeys = KeyCodeList{57}
	assert.Equal(t,
		true,
		keyboard.blockLayerKeys(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 49}
	assert.Equal(t,
		true,
		keyboard.blockLayerKeys(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 50}
	assert.Equal(t,
		false,
		keyboard.blockLayerKeys(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 90}
	assert.Equal(t,
		false,
		keyboard.blockLayerKeys(),
	)
}

func Test_getKeyCode(t *testing.T) {
	var key KeyName = "A"
	assert.Equal(t, 30, int(key.keyCode()))

	key = "ctrl"
	assert.Equal(t, 29, int(key.keyCode()))
	key = "CTRL"
	assert.Equal(t, 29, int(key.keyCode()))
}

func Test_getKeyName(t *testing.T) {
	var key KeyCode = 30
	assert.Equal(t, "A", string(key.keyName()))

	key = 29
	assert.Equal(t, "CTRL", string(key.keyName()))
}

func Test_keyIs(t *testing.T) {
	var key = KeyEvent{}
	key.keyCode = 30
	assert.Equal(t, true, key.is("a"))
	assert.Equal(t, true, key.is("A"))
	assert.Equal(t, true, key.is(30))
	assert.Equal(t, false, key.is("B"))
}

func Test_createLayer(t *testing.T) {
	keyboard := Keyboard{}
	layer := Layer{}
	layer.name = "baselayer"
	layer.fnKeys = KeyCodeList{}
	assert.Equal(t, layer, keyboard.newLayer("baselayer"))

	layer.fnKeys = KeyCodeList{29, 30}
	layer.name = "t"
	assert.Equal(t, layer, keyboard.newLayer("t", 29, 30))

	layer.fnKeys = KeyCodeList{29, 30}
	layer.name = "t2"
	assert.Equal(t, layer, keyboard.newLayer("t2", "CTRL", "A"))
}

func Test_interfaceToKeycode(t *testing.T) {
	code, _ := interfaceToKeyCode(10)
	assert.Equal(t, KeyCode(10), code)

	code, _ = interfaceToKeyCode("a")
	assert.Equal(t, KeyCode(30), code)
}

func Test_interfacesToKeycodes(t *testing.T) {
	keyCodes, _ := interfacesToKeyCodes([]interface{}{KeyCodeList{29, 30}})
	assert.Equal(t, KeyCodeList{29, 30}, keyCodes)

	keyCodes, _ = interfacesToKeyCodes([]interface{}{KeyCodeList{57}})
	assert.Equal(t, KeyCodeList{57}, keyCodes)
}
