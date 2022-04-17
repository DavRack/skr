package keyboard

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_getKeyCode(t *testing.T) {
	var key KeyName = "A"
	assert.Equal(t, 30, int(key.KeyCode()))

	key = "ctrl"
	assert.Equal(t, 29, int(key.KeyCode()))

	key = "CTRL"
	assert.Equal(t, 29, int(key.KeyCode()))
}

func Test_getKeyName(t *testing.T) {
	var key KeyCode = 30
	assert.Equal(t, "A", string(key.KeyName()))

	key = 29
	assert.Equal(t, "CTRL", string(key.KeyName()))

	key = 9999 // some key that doesnt exist
	assert.Equal(t, "", string(key.KeyName()))
}

func Test_delete(t *testing.T) {
	pressedKeys := KeyCodeList{1, 2, 3, 4, 5}
	pressedKeys = pressedKeys.Delete(3)

	assert.Equal(t, pressedKeys, KeyCodeList{1, 2, 4, 5})
}

func Test_equals(t *testing.T) {
	kb := Keyboard{}
	kb.PressedKeys = KeyCodeList{1, 2, 3, 4, 5}
	assert.True(t, kb.PressedKeys.Equals(KeyCodeList{1, 2, 3, 4, 5}))

	assert.False(t, kb.PressedKeys.Equals(KeyCodeList{1, 2}))

	assert.False(t, kb.PressedKeys.Equals(KeyCodeList{1, 2, 3, 4, 6}))
}

func Test_stringToKeycodes(t *testing.T) {
	keycodes, _ := stringToKeyCodes("CTRL+A")
	assert.Equal(t, KeyCodeList{29, 30}, keycodes)

	keycodes, _ = stringToKeyCodes("CTRL+Non_Existing_Key_Code")
	assert.Equal(t, KeyCodeList{}, keycodes)
}

func Test_EndsWith(t *testing.T) {
	assert.True(t, KeyCodeList{1, 2, 3}.EndsWith(KeyCodeList{2, 3}))
	assert.False(t, KeyCodeList{1, 2, 3}.EndsWith(KeyCodeList{2}))
	assert.False(t, KeyCodeList{1, 2, 3}.EndsWith(KeyCodeList{}))
	assert.False(t, KeyCodeList{}.EndsWith(KeyCodeList{1, 2, 3}))
}
func Test_EndsWithSubSet(t *testing.T) {
	assert.True(t, KeyCodeList{1, 2, 3}.EndsWithSubset(KeyCodeList{2, 3}))
	assert.True(t, KeyCodeList{1, 2, 3}.EndsWithSubset(KeyCodeList{2, 3, 4}))
	assert.True(t, KeyCodeList{1}.EndsWithSubset(KeyCodeList{1, 2}))
	assert.False(t, KeyCodeList{1, 2, 3}.EndsWithSubset(KeyCodeList{}))
	assert.False(t, KeyCodeList{}.EndsWithSubset(KeyCodeList{1, 2}))
	assert.False(t, KeyCodeList{1, 2, 3, 4}.EndsWithSubset(KeyCodeList{5}))
}

func Test_interfaceToKeycodes(t *testing.T) {
	input := "L_SHIFT + R_SHIFT"
	espectedOutput := KeyCodeList{42, 54}

	realOutput, _ := interfaceToKeyCodes(input)

	assert.Equal(t, espectedOutput, realOutput)
}
