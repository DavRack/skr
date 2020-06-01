import parserClasses
import os

configPath = os.path.expanduser("~/.config/skr/skr.config")

if os.path.exists(configPath):

    file1 = open(configPath, 'r')
    lines = file1.readlines()

    parser = parserClasses.parser()

    for line in lines:
        parser.parse(line)

    parser.tokenToAction()

    for line in parser.finalLines:
        print(line)
else:
    print("error file dont exist")
