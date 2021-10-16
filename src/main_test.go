package main

import (
	"syscall"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestTable_get_press_keys(t *testing.T) {
	var key_event InputEvent

	key_event = InputEvent{syscall.Timeval{}, keyEvent, 30, keyPressed}

	assert.Equal(t,
		[]uint16{30},
		get_press_keys(key_event, []uint16{}),
	)

	key_event = InputEvent{syscall.Timeval{}, keyEvent, 30, keyReleased}

	assert.Equal(t,
		[]uint16{},
		get_press_keys(key_event, []uint16{30}),
	)

	key_event = InputEvent{syscall.Timeval{}, keyEvent, 30, keyPressed}

	assert.Equal(t,
		[]uint16{1, 2, 3, 4, 5, 6, 30},
		get_press_keys(key_event, []uint16{1, 2, 3, 4, 5, 6}),
	)

	key_event = InputEvent{syscall.Timeval{}, keyEvent, 30, keyPressed}

	assert.Equal(t,
		[]uint16{1, 2, 3, 4, 5, 6, 30},
		get_press_keys(key_event, []uint16{1, 2, 3, 4, 5, 6, 30}),
	)
}

func TestTable_get_layer(t *testing.T) {
	// boiler plate
	var test_layers []layer
	test_layers = append(test_layers, layer{
		0,
		action{},
		[]action{
			{
				script{[]uint16{}, "", false},
				remap{[]uint16{58}, []uint16{29}, false, false},
			},
		},
	})
	test_layers = append(test_layers, layer{
		29,
		action{},
		[]action{
			{
				script{[]uint16{}, "", false},
				remap{[]uint16{58}, []uint16{29}, false, false},
			},
		},
	})

	//testing
	var tests = []struct {
		pressKeys []uint16
		layers    []layer
		expected  layer
	}{
		{[]uint16{}, test_layers, test_layers[0]},
		{[]uint16{}, []layer{}, layer{}},
		{[]uint16{30}, test_layers, test_layers[0]},
		{[]uint16{29}, test_layers, test_layers[1]},
		{[]uint16{29, 30}, test_layers, test_layers[1]},
		{[]uint16{20, 29}, test_layers, test_layers[0]},
	}

	for _, test := range tests {
		output := get_layer(test.pressKeys, test.layers)
		if !(assert.Equal(t, output, test.expected)) {
			t.Error("Fail")
			t.Error("output  ", output)
			t.Error("expected", test.expected)
		}
	}
}

func TestTable_decide_actions(t *testing.T) {
	// boiler plate
	var test_layers []layer
	test_layers = append(test_layers, layer{
		0,
		action{},
		[]action{
			{script{}, remap{[]uint16{58}, []uint16{29}, false, false}},
			{script{[]uint16{1}, "st", true}, remap{}},
			{script{[]uint16{58, 30}, "st", true}, remap{}},
			{script{}, remap{[]uint16{30}, []uint16{29}, false, true}},
		},
	})

	test_layers = append(test_layers, layer{
		29,
		action{},
		[]action{
			{script{}, remap{[]uint16{58}, []uint16{29}, false, false}},
		},
	})

	type test struct {
		pressKeys []uint16
		layers    []layer
		expected  action
	}

	in := test{[]uint16{1}, test_layers, test_layers[0].actions[1]}
	output := decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{29, 31}, test_layers, action{}}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{29, 58}, test_layers, test_layers[1].actions[0]}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{29}, test_layers, action{}}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{40, 58, 30}, test_layers, test_layers[0].actions[2]}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{58, 30}, test_layers, test_layers[0].actions[2]}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{58}, test_layers, test_layers[0].actions[0]}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{}, test_layers, action{}}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)

	in = test{[]uint16{36}, []layer{}, action{}}
	output = decide_actions(in.pressKeys, in.layers)
	assert.Equal(t, in.expected, output)
}

func TestTable_delete_uint16(t *testing.T) {
	var tests = []struct {
		list     []uint16
		value    uint16
		expected []uint16
	}{
		{[]uint16{1, 2, 3}, 0, []uint16{1, 2, 3}},
		{[]uint16{1, 2, 3}, 1, []uint16{2, 3}},
		{[]uint16{1, 2, 3}, 2, []uint16{1, 3}},
		{[]uint16{1, 2, 3}, 3, []uint16{1, 2}},
		{[]uint16{}, 0, []uint16{}},
	}
	for _, test := range tests {
		output := delete_uint16(test.list, test.value)
		assert.Equal(t, output, test.expected)
	}
}

func TestTable_remove_uint16(t *testing.T) {
	var tests = []struct {
		list     []uint16
		value    int
		expected []uint16
	}{
		{[]uint16{1, 2, 3}, 0, []uint16{2, 3}},
		{[]uint16{1, 2, 3}, 1, []uint16{1, 3}},
		{[]uint16{}, 0, []uint16{}},
	}
	for _, test := range tests {
		output := remove_uint16_at(test.list, test.value)
		if !(assert.Equal(t, output, test.expected)) {
			t.Error("Fail")
			t.Error("output  ", output)
			t.Error("expected", test.expected)
		}
	}
}

