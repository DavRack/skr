package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"syscall"
	"testing"

	"github.com/stretchr/testify/assert"
)

func mock_keyEvent(code int, state KeyState) KeyboardEvent {
	return KeyboardEvent{syscall.Timeval{}, key_event, KeyCode(code), state}
}

func (event KeyboardEvent) toString() string {
	code := event.Code
	state := ""
	if event.Value == keyPressed {
		state = "Key pressed"
	}
	if event.Value == keyReleased {
		state = "Key released"
	}
	resoult := "Code: " + fmt.Sprint(code) + ", State: " + state
	return resoult
}

func keyboardEventsToSimpleList(espectedEvents []KeyboardEvent) []string {
	simpleEvents := []string{}
	for _, event := range espectedEvents {
		simpleEvents = append(simpleEvents, event.toString())
	}
	return simpleEvents
}
func keyboardInToSimpleList(keyboard Keyboard) []string {
	simpleEvents := []string{}
	for {
		event, err := keyboard.IO.read()
		if err != nil {
			break
		}
		if event.Type != key_event {
			continue
		}
		simpleEvents = append(simpleEvents, event.toString())
	}
	return simpleEvents
}

func setup(inputEvents []KeyboardEvent) Keyboard {

	keyboardIn, keyboardOut, _ := os.Pipe()
	keyboardStdin := KeyboardIO{}
	keyboardStdin.ioReader = bufio.NewReader(keyboardIn)
	keyboardStdin.ioWriter = bufio.NewWriter(keyboardOut)

	keyboard := initConfig()
	keyboard.path = "/dev/null"
	KeyboardIO := keyboardStdin
	keyboard.IO = KeyboardIO
	for _, inputEvent := range inputEvents {
		binary.Write(KeyboardIO.ioWriter, binary.LittleEndian, &inputEvent)
	}
	nonKeyboardEvent := KeyboardEvent{}
	nonKeyboardEvent.Type = 6
	binary.Write(KeyboardIO.ioWriter, binary.LittleEndian, &nonKeyboardEvent)
	KeyboardIO.flush()
	return keyboard
}

func setdown(keyboard Keyboard) {
	nonKeyboardEvent := KeyboardEvent{}
	nonKeyboardEvent.Type = 6
	keyboard.IO.write(nonKeyboardEvent)
	keyboard.IO.flush()
}

func transparent_config(keyboard *Keyboard) (blockCurrentKey bool) {
	currentKeyEvent := keyboard.lastKey
	keyboard.execute(currentKeyEvent)
	return
}

func Test_no_custom_actions(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}
	keyboard := setup(inputEvents)

	loop(&keyboard, transparent_config)
	setdown(keyboard)

	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}

func basic_remap(keyboard *Keyboard) (blockCurrentKey bool) {
	if keyboard.lastKey.is("A") {
		currentKeyEvent := keyboard.lastKey
		currentKeyEvent.keyCode = 31
		keyboard.execute(currentKeyEvent)
		return
	}

	if keyboard.keyRemap(31, 32) {
		return
	}
	currentKeyEvent := keyboard.lastKey
	keyboard.execute(currentKeyEvent)
	return
}

func Test_basic_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
		mock_keyEvent(31, keyPressed),
		mock_keyEvent(31, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(31, keyPressed),
		mock_keyEvent(31, keyReleased),
		mock_keyEvent(32, keyPressed),
		mock_keyEvent(32, keyReleased),
	}
	keyboard := setup(inputEvents)
	loop(&keyboard, basic_remap)
	setdown(keyboard)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))

}

func remap_without_if(keyboard *Keyboard) (blockCurrentKey bool) {
	keyboard.keyRemap(30, 32)

	if keyboard.someActionExecuted == false {
		currentKeyEvent := keyboard.lastKey
		keyboard.execute(currentKeyEvent)
	}
	return
}

func Test_remap_no_check_for_execution_sucsess(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(31, keyPressed),
		mock_keyEvent(31, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(31, keyPressed),
		mock_keyEvent(31, keyReleased),
	}
	keyboard := setup(inputEvents)

	loop(&keyboard, remap_without_if)
	setdown(keyboard)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}

func Test_remap_no_check_for_execution_sucsess_faling_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}
	keyboard := setup(inputEvents)

	loop(&keyboard, remap_without_if)
	setdown(keyboard)
	assert.NotEqual(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}
func Test_remap_no_check_for_execution_sucsess_passing_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(32, keyPressed),
		mock_keyEvent(32, keyReleased),
	}
	keyboard := setup(inputEvents)

	loop(&keyboard, remap_without_if)
	setdown(keyboard)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}

func Test_remap_no_check_for_execution_sucsess_passing_remap_and_no_remap(t *testing.T) {
	inputEvents := []KeyboardEvent{
		mock_keyEvent(30, keyPressed),
		mock_keyEvent(30, keyReleased),
		mock_keyEvent(20, keyPressed),
		mock_keyEvent(20, keyReleased),
	}

	espectedOutput := []KeyboardEvent{
		mock_keyEvent(32, keyPressed),
		mock_keyEvent(32, keyReleased),
		mock_keyEvent(20, keyPressed),
		mock_keyEvent(20, keyReleased),
	}
	keyboard := setup(inputEvents)

	loop(&keyboard, remap_without_if)
	setdown(keyboard)
	assert.Equal(t, keyboardEventsToSimpleList(espectedOutput), keyboardInToSimpleList(keyboard))
}
