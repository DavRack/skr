package main

func decide_actions(pressKeys []uint16, layers []layer) action {
	// evaluate each layer to find remaps
	L := get_layer(pressKeys, layers)
	actions := L.actions

	for _, a := range actions {
		r := a.remap_action
		s := a.script_action

		if match_keypress_subset(pressKeys, s.trigger) {
			return a
		} else if match_keypress_subset(pressKeys, r.trigger) {
			return a
		}
	}

	// return an empty/default action in case of no match
	return action{}
}
