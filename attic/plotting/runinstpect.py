from rundata import *

#rundata(energy, runstart, runend) or rundata(energy, runlist)
rd = rundata("70n",1623,1623)
#rd = rundata(70,[1096,1097,1098,1099,1100,1101,1242,1103,1104,1243])

fig, ax, figtxt, axtxt, peakpositiondict, cconstantdict = rd.get_and_plot_calibration_constants(fitting=False)
pltdir = '/home/pioneer/shiftdata/plots/'
filenamefits = f"cconstants_{rd.energy}_fits.pdf"
#filenamecc = f"cconstants_{rd.energy}.pdf"

fig.savefig(pltdir+filenamefits)
#figtxt.savefig(pltdir+filenamecc)
plt.show()

rd.write_cconstans_to_csv(cconstantdict)#,replaceFile=True)

#hodoscope (total and one for each run)