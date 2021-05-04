package main

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

// func get_raw_events(decided_action action, raw_input InputEvent) ([]raw_key_action, []raw_script_action) {

// 	if reflect.DeepEqual(decided_action, action{}) {
// 		// if theres no selec
// 		return []raw_key_action{}, []raw_script_action{}

// 	} else if !reflect.DeepEqual(decided_action.remap_action, remap{}) {
// 		// if action is a remap action
// 		raw_key_actions := []raw_key_action{}

// 		var raw_action raw_key_action

// 		if len(decided_action.remap_action.action) > 1 {

// 		} else {
// 			raw_action = raw_key_action{
// 				decided_action.remap_action.action[0],
// 				raw_input.Value,
// 				0,
// 				0,
// 			}
// 			raw_key_actions = append(raw_key_actions, raw_action)

// 		}

// 		return raw_key_actions, []raw_script_action{}

// 	} else {
// 		// get raw_script_actions
// 		raw_script_actions := []raw_script_action{}

// 		var raw_action raw_script_action

// 		raw_action = raw_script_action{
// 			decided_action.script_action.action,
// 			0,
// 			0,
// 		}
// 		raw_script_actions = append(raw_script_actions, raw_action)
// 		return []raw_key_action{}, raw_script_actions
// 	}
// }
