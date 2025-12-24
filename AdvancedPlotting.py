import pandas as pd
import numpy as np
from datascroller import scroll
import matplotlib.pyplot as pl
import re
import matplotlib.patches as mpatches
import pandas as pd

# Replace with your modified URL
# Needs to be copied from the address bar, not the sharing link
# Sheet you want to import needs to also be the first sheet
# https://docs.google.com/spreadsheets/d/1J6bVYfHK-ewSPv4O99pcA1BSmpvQNUyYC2B8Q6qjzfA/edit?gid=2108746868#gid=2108746868
url = 'https://docs.google.com/spreadsheets/d/1icJftrPnXzKfy7rprYRpfwIebEnd80TKQao_1rrkwHI/edit?gid=2108746868#gid=2108746868'
new_url = url.replace('/edit?gid=', '/export?format=csv&gid=')
# print(new_url)
# List of columns in the spreadsheet. As of 9/24/24
# ['Run Number', 'Cathode', 'Material', 'mass injected', 'MassAnalyzed', 'Time (sec)',
# 'Pellatron TV (MV)', 'Decapot TV (MV)', 'attenuation', 'WF Plates', 'MBS', '238U NEC Cup Before (nA)',
 # '238U NEC Cup After (nA)', '238U NEC Jumping (nA)', '235U NEC Current (If readable) (nA)',
 # 'Beamline FC current (nA)', 'Transmission', 'Transmission For Calculation', '# of Counts in DAQ region',
  # 'Total Counts', '3 sigma counts', 'Notes', 'Jumping Current from Files', 'counts/nC', '3sigma counts/nC',
   # '3sigma count/nC at Det', 'Unnamed: 26', 'Run', 'Time', 'MCP1', 'MCP2', 'MCP Transmission', 'Trigger',
   # 'Trigger Live', 'Unnamed: 34', 'Unnamed: 35', 'A1 Mean', 'A1 StDev', 'A2 Mean', 'A2 StDev', 'ToF Mean', 'ToF StDev']
df = pd.read_csv(new_url)
df.RunNumber = pd.to_numeric(df.RunNumber, errors='coerce')
# print(df)
mass = 236
dfGoodRuns = df[df["RunNumber"]>3600]
df236Runs = df[df["MassAnalyzed"]==236]
df236GoodRuns = dfGoodRuns[dfGoodRuns["MassAnalyzed"]==236]
df236StdRuns = df236GoodRuns[df236GoodRuns["Material"].str.contains("Std")]
df235Runs = df[df["MassAnalyzed"]==235]
numSamples235 = df235Runs['Cathode'].nunique()
cathodes235 = df235Runs['Cathode'].unique()
# print(df236Runs)
numSamples = df236Runs['Cathode'].nunique()
cathodes = df236Runs['Cathode'].unique()
# print(cathodes)
# print (numSamples)
ycolumn = "3sigma 236/238 ratio"
# print(df236Runs[ycolumn])
# min = df236Runs[ycolumn].min()
# if (min <1E-14 ):
#     min = 1E-11
#
# max = df236Runs[ycolumn].max()
# # print(min,max)
# fig, axs = pl.subplots(3,3)
# for it, cathode in enumerate(cathodes):
#     df236Cathode=df236Runs[df236Runs["Cathode"]==cathode]
#     # print("Material: ",df236Cathode['Material'].iloc[0])
#     axs[it//3,it%3].scatter(df236Cathode["Run Number"],df236Cathode[ycolumn])
#     axs[it//3,it%3].set_title("Cathode {:2.0f}: {}".format(cathode,df236Cathode['Material'].iloc[0]))
#     axs[it//3,it%3].set_yscale('log')
#     axs[it//3,it%3].set_ylim([min,max])
#     axs[it//3,it%3].set_xlim([3600,3700])
# fig.suptitle("Mass: {}, {}".format(mass,ycolumn))

# Figure to show how blank ratios spiked
# pl.figure(2)
# df236Cathode=df236Runs[df236Runs["Cathode"]==18]
# pl.scatter(df236Cathode["Run Number"],df236Cathode[ycolumn], color = 'blue')
# pl.title("UO$_3$ Blank",fontsize=20)
# pl.yscale('log')
# pl.ylim([1E-8,1E-6])
# pl.xlabel("Run Number",fontsize=18)
# pl.ylabel("Raw Concentration",fontsize=18)

