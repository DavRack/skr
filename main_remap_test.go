package main

import (
	. "skr/keyboard"
	"testing"

	"github.com/stretchr/testify/assert"
)

func many_to_one_remap(kb *Keyboard) (blockCurrentKey bool) {
	kb.KeyRemap("L_SHIFT + R_SHIFT", "TAB")

	if kb.SomeActionExecuted == false {
		currentKeyEvent := kb.LastKey
		kb.Execute(currentKeyEvent)
	}
	return
}

func Test_many_to_one_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(42, KeyPressed),
		mock_keyEvent(30, KeyPressed),
		mock_keyEvent(30, KeyReleased),
		mock_keyEvent(42, KeyReleased),

		mock_keyEvent(42, KeyPressed),
		mock_keyEvent(42, KeyReleased),

		mock_keyEvent(42, KeyPressed),
		mock_keyEvent(54, KeyPressed),
		mock_keyEvent(42, KeyReleased),
		mock_keyEvent(54, KeyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(42, KeyPressed),
		mock_keyEvent(30, KeyPressed),
		mock_keyEvent(30, KeyReleased),
		mock_keyEvent(42, KeyReleased),

		mock_keyEvent(42, KeyPressed),
		mock_keyEvent(42, KeyReleased),

		mock_keyEvent(15, KeyPressed),
		mock_keyEvent(15, KeyReleased),
	}
	keyboard := runTest(inputEvents, many_to_one_remap)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))

}

func one_to_many_remap(kb *Keyboard) (blockCurrentKey bool) {
	if kb.LastKey.Is("A") {
		kb.ExecuteDefaulAction = false
		if kb.LastKey.KeyState == KeyPressed {
			kb.PressKey(29)
			kb.PressKey(20)
			kb.ReleaseKey(20)
			kb.ReleaseKey(29)
		}
	}

	if kb.ExecuteDefaulAction {
		currentKeyEvent := kb.LastKey
		kb.Execute(currentKeyEvent)
	}
	return
}

func Test_one_to_many_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, KeyPressed),
		mock_keyEvent(30, KeyReleased),
		mock_keyEvent(31, KeyPressed),
		mock_keyEvent(31, KeyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(29, KeyPressed),
		mock_keyEvent(20, KeyPressed),
		mock_keyEvent(20, KeyReleased),
		mock_keyEvent(29, KeyReleased),
		mock_keyEvent(31, KeyPressed),
		mock_keyEvent(31, KeyReleased),
	}
	keyboard := runTest(inputEvents, one_to_many_remap)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))

}

func basic_remap(kb *Keyboard) (blockCurrentKey bool) {
	if kb.LastKey.Is("A") {
		currentKeyEvent := kb.LastKey
		currentKeyEvent.KeyCode = 31
		kb.Execute(currentKeyEvent)
		return
	}
	if kb.LastKey.Is(KeyCode(32)) {
		currentKeyEvent := kb.LastKey
		currentKeyEvent.KeyCode = 33
		kb.Execute(currentKeyEvent)
		return
	}

	kb.KeyRemap(31, 32)

	if kb.SomeActionExecuted == false {
		currentKeyEvent := kb.LastKey
		kb.Execute(currentKeyEvent)
	}
	return
}

func Test_basic_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, KeyPressed),
		mock_keyEvent(30, KeyReleased),
		mock_keyEvent(31, KeyPressed),
		mock_keyEvent(31, KeyReleased),
		mock_keyEvent(32, KeyPressed),
		mock_keyEvent(32, KeyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(31, KeyPressed),
		mock_keyEvent(31, KeyReleased),
		mock_keyEvent(32, KeyPressed),
		mock_keyEvent(32, KeyReleased),
		mock_keyEvent(33, KeyPressed),
		mock_keyEvent(33, KeyReleased),
	}
	keyboard := runTest(inputEvents, basic_remap)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))

}
