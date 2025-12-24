import matplotlib.pyplot as pl
import pandas as pd

AllData = pd.read_csv("Fits.csv")
BadRuns = [3216,3104,3105]
Data235 = AllData.loc[(AllData["mass analyzed"]==235) & (AllData["A2 Mean"]<3000) & (AllData["A2 Mean"]!=0) & (~AllData["Run Number"].isin(BadRuns))]
Data234 = AllData.loc[(AllData["mass analyzed"]==234) & (AllData["A2 Mean"]<3000) & (AllData["A2 Mean"]!=0) & (~AllData["Run Number"].isin(BadRuns))]
fig1,ax1a = pl.subplots()

ax1a.set_title("A2 Data for 235 Runs")
ax1a.set_xlabel("Run Number")
ax1a.set_ylabel("A2 mean (channels)")
ax1a.scatter(Data235["Run Number"],Data235["A2 Mean"],color = 'blue',marker = 'o',label = "Means")
ax1b = ax1a.twinx()
ax1b.set_xlabel("Run Number")
ax1b.set_ylabel("A2 StDev (Channels)")
ax1b.scatter(Data235["Run Number"],Data235["A2 StDev"],color = 'red', marker = 'x', label = "StdDevs")
lines1a, labels1a = ax1a.get_legend_handles_labels()
lines1b, labels1b = ax1b.get_legend_handles_labels()
ax1b.legend(lines1a + lines1b, labels1a + labels1b, loc=0)

fig2,ax2a = pl.subplots()
ax2a.set_title("A2 Data for 234 Runs")
ax2a.set_xlabel("Run Number")
ax2a.set_ylabel("A2 mean (channels)")
ax2a.scatter(Data234["Run Number"],Data234["A2 Mean"],color = 'blue',marker = 'o',label = "Means")
ax2b = ax2a.twinx()
ax2b.set_xlabel("Run Number")
ax2b.set_ylabel("A2 StDev (Channels)")
ax2b.scatter(Data234["Run Number"],Data234["A2 StDev"],color = 'red', marker = 'x', label = "StdDevs")
lines2a, labels2a = ax2a.get_legend_handles_labels()
lines2b, labels2b = ax2b.get_legend_handles_labels()
ax2a.legend(lines2a + lines2b, labels2a + labels2b, loc=0)
pl.show()
