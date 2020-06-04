from remapData import keyCodes

BLANK = -1

class failAction:
    def output(self):
        return ["FAIL"]
    def macroOutput(self):
        return ["FAIL"]
    def swapOutput(self):
        return ["FAIL"]

class action(failAction):
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
        o = ["KeyRemap",self.hotkey,self.action,1,sleep]
        o2 = ["KeyRemap",self.hotkey,self.action,0,0.01]

        return [' '.join(str(e) for e in o),' '.join(str(e) for e in o2)]

class keyAction(action):
    def output(self):
        o = ["KeyRemap",self.hotkey,self.action,BLANK,0]
        return [' '.join(str(e) for e in o)]

    def macroOutput(self):
        return self.outputFullKey()

    def swapOutput(self):
        o = ["KeyRemap",self.hotkey,self.action,BLANK,0]
        o2 = ["KeyRemap",self.action,self.hotkey,BLANK,0]
        return [' '.join(str(e) for e in o),' '.join(str(e) for e in o2)]

class scriptAction(action):
    def output(self):
        o = ["Script",self.hotkey,self.keyState,self.wait,self.action]
        return [' '.join(str(e) for e in o)]

    def macroOutput(self):
        sleep = self.wait
        if sleep == 0:
            sleep = 0.01

        if self.hotkey == 0:
            return ["FAIL"]
        else:
            o = ["Script",self.hotkey,self.keyState,sleep,self.action]
            return [' '.join(str(e) for e in o)]

class textAction(action):
    def output(self):
        out = []
        keyAction = action(self.hotkey,[],"0")
        for char in self.action:
            keyAction.action = getKeyCode(char)

            for i in keyAction.outputFullKey():
                out.append(i)

        return out

    def macroOutput(self):
        return self.output()

class keyboardPathAction(failAction):
    def __init__(self,path):
        self.path = path

    def output(self):
        o = ["KeyboardPath",self.path]
        return [' '.join(str(e) for e in o)]

class newLayerAction(failAction):
    def __init__(self,key):
        self.keyCode = getKeyCode(key)

    def output(self):
        if self.keyCode > 0:
            o = ["NewLayer",self.keyCode]
            return [' '.join(str(e) for e in o)]
        else:
            return ["FAIL"]

def isKey(token):
    code = getKeyCode(token)
    return code > 0

def getKeyCode(token):
    if type(token) == int:
        return token

    if token == " ":
        return 57

    key = "KEY_"+token.strip().upper()

    if key in keyCodes:
        keyCode = keyCodes[key]
        return keyCode

    elif token[0:token.find("(")].upper() == "KEYCODE":
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
    if len(token) == 0:
        return failAction()
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
        if "SCRIPT" in function.upper():
            arguments = getArguments(function)
            return scriptAction(hotkey,arguments,actions[1])

        if "KEY" in function.upper():
            arguments = getArguments(function)
            return keyAction(hotkey,arguments,actions[1])

def getWait(argList):
    if type(argList) == str:
        return float(argList)

    for arg in argList:
        if arg[0].strip().upper() == "WAIT":
            return float(arg[1])
    return 0

def getKeyState(argList):
    for arg in argList:
        arg[0] = arg[0].upper()

        if arg[0].strip() == "KEYSTATE":
            arg[1] = arg[1].upper()

            if arg[1].strip() == "PRESS":
                return 1
            elif arg[1].strip() == "RELEASE":
                return 0
    return 1

def lineToTokens(line):
    parserTokens = ["<->","->","=>"]
    lineTokens = []
    toParse = line.strip() # remove sorrounding spaces
    if len(toParse) > 0 and toParse[0] != "#":
        for parserToken in parserTokens:
            lineTokens = toParse.split(parserToken,1)
            if len(lineTokens) == 2:
                lineTokens.insert(1,parserToken)
                lineTokens = list(map(str.strip,lineTokens))
                break
        return lineTokens
    else:
        return []

def tokenToAction(tokens,hotkey):
    if len(tokens) == 1:
        return failAction().output(),hotkey

    command = tokens[2]
    operator = tokens[1]
    trigger = tokens[0]

    if trigger != "":
        hotkey = trigger

    action = getAction(hotkey,command)

    if trigger.upper() == "KEYBOARDPATH":
        action = keyboardPathAction(command)

    elif trigger.upper() == "NEWLAYER":
        action = newLayerAction(command)

    if operator == "->":
        hotkey = 0
        output = action.output()

    elif operator == "=>":
        output = action.macroOutput()

    elif operator == "<->":
        hotkey = 0
        output = action.swapOutput()

    return output,hotkey

def parse(lines):
    finalLines = []
    finalLines.append("NewLayer 0")
    hotkey = 0

    for ln,line in enumerate(lines):
        tokens = lineToTokens(line)
        if tokens:

            parsedLine,hotkey = tokenToAction(tokens,hotkey)

            if "FAIL" == parsedLine[0]:
                msg = "FAIL to parse line "+str(ln+1)
                finalLines.append(msg)
                break
            else:
                finalLines = finalLines+parsedLine

    return finalLines
