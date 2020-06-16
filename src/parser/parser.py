import parserClasses
import os

user = os.getenv("USER")

if user == "root":
    user == os.getenv("SUDO_USER")
    if user == "":
        user == "root"

configPath = "/home/"+user+"/.config/skr/skr.config"

if os.path.exists(configPath):

    configFile = open(configPath, 'r')
    lines = configFile.readlines()

    finalLines = parserClasses.parse(lines)

    if len(finalLines) == 1:
        print("FAIL empty config file")

    else:
        for line in finalLines:
            print(line)
else:
    print("FAIL can't find config file, to create one run skr --init")
