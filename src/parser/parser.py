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
    print("can't find config file in ~/.config/skr/skr.config")
    print("create a blanck config file?")
    
    userAnswer = input("yes/no ")
    userAnswer = userAnswer.upper()

    if userAnswer == "YES" or userAnswer == "Y":
        path = os.path.expanduser("~/.config/skr") 
        os.makedirs(path)
        path = path+"/skr.config"
        os.mknod(path)
        print("config file created")
