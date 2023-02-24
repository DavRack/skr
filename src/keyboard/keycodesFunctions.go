package keyboard

import (
	"reflect"
	"strings"
)

func stringToKeyCodes(input string) (KeyCodeList, bool) {
	inputNoSpaces := strings.ReplaceAll(input, " ", "")
	keys := strings.Split(inputNoSpaces, "+")

	outputKeyCodes := KeyCodeList{}

	for _, key := range keys {
		keyCode := KeyName(key).KeyCode()

		if keyCode == 0 {
			return KeyCodeList{}, false
		}

		outputKeyCodes = append(outputKeyCodes, keyCode)
	}

	return outputKeyCodes, true
}

func interfaceToKeyCode(key interface{}) (keyCode KeyCode, ok bool) {
	keys, ok := interfaceToKeyCodes(key)

	if len(keys) != 1 {
		return 0, false
	}

	return keys[0], true
}

func interfaceToKeyCodes(keys interface{}) (keyCodes KeyCodeList, ok bool) {
	ok = true
	switch keys.(type) {
	case string:
		keyCodes, ok = stringToKeyCodes(keys.(string))
		return
	case int:
		keyCodes = KeyCodeList{KeyCode(keys.(int))}
		return
	case KeyCode:
		keyCodes = KeyCodeList{keys.(KeyCode)}
		return
	case KeyCodeList:
		keyCodes = keys.(KeyCodeList)
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

func (key KeyName) KeyCode() KeyCode {
	keyName := strings.ToUpper(string(key))
	keyCode := keyCodesAlias[keyName]
	if keyCode != 0 {
		return keyCode
	}
	keyCode = systemKeyCodes[keyName]
	return keyCode
}

func (key KeyCode) KeyName() KeyName {
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
