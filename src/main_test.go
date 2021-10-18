package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestTable_get_keyboard_path_from_name(t *testing.T) {
	_, out := get_keyboard_path_from_name("ASUSTeK Computer Inc. N-KEY Device")
	assert.Equal(t, "/dev/input/event9", out)

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

func Test_isActiveLayer(t *testing.T) {
	keyboard := Keyboard{}
	keyboard.layers = []KeyCodeList{
		{42, 54},
		{57},
		{57, 42, 54},
		{57, 42},
	}

	keyboard.pressedKeys = KeyCodeList{57}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(KeyCodeList{57}),
	)

	keyboard.pressedKeys = KeyCodeList{57, 79}
	assert.Equal(t,
		true,
		keyboard.isActiveLayer(KeyCodeList{57}),
	)

	keyboard.pressedKeys = KeyCodeList{42, 54}
	assert.Equal(t,
		false,
		keyboard.isActiveLayer(KeyCodeList{57}),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 54}
	assert.Equal(t,
		false,
		keyboard.isActiveLayer(KeyCodeList{57, 42}),
	)

	keyboard.pressedKeys = KeyCodeList{42, 54}
	assert.Equal(t,
		false,
		keyboard.isActiveLayer(KeyCodeList{57, 42, 54}),
	)

	keyboard.pressedKeys = KeyCodeList{42, 54}
	assert.Equal(t,
		false,
		keyboard.isActiveLayer(KeyCodeList{95, 2, 54}),
	)
}

func Test_getActiveLayer(t *testing.T) {
	keyboard := Keyboard{}
	keyboard.layers = []KeyCodeList{
		{42, 54},
		{57},
		{57, 42, 54},
		{57, 42},
	}
	keyboard.pressedKeys = KeyCodeList{42, 54}
	assert.Equal(t,
		KeyCodeList{42, 54},
		keyboard.getActiveLayer(),
	)

	keyboard.pressedKeys = KeyCodeList{40, 54}
	assert.Equal(t,
		KeyCodeList{},
		keyboard.getActiveLayer(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 5}
	assert.Equal(t,
		KeyCodeList{57, 42},
		keyboard.getActiveLayer(),
	)

	keyboard.pressedKeys = KeyCodeList{57, 42, 54, 10}
	assert.Equal(t,
		KeyCodeList{57, 42, 54},
		keyboard.getActiveLayer(),
	)
}

func Test_blockLayerKeys(t *testing.T) {
	keyboard := Keyboard{}
	keyboard.layers = []KeyCodeList{
		{42, 54},
		{57, 42, 54},
		{57, 49, 54},
		{57, 42},
		{57},
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
