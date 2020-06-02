import parserClasses
import os

configPath = os.path.expanduser("~/.config/skr/skr.config")

if os.path.exists(configPath):

    configFile = open(configPath, 'r')
    lines = configFile.readlines()
    
    finalLines = parserClasses.parse(lines)

    for line in finalLines:
        print(line)
else:
    print("error file dont exist")
