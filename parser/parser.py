import parserClasses
import os

configPath = os.path.expanduser("~/.config/skr/skr.config")

if os.path.exists(configPath):

    file1 = open(configPath, 'r')
    lines = file1.readlines()

    tokenLines = []

    for line in lines:
        tokens = parserClasses.lineToTokens(line)
        if tokens:
            tokenLines.append(tokens)

    finalLines = parserClasses.tokenToAction(tokenLines)

    for line in finalLines:
        print(line)
else:
    print("error file dont exist")
