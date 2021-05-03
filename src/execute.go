package main

import "fmt"

func execute_raw_keys(raw_keys []raw_key_action) {
	for _, raw_key := range raw_keys {
		fmt.Println("keys", raw_key)
	}
}
func execute_raw_scripts(raw_scripts []raw_script_action) {
	for _, raw_script := range raw_scripts {
		fmt.Println("script", raw_script)
	}
}
