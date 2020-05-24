from remapData import keyCodes

configPath = "/home/david/.config/skr/skr.config"

press = 1
release = 0
BLANK = -1
class action:
    wait = 0
    keyState = 1
    def __init__(self,hotkey,arguments,action):
        self.hotkey = getKeyCode(hotkey)
        self.arguments = arguments
        self.wait = getWait(arguments)
        self.keyState = getKeyState(arguments)
        self.action = action
    def outputFullKey(self):
        sleep = self.wait
        if sleep == 0:
            sleep = 0.01
        print("KeyRemap",self.hotkey,self.action,1,sleep)
        print("KeyRemap",self.hotkey,self.action,0,0.01)

class keyAction(action):
    def output(self):
        print("KeyRemap",self.hotkey,self.action,BLANK,0)
    def macroOutput(self):
        self.outputFullKey()

class scriptAction(action):
    def output(self):
        print("script",self.hotkey,self.keyState,self.wait,self.action)
    def macroOutput(self):
        sleep = self.wait
        if sleep == 0:
            sleep = 0.01
        print("script",self.hotkey,self.keyState,sleep,self.action)
            
class textAction(action):
    def output(self):
        keyAction = action(self.hotkey,[],"0")
        text = iter(self.action)

        char = next(text,None)

        while char is not None:
            keyAction.action = getKeyCode(char)
            keyAction.outputFullKey()
            char = next(text,None)

class keyboardPathAction:
    def __init__(self,path):
        self.path = path
    def output(self):
        print("KeyboardPath",self.path)

class newLayerAction:
    def __init__(self,key):
        self.keyCode = getKeyCode(key)
    def output(self):
        print("NewLayer",self.keyCode)

class parser:
    tokens = []

    def parse(self,line):
        lineTokens = []
        toParse = line.strip() # remove sorrounding spaces
        if len(toParse) > 0 and toParse[0] != "#":
            lineTokens = toParse.split(" ",1)
            if lineTokens[0] != "=>":
                lineTokens = toParse.split(" ",2)
            else: 
                lineTokens.insert(0,"")
            self.tokens.append(lineTokens)

    def tokenToAction(self):
        hotkey = 0
        print("NewLayer",0)
        for line in self.tokens:
            if len(line) == 1: # not a remap or macro
                token = line[0]
                arguments = getArguments(token)[0][0]

                if "KeyboardPath" in token:
                    action = keyboardPathAction(arguments)
                elif "NewLayer" in token:
                    action = newLayerAction(arguments)

                action.output()

            # key remap
            else:
                if line[0] != "":
                    hotkey = line[0]

                action = getAction(hotkey,line[2])

                if line[1] == "->":
                    action.output()
                elif line[1] == "=>":
                    action.macroOutput()

def isKey(token):
    code = getKeyCode(token)
    return code > 0

def getKeyCode(token):
    if type(token) == int:
        return token

    if token == " ":
        return 57

    key = "KEY_"+token.upper()
    if key in keyCodes:
        keyCode = keyCodes[key]
        return keyCode

    elif token[0:token.find("(")] == "KeyCode":
        keyCode = int(getArguments(token)[0][0])
        return keyCode

    else:
        return -1

def getArguments(token):
    final = []
    if token.find("(") < token.find(")"):
        arguments = token[token.find("(")+1:token.rfind(")")]
        if len(arguments) > 0:
            arguments = arguments.strip()
            arguments = arguments.split(",")
            for arg in arguments:
                argument = arg.split(":")
                map(str.strip,argument)
                final.append(argument)
    return final

def getAction(hotkey,token):
    actions = token.split("=",1)

    function = actions[0]
    function = function.strip()
    arguments = []

    if len(actions) == 1:
        if function[0] == '"':
            action = function[1:-1]
            return textAction(hotkey,arguments,action)
        elif isKey(function):
            action = getKeyCode(function)
            return keyAction(hotkey,arguments,action)
    else:
        if "Script" in function:
            arguments = getArguments(function)
            return scriptAction(hotkey,arguments,actions[1])

def getWait(argList):
    for arg in argList:
        if arg[0].strip() == "wait":
            return float(arg[1])
    return 0

def getKeyState(argList):
    for arg in argList:
        if arg[0].strip() == "keyState":
            if arg[1].strip() == "Press":
                return 1
            elif arg[1].strip() == "Release":
                return 0
    return 1

file1 = open(configPath, 'r')
lines = file1.readlines()

parser = parser()

for line in lines:
    parser.parse(line)

parser.tokenToAction()
