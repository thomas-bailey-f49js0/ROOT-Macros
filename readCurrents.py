import numpy as np
from os.path import exists

# Get all of the run numbers, as well as info
# on when a transmission measurement was taken
Runs = np.loadtxt("RunData.tsv",dtype='int',skiprows = 1,usecols = 0, delimiter = '\t')
f = open("Currents.csv", "w")
# This array has every run number as well as
# the cathode number when transmission was measured
for i in Runs:
    # Transmission meausrement was taken
    if (i <3000) :
        f.write("{},Transmission\n".format(i))
    # Was a run, but not one where jumping was used
    elif (not exists("../CurrentFiles/run{}.current".format(i))):
        f.write("{},NoCurrent\n".format(i))
    # Was a run, and we do have a current file for it
    else:
        currents = -1E9*np.loadtxt("../CurrentFiles/run{}.current".format(i),usecols = 1)
        avg = np.average(currents)
        stdev = np.std(currents)
        f.write("{},{:.6f},{:.6f}\n".format(i,avg,stdev))
