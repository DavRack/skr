package main

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
	"os/exec"
	"strings"
	"syscall"
	"time"
)

type KeyboardIO interface {
	read() (KeyboardEvent, error)
	write(KeyboardEvent) error
	flush()
}

type keyboardStdinIO struct {
	ioReader *bufio.Reader
	ioWriter *bufio.Writer
}

type keyboardEventIO struct {
	inputKeyboardEvents  []KeyboardEvent
	outputKeyboardEvents []KeyboardEvent
}

var prev_evt_wrap KeyboardEvent = KeyboardEvent{syscall.Timeval{}, 4, 4, 4}
var post_evt_wrap KeyboardEvent = KeyboardEvent{syscall.Timeval{}, 0, 0, 0}

func (keyboard *Keyboard) pressKey(key interface{}) (ok bool) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: pressed,
	}
	keyboard.execute(keyEvent)
	ok = true
	return
}

func (keyboard *Keyboard) releaseKey(key interface{}) {
	keyCode, ok := interfaceToKeyCode(key)
	if !ok {
		return
	}
	keyEvent := KeyEvent{
		keyCode:  keyCode,
		keyState: released,
	}
	keyboard.execute(keyEvent)
}

func (keyboard *Keyboard) execute(keyEvent KeyEvent) {
	inputEvent := keyEvent.toInputEvent()
	// state := ""
	// if keyEvent.keyState == pressed
	// 	state = "keyPressed"
	// }
	// if keyEvent.keyState == released {
	// 	state = "keyReleased"
	// }
	// if keyEvent.keyState == held {
	// 	state = "keyHeld"
	// }

	// fmt.Println("KeyCode:", keyEvent.keyCode, "Event type:", state)
	// var bytes bytes.Buffer
	// enc := gob.NewEncoder(&bytes)

	// fmt.Println("New Write")

	// enc.Encode(pre_evt_wrap)
	// keyboard.ioWriter.Write(bytes.Bytes())

	// enc.Encode(inputEvent)
	// keyboard.ioWriter.Write(bytes.Bytes())

	// enc.Encode(post_evt_wrap)
	// keyboard.ioWriter.Write(bytes.Bytes())
	t := time.Now()
	sec := t.Unix()
	usec := int64(t.Nanosecond())

	inputEvent.Time.Sec = sec
	inputEvent.Time.Usec = usec

	err := keyboard.IO.write(prev_evt_wrap)
	if err != nil {
		fmt.Println(err)
	}
	err = keyboard.IO.write(inputEvent)
	if err != nil {
		fmt.Println(err)
	}
	err = keyboard.IO.write(post_evt_wrap)
	if err != nil {
		fmt.Println(err)
	}
	keyboard.IO.flush()
}

func (keyEvent KeyEvent) toInputEvent() (inputEvent KeyboardEvent) {
	inputEvent = KeyboardEvent{
		Time:  keyEvent.time,
		Type:  key_event,
		Code:  keyEvent.keyCode,
		Value: keyEvent.keyState,
	}
	return
}

func get_keyboard_path_from_name(name string) (error, string) {

	command := "sudo sed -n '/" + name + "/,/Handlers/p' /proc/bus/input/devices | grep -o 'event.*' -m 1"

	out, _ := exec.Command("bash", "-c", command).Output()
	if len(out) == 0 {
		return errors.New("Keyboard not found"), ""
	}
	input_event := strings.TrimSpace(string(out))

	return nil, "/dev/input/" + input_event

}

func (keyboard keyboardStdinIO) read() (KeyboardEvent, error) {
	var raw_input KeyboardEvent
	binary.Read(keyboard.ioReader, binary.LittleEndian, &raw_input)
	return raw_input, nil
}

func (keyboard keyboardStdinIO) write(outputKeyEvent KeyboardEvent) error {
	err := binary.Write(keyboard.ioWriter, binary.LittleEndian, &outputKeyEvent)
	return err
}

func (keyboard keyboardStdinIO) flush() {
	keyboard.ioWriter.Flush()
}

func initKeyboardStdinIO(keyboardName string) keyboardStdinIO {
	var keyboard keyboardStdinIO
	// create a process to read raw input data from interception tools
	_, keyboardPath := get_keyboard_path_from_name(keyboardName)

	write_cmd := exec.Command("uinput", "-d", keyboardPath)
	write_pipe, _ := write_cmd.StdinPipe()
	write_cmd.Start()
	defer write_cmd.Wait()
	ioWriter := bufio.NewWriter(write_pipe)

	read_cmd := exec.Command("intercept", "-g", keyboardPath)
	read_pipe, _ := read_cmd.StdoutPipe()
	read_cmd.Start()
	defer read_cmd.Wait()
	ioReader := bufio.NewReader(read_pipe)

	keyboard.ioReader = ioReader
	keyboard.ioWriter = ioWriter

	return keyboard
}

func (keyboard keyboardEventIO) read() (KeyboardEvent, error) {
	if len(keyboard.inputKeyboardEvents) == 0 {
		return KeyboardEvent{}, errors.New("No more input events")
	}
	raw_event := keyboard.inputKeyboardEvents[0]
	keyboard.inputKeyboardEvents = keyboard.inputKeyboardEvents[1:]
	fmt.Print(keyboard.inputKeyboardEvents)
	return raw_event, nil
}
func (keyboard keyboardEventIO) write(outputKeyEvent KeyboardEvent) error {
	if outputKeyEvent.Type == key_event {
		outputEvents := keyboard.outputKeyboardEvents
		keyboard.outputKeyboardEvents = append(outputEvents, outputKeyEvent)
	}
	return nil
}
func (keyboard keyboardEventIO) flush() {
}

func initKeyboardEventIO() keyboardEventIO {
	var keyboard keyboardEventIO
	return keyboard
}
