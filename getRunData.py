import numpy as np
from os.path import exists

Runs = np.loadtxt("RunData.tsv",dtype='int',skiprows = 1,usecols = 0)
f = open("AllScalars.csv","w")
f.write("Run,Time,MCP1,MCP2,Trig,TrigL\n")

for i in Runs:
    if (i<3000):
        f.write("Transmission\n")
        continue
    # fileName = "run{}.scalers".format(runNumber)
    # print(fileName)
    fileName = "../../run{}.scalers".format(i)
    if (not exists("../../run{}.scalers".format(i))):
        f.write("{},0,0,0,0,0\n".format(i))
    else:
        file = open(fileName,'r')
        for line in file.readlines():
            if ("Duration" in line):
                timeString = line[-9:]
                totalTime = int(timeString[0:2])*3600+int(timeString[3:5])*60+int(timeString[6:8])
            elif ("MCP1" in line):
                MCP1Scalers = line.split()
            elif ("MCP2" in line):
                MCP2Scalers = line.split()
            elif ("Trigger" in line and "live" not in line):
                Trigger = line.split()
            elif ("Trigger.live" in line):
                TriggerLive = line.split()
        f.write("{:4d},{:5d},{:8s},{:8s},{:8s},{:8s}\n".format(i,totalTime,MCP1Scalers[1],MCP2Scalers[1],Trigger[1],TriggerLive[1]))
    file.close()
