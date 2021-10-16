package main

import (
	"os/exec"
)

func parse() []layer {
	var layers []layer
	layers = append(layers, layer{
		0,
		action{},
		[]action{
			{script{}, remap{[]uint16{30}, []uint16{29}, false, true}},
		},
	})
	return layers
}

func get_keyboard_path_from_name(name string) string {

	command := "cat /proc/bus/input/devices | awk '$1==\"N:\" || $1==\"H:\" {print $0}' | grep \"" + name + "\" -A1 | tail -1 | tr \" \" \"\n\" | grep event"

	out, _ := exec.Command("bash", "-c", command).Output()
	input_event := string(out)[:len(out)-1]

	return "/dev/input/" + input_event

}
