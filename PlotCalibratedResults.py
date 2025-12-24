import matplotlib.pyplot as pl

pl.figure(1)
xValuesExp = [0.95 + i for i in range(4)]
xValuesCal = [1.05 + i for i in range(4)]
colors = ['r','b']
markers = ['<','x']
labels = ['Expected\nValues','Calibrated\nValues']

expectedValues = [3.37739E-10,
8.85629E-10,
2.37726E-09,
7.03827E-09]
expectedErrors = [3.453948E-11,
3.601915E-11,
4.596265E-11,
9.830943E-11]
MatNames = ["USRM-{}".format(i+1) for i in range(4)]

calibratedValues = [3.40E-10,
8.83E-10,
2.42E-09,
7.68E-09]
calibratedErrors = [2.57E-10,
2.25E-10,
4.46E-10,
1.29E-9]

pl.scatter(xValuesExp,expectedValues,color = 'r',marker = '<',label = labels[0])
pl.errorbar(xValuesExp,expectedValues,yerr = expectedErrors,linestyle = '',color = 'k')

pl.scatter(xValuesCal,calibratedValues,color = 'b',marker = 'x',label = labels[1])
pl.errorbar(xValuesCal,calibratedValues,yerr = calibratedErrors,linestyle = '',color = 'k')

# patchExp = mpatches.Patch(color='r', label=labels[0])
# patchCal = mpatches.Patch(color='b', label=lables[1])
#
# pl.legend(handles=[patchExp,patchCal])
pl.legend()
pl.yscale("log")
pl.ylim(5E-11,1.2E-8)
pl.ylabel("$^{236}U/^{238}U$ ratio",fontsize=14)
pl.xlabel("Material",fontsize=14)
pl.xticks([1,2,3,4],MatNames)
pl.savefig("/Users/thomasbailey/Documents/AMS/Dissertation/Figures/Chapter5/USRMCalibratedResults.pdf")
pl.show()
