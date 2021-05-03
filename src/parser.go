package main

func parse() []layer {
	var layers []layer
	layers = append(layers,
		layer{
			0,
			action{},
			[]action{
				{script{}, remap{[]uint16{30}, []uint16{29}, false, true}},
				{script{}, remap{[]uint16{58, 36}, []uint16{108}, false, true}},
			},
		})
	return layers
}
