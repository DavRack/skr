package main

func remove_layer_at(list []layer, index int) []layer {
	return append(list[:index], list[index+1:]...)
}

// remove_uint16_at deletes element at index i
func remove_uint16_at(list []uint16, index int) []uint16 {
	if index >= 0 && index < len(list) {
		return append(list[:index], list[index+1:]...)
	}
	return list
}

// delete_uint16 deletes the first instance of value in list
// and returns a new list with those values
func delete_uint16(list []uint16, value uint16) []uint16 {
	for i := 0; i < len(list); i++ {
		if list[i] == value {
			return remove_uint16_at(list, i)
		}
	}
	return list
}