# Figure to show 235/234 ratios for each cathode
# ycolumn = "235/234Ratio"
# markers = ['.','o','v','^','1','3','8','s','p','P','*','h','X','D','d','<','>','H','2','4','5','6','7']
# pl.figure(2)
# # print(cathodes235)
# for it, cathode in enumerate(cathodes235):
#     # print(it)
#     # print(cathode)
#     # print(markers[it])
#     df235Cathode=df235Runs[df235Runs["Cathode"]==cathode]
#     # print("Material: ",df236Cathode['Material'].iloc[0])
#     pl.scatter(df235Cathode["Run Number"],df235Cathode[ycolumn],label = df235Cathode['Material'].iloc[0],marker = markers[it])
# # print("out")
# pl.title("$^{235}$U/$^{234}$U Ratios",fontsize=24)
# df235Runs.dropna(subset = ycolumn,inplace = True)
# avRatio = df235Runs.loc[:,ycolumn].mean()
# stdevRatio = df235Runs[ycolumn].std()
# # scroll(df235Runs)
# # print(df235Cathode.loc[:,ycolumn])
# pl.axhline(avRatio,color = 'k',linewidth = 3,label = "Average Ratio")
# pl.axhline(133.4,color = 'b',linewidth = 3,label = "Natural Ratio")
# # pl.axhline(avRatio+stdevRatio,color = 'k',linewidth = 1)
# # pl.axhline(avRatio-stdevRatio,color = 'k',linewidth = 1)
# # print([df235Runs.loc[:,"Run Number"].min(),df235Runs.loc[:,"Run Number"].max()])
# pl.fill_between([df235Runs.loc[:,"Run Number"].min(),df235Runs.loc[:,"Run Number"].max()],avRatio+stdevRatio,avRatio-stdevRatio,color = [0.8,0,0,.3])
# pl.xlabel("Run Number",fontsize=20)
# pl.legend()
# print(avRatio,stdevRatio)

# pl.figure(3)
# pl.scatter(df236GoodRuns["Run Number"],df236GoodRuns["236 Concentration via 235"],label = "235U Method")
# pl.scatter(df236GoodRuns["Run Number"],df236GoodRuns["236ConcentrationVia234"],label = "234U Method")
# pl.scatter(df236GoodRuns["Run Number"],df236GoodRuns["236Concentration via Std Correction"],label = "StdCorrection Method")
# # pl.yscale("log",base=3)
# pl.legend()
# pl.show()
#
# pl.figure(4)
# xValues = [0.85,0.95,1.05,1.15]
# colors = ['r','b','k','g']
# markers = ['<','>','x','_']
# labels = ['$^{234}U$ Method','$^{235}U$ Method','StdCorr Method','ExpValues']
# columnNames = ['236ConcentrationVia234','236 Concentration via 235','236Concentration via Std Correction']
# errColumnNames = ['236 Concentration via 234 Error','236 Concentration via 235 Error','236Concentration via Std Correction Error']
# for i in columnNames:
#     df236GoodRuns[i] = pd.to_numeric(df236GoodRuns[i], errors = 'coerce')
#
# for i in errColumnNames:
#     df236GoodRuns[i] = pd.to_numeric(df236GoodRuns[i], errors = 'coerce')
#
# url = 'https://docs.google.com/spreadsheets/d/1J6bVYfHK-ewSPv4O99pcA1BSmpvQNUyYC2B8Q6qjzfA/edit?gid=1692047820#gid=1692047820'
# new_url = url.replace('/edit?gid=', '/export?format=csv&gid=')
# df2 = pd.read_csv(new_url)
# dfStandards = df2[df2["Material"].notnull()]
# # print(dfStandards)
# expectedValues = dfStandards['Expected Ratio 236/238'].tolist()
# expectedErrors = dfStandards['Total Error'].tolist()
# MatNames = dfStandards['Material'].tolist()




