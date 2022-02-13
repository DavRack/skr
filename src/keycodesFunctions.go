package main

import (
	"reflect"
	"strings"
)

func interfaceToKeyCode(key interface{}) (keyCode KeyCode, ok bool) {
	ok = true
	switch key.(type) {
	case string:
		keyCode = KeyName(key.(string)).keyCode()
		return
	case int:
		keyCode = KeyCode(key.(int))
		return
	case KeyCode:
		keyCode = key.(KeyCode)
		return
	default:
		ok = false
		return
	}
}

func interfacesToKeyCodes(keys []interface{}) (keyCodes KeyCodeList, ok bool) {
	ok = true
	if len(keys) == 1 && reflect.TypeOf(keys[0]).Name() == "KeyCodeList" {
		Keys := keys[0].(KeyCodeList)
		keys = []interface{}{}
		for _, key := range Keys {
			keys = append(keys, key)
		}
	}
	for _, key := range keys {
		keyCode, Ok := interfaceToKeyCode(key)
		if !Ok {
			ok = Ok
			keyCodes = KeyCodeList{}
			return
		}
		keyCodes = append(keyCodes, keyCode)
	}
	return
}

func (key KeyName) keyCode() KeyCode {
	keyName := strings.ToUpper(string(key))
	keyCode := keyCodesAlias[keyName]
	if keyCode != 0 {
		return keyCode
	}
	keyCode = systemKeyCodes[keyName]
	return keyCode
}

func (key KeyCode) keyName() KeyName {
	keyName, ok := mapkey(keyCodesAlias, key)
	if ok {
		return KeyName(keyName)
	}
	keyName, ok = mapkey(systemKeyCodes, key)
	if ok {
		return KeyName(keyName)
	}
	return ""
}

func mapkey(m map[string]KeyCode, value KeyCode) (key string, ok bool) {
	for k, v := range m {
		if v == value {
			key = k
			ok = true
			return
		}
	}
	return
}
