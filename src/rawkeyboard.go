package main

import (
	"bufio"
	"os/exec"
)

func keyboardReader(keyboardPath string) *bufio.Reader {
	// create a process to read raw input data from interception tools
	readCmd := exec.Command("sudo", "intercept", keyboardPath)
	readPipe, _ := readCmd.StdoutPipe()
	readCmd.Start()
	return bufio.NewReader(readPipe)
}