# cathodes = df236GoodRuns['Cathode'].unique()
# # print("Cathodes: ",sorted(cathodes))
# for it, cathode in enumerate(sorted(cathodes)):
#     df236GoodRunsCathode=df236GoodRuns[df236GoodRuns["Cathode"]==cathode]
#     yValues = [df236GoodRunsCathode.loc[:,i].mean() for i in columnNames]
#     # meth234 = df236GoodRunsCathode.loc[:, '236ConcentrationVia234'].mean()
#     # meth235 = df236GoodRunsCathode.loc[:, '236 Concentration via 235'].mean()
#     # methStd = df236GoodRunsCathode.loc[:, '236Concentration via Std Correction'].mean()
#     yerrs = [df236GoodRunsCathode.loc[:,i].mean() for i in errColumnNames]
#     # methStdError = df236GoodRunsCathode.loc[:, '236Concentration via Std Correction Error'].mean()
#     # yValues = [meth234,meth235,methStd,expectedValues[it]]
#     yValues.append(expectedValues[it])
#     yerrs.append(expectedErrors[it])
#     print(yValues)
#     # yerrs = [0,0,methStdError,0]
#     for i in range(4):
#         pl.scatter(xValues[i]+it,yValues[i],color = colors[i],marker = markers[i])
#         pl.errorbar(xValues[i]+it,yValues[i],yerr = yerrs[i],color = colors[i])
#
#     # print("Material: ",df236Cathode['Material'].iloc[0])
#     # pl.scatter(df236GoodRunsCathode["Run Number"],df236GoodRunsCathode["236 Concentration via 235"],label = "235U Method",marker = markers[it],color = 'r')
#     # pl.scatter(df236GoodRunsCathode["Run Number"],df236GoodRunsCathode["236ConcentrationVia234"],label = "234U Method",marker = markers[it],color = 'b')
#     # pl.scatter(df236GoodRunsCathode["Run Number"],df236GoodRunsCathode["236Concentration via Std Correction"],label = "StdCorrection Method",marker = markers[it],color = 'k')
# # pl.title("$^{236}$U/$^{238}$U Ratios",fontsize=24)
# for i in range(4):
#     pl.scatter(1,1,color = colors[i],label = labels[i],marker = markers[i])
# pl.legend()
# # patch234 = mpatches.Patch(color='r', label='234U Method')
# # patch235 = mpatches.Patch(color='b', label='235U Method')
# # patchStd = mpatches.Patch(color='k', label='StdCorr Method')
# # patchExp = mpatches.Patch(color='g', label='ExpValues')
# # pl.legend(handles=[patch234,patch235,patchStd,patchExp])
# pl.yscale("log")
# pl.ylim(5E-11,1.2E-8)
# pl.ylabel("$^{236}U/^{238}U$ ratio",fontsize=14)
# pl.xlabel("Material",fontsize=14)
# pl.xticks([1,2,3,4,5],MatNames)
# # pl.legend()
# # pl.figure(5)
# #
# # ycolumn = "Counts/TriggerLive"
# # dfGoodRuns[ycolumn] = pd.to_numeric(dfGoodRuns[ycolumn], errors = 'coerce')
# # df236GoodRuns[ycolumn] = pd.to_numeric(df236GoodRuns[ycolumn], errors = 'coerce')
# # pl.scatter(dfGoodRuns['Run Number'],dfGoodRuns[ycolumn])
# # pl.scatter(df236GoodRuns['Run Number'],df236GoodRuns[ycolumn])
# pl.savefig("../../../Papers/236U/StandardsPaper/Results.pdf")

pl.figure(6)
ycolumn = "Average SCF with 0 count Runs"
Standards = ['Std {}'.format(i+1) for i in range(4)]
Standards.append('Vienna Std')
print(Standards)
for i in Standards:
    df236StdCathode=df236StdRuns[df236StdRuns["Material"]==i]
    pl.scatter(df236StdCathode['Run Number'],df236StdCathode[ycolumn],label = i)
    # pl.errorbar(df236StdCathode['Run Number'],df236StdCathode[ycolumn],np.sqrt(df236StdCathode[ycolumn]),ls='')
# ycolumn = "Average Standard Correction Factor"
# pl.scatter(df236StdRuns['Run Number'],df236StdRuns[ycolumn],label = 'Average of each set',marker='_')
# pl.errorbar(df236StdRuns['Run Number'],df236StdRuns['Standard Correction Factor'],df236StdRuns['SCF Error'],ls='',color = 'k')
pl.xlabel("Run Number")
pl.ylabel("Standard Correction Factor")
pl.legend()


#
# pl.figure(7)
# ycolumn = "236 Concentration via 235"
# pl.scatter(df236GoodRuns['Run Number'],df236GoodRuns[ycolumn],label = '235 Method')
# # pl.errorbar(df236GoodRuns['Run Number'],df236GoodRuns[ycolumn],np.sqrt(df236StdRuns[ycolumn]),ls='')
# ycolumn = "236ConcentrationVia234"
# pl.scatter(df236GoodRuns['Run Number'],df236GoodRuns[ycolumn],label = '234 Method')
# ycolumn = "236Concentration via Std Correction"
# pl.scatter(df236GoodRuns['Run Number'],df236GoodRuns[ycolumn],label = 'Standard Correction Method')
#
# pl.xlabel("Run Number")
# pl.ylabel("236U/238U ratio")
# pl.yscale('log')
# pl.legend()




pl.show()