func TestTable_match_keypress_subset(t *testing.T) {
	var tests = []struct {
		list     []uint16
		subset   []uint16
		expected bool
	}{
		{[]uint16{5, 6, 7, 8}, []uint16{6, 7}, false},
		{[]uint16{5, 6, 7}, []uint16{6, 7}, true},
		{[]uint16{5}, []uint16{5}, true},
		{[]uint16{5}, []uint16{}, false},
		{[]uint16{7}, []uint16{6, 7}, false},
		{[]uint16{}, []uint16{}, false},
	}
	for _, test := range tests {
		output := match_keypress_subset(test.list, test.subset)
		assert.Equal(t, test.expected, output)
	}
}

func TestTable_get_raw_events(t *testing.T) {
	// boiler plate
	type test struct {
		decided_action action
		raw_input      InputEvent
		key_output     []raw_key_action
		script_output  []raw_script_action
	}

	// tests

	data := new(test)

	data.decided_action = action{}
	data.raw_input = InputEvent{}
	data.key_output = []raw_key_action{}
	data.script_output = []raw_script_action{}

	k_output, s_output := get_raw_events(data.decided_action, data.raw_input)

	// empty input implies empty output
	assert.Equal(t, data.key_output, k_output)
	assert.Equal(t, data.script_output, s_output)

	data = new(test)

	data.decided_action = action{
		script{},
		remap{[]uint16{29}, []uint16{16}, false, true},
	}

	data.raw_input = InputEvent{
		syscall.Timeval{},
		keyEvent,
		29,
		keyPressed,
	}

	data.key_output = []raw_key_action{
		{16, keyPressed, 0, 0},
	}
	data.script_output = []raw_script_action{}

	k_output, s_output = get_raw_events(data.decided_action, data.raw_input)

	assert.Equal(t, data.key_output, k_output)
	assert.Equal(t, data.script_output, s_output)

	data = new(test)

	data.decided_action = action{
		script{},
		remap{[]uint16{29}, []uint16{16}, false, true},
	}

	data.raw_input = InputEvent{
		syscall.Timeval{},
		keyEvent,
		29,
		keyReleased,
	}

	data.key_output = []raw_key_action{
		{16, keyReleased, 0, 0},
	}
	data.script_output = []raw_script_action{}

	k_output, s_output = get_raw_events(data.decided_action, data.raw_input)

	assert.Equal(t, data.key_output, k_output)
	assert.Equal(t, data.script_output, s_output)

	data = new(test)

	data.decided_action = action{
		script{},
		remap{[]uint16{29}, []uint16{18}, false, true},
	}

	data.raw_input = InputEvent{
		syscall.Timeval{},
		keyEvent,
		29,
		keyPressed,
	}

	data.key_output = []raw_key_action{
		{18, keyPressed, 0, 0},
	}
	data.script_output = []raw_script_action{}

	k_output, s_output = get_raw_events(data.decided_action, data.raw_input)

	assert.Equal(t, data.key_output, k_output)
	assert.Equal(t, data.script_output, s_output)

	data = new(test)

	data.decided_action = action{
		script{[]uint16{29}, "htop", false},
		remap{},
	}

	data.raw_input = InputEvent{
		syscall.Timeval{},
		keyEvent,
		29,
		keyPressed,
	}

	data.key_output = []raw_key_action{}
	data.script_output = []raw_script_action{
		{"htop", 0, 0},
	}

	k_output, s_output = get_raw_events(data.decided_action, data.raw_input)

	assert.Equal(t, data.key_output, k_output)
	assert.Equal(t, data.script_output, s_output)

	data = new(test)

	data.decided_action = action{
		script{[]uint16{29}, "st", false},
		remap{},
	}

	data.raw_input = InputEvent{
		syscall.Timeval{},
		keyEvent,
		29,
		keyPressed,
	}

	data.key_output = []raw_key_action{}
	data.script_output = []raw_script_action{
		{"st", 0, 0},
	}

	k_output, s_output = get_raw_events(data.decided_action, data.raw_input)

	assert.Equal(t, data.key_output, k_output)
	assert.Equal(t, data.script_output, s_output)
}

func TestTable_get_raw_key_events(t *testing.T) {

}

func TestTable_get_keyboard_path_from_name(t *testing.T) {
	out := get_keyboard_path_from_name("AT Translated Set 2 keyboard")
	assert.Equal(t, "/dev/input/event7", out)
}

func TestTable_list_has(t *testing.T) {
	assert.Equal(t, false, list_uint16_contains([]uint16{}, 0))
	assert.Equal(t, true, list_uint16_contains([]uint16{1}, 1))
	assert.Equal(t, false, list_uint16_contains([]uint16{10}, 1))
	assert.Equal(t, true, list_uint16_contains([]uint16{10, 1}, 1))
	assert.Equal(t, false, list_uint16_contains([]uint16{10, 5}, 1))
}
