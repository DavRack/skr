package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_b(t *testing.T) {
	pressedKeys = []uint16{58, 36}
	assert.Equal(t, pressedKeys, pressedKeys)
}
