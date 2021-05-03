package main

import (
	"os"
	"os/exec"
	"reflect"
)

func keyboard_exist(keyboard_path string) bool {
	_, err := os.Stat(keyboard_path)
	return err == nil
}

func list_uint16_contains(list []uint16, value uint16) bool {
	for i := 0; i < len(list); i++ {
		if list[i] == value {
			return true
		}
	}
	return false
}

func get_press_keys(key_event InputEvent, pressKeys []uint16) []uint16 {
	if key_event.Value == keyPressed {
		if !list_uint16_contains(pressKeys, key_event.Code) {
			pressKeys = append(pressKeys, key_event.Code)
		}

	} else if key_event.Value == keyReleased {
		pressKeys = delete_uint16(pressKeys, key_event.Code)

	}
	return pressKeys
}

func get_layer(pressKeys []uint16, layers []layer) layer {
	if len(layers) == 0 {
		return layer{}
	}
	if len(pressKeys) > 0 {
		// get the firs pressed key
		first_pressed_key := pressKeys[0]

		for _, l := range layers {
			if first_pressed_key != 0 && first_pressed_key == l.layer_key {
				return l
			}
		}
	}
	return layers[0]
}

// match_keypress_subset returns true if "subset" is a subset of "list"
func match_keypress_subset(list []uint16, subset []uint16) bool {
	if len(list) == 0 || len(subset) == 0 {
		return false
	}

	if len(list) < len(subset) {
		return false
	}

	con := len(list) - 1
	for i := len(subset) - 1; i > -1; i-- {
		if subset[i] != list[con] {
			return false
		}
		con--
	}
	return true
}

func get_raw_events(decided_action action, raw_input InputEvent) ([]raw_key_action, []raw_script_action) {

	if reflect.DeepEqual(decided_action, action{}) {
		// if theres no selec
		return []raw_key_action{}, []raw_script_action{}

	} else if !reflect.DeepEqual(decided_action.remap_action, remap{}) {
		// if action is a remap action
		raw_key_actions := []raw_key_action{}

		var raw_action raw_key_action

		if len(decided_action.remap_action.action) > 1 {

		} else {
			raw_action = raw_key_action{
				decided_action.remap_action.action[0],
				raw_input.Value,
				0,
				0,
			}
			raw_key_actions = append(raw_key_actions, raw_action)

		}

		return raw_key_actions, []raw_script_action{}

	} else {
		// get raw_script_actions
		raw_script_actions := []raw_script_action{}

		var raw_action raw_script_action

		raw_action = raw_script_action{
			decided_action.script_action.action,
			0,
			0,
		}
		raw_script_actions = append(raw_script_actions, raw_action)
		return []raw_key_action{}, raw_script_actions
	}
}

func get_keyboard_path_from_name(name string) string {
	command := "cat /proc/bus/input/devices | awk '$1==\"N:\" || $1==\"H:\" {print $0}' | grep \"" + name + "\" -A1 | tail -1 | tr \" \" \"\n\" | grep event"

	out, _ := exec.Command("bash", "-c", command).Output()
	input_event := string(out)[:len(out)-1]

	return "/dev/input/" + input_event

}
