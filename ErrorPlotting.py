import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as pl
pl.rcParams["figure.figsize"] = (15,7)

# Replace with your modified URL
# Needs to be copied from the address bar, not the sharing link
# Sheet you want to import needs to also be the first sheet
# https://docs.google.com/spreadsheets/d/1J6bVYfHK-ewSPv4O99pcA1BSmpvQNUyYC2B8Q6qjzfA/edit?gid=2108746868#gid=2108746868
url = 'https://docs.google.com/spreadsheets/d/1x3V_CO74Kd4U3RlOoU7DoPfjDUpmE-vSuSppVNNmPPU/edit?gid=2108746868#gid=2108746868'
new_url = url.replace('/edit?gid=', '/export?format=csv&gid=')
df = pd.read_csv(new_url)
# print(df)
sns.set_theme()
fig, ax1 = pl.subplots()
xLabelNames = ["Mass of Standard \nUsed to Make St-1","Mass of \nYellowcake Used","Volume of St-1\nused to make St-2","Volume of St-2\nAdded to USRM-X","Volume of St-1","Volume of St-2","ICP-OES \nMass Fraction","Yellowcake \n$^{236}$U Atom %","Yellowcake \n$^{238}$U Atom %","NBS U-005b \n$^{236}$U Atom %","NBS U-005b \n$^{238}$U Atom %",]
columnNames = ["Mass of Standard","Mass of Yellowcake Used","Volume of St-1 used to make St-2","Volume of St-2 Added to USRM-X","Volume of St-1","Volume of St-2","ICP-OES Mass Fraction","Yellowcake 236U Atom %","Yellowcake 238U Atom %","U-005 236U Atom %","U-005 238U Atom %"]
for it,column in enumerate(columnNames):
    if (it == 0):
        sns.barplot(data=df,hue = "Material",x=it,y=column,legend = True)
    else:
        sns.barplot(data=df,hue = "Material",x=it,y=column,legend = False)

# ax2 = ax1.twinx()
# ax2.set_ylabel("Contribution to Total Variance")
# sns.barplot(data=df,hue = "Material",x=len(xLabelNames),y="Yellowcake 236U Atom %")
pl.xticks(range(0,len(xLabelNames),1),xLabelNames)
pl.xticks(rotation=80,fontsize = 16)
pl.ylabel("Contribution to Total Variance",fontsize = 16)

pl.ylim(3E-28,3E-21)
# ax2.set_ylim(1E-21,3E-21)
pl.yscale("log")
# ax2.set_yscale("log")
pl.title("USRM-X Series Uncertainty Breakdown",fontsize=20)
pl.tight_layout()
pl.subplots_adjust(left=0.1)
pl.rcParams["figure.figsize"] = (12,4)

pl.savefig("../../../Papers/236U/StandardsPaper/Uncertainty.png")
pl.show()
