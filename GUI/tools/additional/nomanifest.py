import re
import os
import sys

for file in sys.argv[1:]:
    if (os.path.exists(file)):
        f = open(file, 'r')
        allLines = f.readlines()
        f.close()
        for i in range(len(allLines)-1):
            line = allLines[i]
            line = line.lstrip()
            if (line.startswith("Name=\"VCLinkerTool\"")):
                allLines[i+1:1] = "\t\t\t\tGenerateManifest=\"false\"\n"
        f = open(file, 'w')
        for line in allLines:
            f.write(line)
        f.close()
        print "Adapted file: " + file
    else:
        print "Couldn't find find: " + file