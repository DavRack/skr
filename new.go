package main

type keyboardState struct {
	PresedKeys []int
	NodeStack  []node
}

type node struct {
	Trigger           []int
	BlockKey          bool
	In                func(keyboardState) MatchStatus
	Out               func(keyboardState) MatchStatus
	Actions           func(keyboardState)
	ConsecuentActions []node
}

type MatchStatus string

const noMatch MatchStatus = "noMatch"
const partialMatch MatchStatus = "partialMatch"
const fullMatch MatchStatus = "fullMatch"

type key struct {
	code  int
	state int
	time  int
}

var tree node = node{
	In: func(keyboardState) MatchStatus {
		return fullMatch
	},
	Out: func(keyboardState) MatchStatus {
		return fullMatch
	},
	Actions: func(keyboardState) {
		// execute current key event
	},
	ConsecuentActions: []node{
		{ // l_shift + r_shift -> tab
			In: func(keyboardState) MatchStatus {
				// trigger := []int{42, 54}
				// true if both shift press
				// if l_shift && r_shift
				// 	return partialMatch
				return partialMatch
			},
			Out: func(keyboardState) MatchStatus {
				return fullMatch
			},
			Actions: func(keyboardState) {
				// send capslock
			},
			ConsecuentActions: nil,
		},
		{ // doble_tap(A) -> ESC
			In: func(keyboardState) MatchStatus {
				return fullMatch
			},
			Out: func(keyboardState) MatchStatus {
				return fullMatch
			},
			ConsecuentActions: []node{},
		},
	},
}
