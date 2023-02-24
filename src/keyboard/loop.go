package keyboard

type keyboardTreeState struct {
	nodeStack        []Node
	nextInCondition  TriggerCondition
	nextOutCondition TriggerCondition
	keboardTree      Node
}

type TriggerCondition func(KeyEvent) bool
type ActionFunction func(Keyboard)

type Node struct {
	Childs         []Node
	FallbackEvents []KeyboardEvent
	InConditions   []TriggerCondition
	OutConditions  []TriggerCondition
	Action         ActionFunction
}

func backedConfig() keyboardTreeState {
	treeState := keyboardTreeState{}
	treeState.keboardTree = Node{
		InConditions: []TriggerCondition{},
		Childs: []Node{
			{
				// this's a layer wich trigger is [30, 31]
				InConditions: []TriggerCondition{
					KeyPressedCondition(30),
					KeyPressedCondition(31),
				},
				Childs: []Node{
					{
						InConditions: []TriggerCondition{
							KeyActiveCondition(32),
						},
						Action: sendCurrentKeyAction,
					},
				},
			},
		},
	}
	return treeState
}

func (kb Keyboard) recursiveKeyboardTreeEvaluation(keyboardNodeTree keyboardTreeState) {
	var currentNode Node

	nodesInStackCount := len(keyboardNodeTree.nodeStack)
	if nodesInStackCount > 0 {
		currentNode = keyboardNodeTree.nodeStack[nodesInStackCount-1]
	} else {
		currentNode = keyboardNodeTree.keboardTree
		keyboardNodeTree.nodeStack = []Node{currentNode}
	}

	inConditions := currentNode.InConditions

	for _, inCondition := range inConditions {
		if inCondition(kb.LastKey) {
		}
	}
}
