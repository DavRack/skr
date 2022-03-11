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

type KeyboardIO struct {
	ioReader *bufio.Reader
	ioWriter *bufio.Writer
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

func (keyboard KeyboardIO) read() (KeyboardEvent, error) {
	var raw_input KeyboardEvent
	err := binary.Read(keyboard.ioReader, binary.LittleEndian, &raw_input)
	if raw_input.Type == 6 {
		err = errors.New("Invalid Keyboard event")
	}
	return raw_input, err
}

func (keyboard KeyboardIO) write(outputKeyEvent KeyboardEvent) error {
	err := binary.Write(keyboard.ioWriter, binary.LittleEndian, &outputKeyEvent)
	return err
}

func (keyboard KeyboardIO) flush() {
	keyboard.ioWriter.Flush()
}

func initKeyboardStdinIO(keyboardName string) KeyboardIO {
	var keyboard KeyboardIO
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
