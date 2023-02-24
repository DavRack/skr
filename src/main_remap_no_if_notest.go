package main

import (
	"skr/keyboard"
	"testing"

	"github.com/stretchr/testify/assert"
)

func remap_without_if(keyboard *keyboard.Keyboard) (blockCurrentKey bool) {
	keyboard.KeyRemap(30, 32)

	if keyboard.SomeActionExecuted == false {
		currentKeyEvent := keyboard.LastKey
		keyboard.Execute(currentKeyEvent)
	}
	return
}

func Test_remap_no_check_for_execution_sucsess_passing_remap_and_no_remap(t *testing.T) {
	inputEvents := []keyboard.KeyboardEvent{
		mock_keyEvent(30, keyboard.KeyPressed),
		mock_keyEvent(30, keyboard.KeyReleased),
		mock_keyEvent(20, keyboard.KeyPressed),
		mock_keyEvent(20, keyboard.KeyReleased),
	}

	espectedOutput := []keyboard.KeyboardEvent{
		mock_keyEvent(32, keyboard.KeyPressed),
		mock_keyEvent(32, keyboard.KeyReleased),
		mock_keyEvent(20, keyboard.KeyPressed),
		mock_keyEvent(20, keyboard.KeyReleased),
	}

	keyboard := runTest(inputEvents, remap_without_if)

	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}
