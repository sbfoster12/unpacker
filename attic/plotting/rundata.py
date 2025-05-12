from nearline_helpers import *

import ROOT as r
import os
import hist
import uproot
from scipy.stats import norm

from matplotlib.patches import RegularPolygon
import csv

outdir = '/home/pioneer/shiftdata/'

"""
def GetMaximumInsideRanges(hist,x0,x1):
    # Define the ROI in terms of bin indices
    x0_bin = hist.GetXaxis().FindBin(x0)
    x1_bin = hist.GetXaxis().FindBin(x1)
    
    # Create a smaller histogram representing the ROI
    roi_histogram = hist.Clone("ROI_Histogram")
    roi_histogram.GetXaxis().SetRange(x0_bin, x1_bin)

    # Find the bin with the maximum value in the ROI histogram
    max_bin = roi_histogram.GetMaximumBin()
    x_max = roi_histogram.GetBinCenter(max_bin)
    max = roi_histogram.GetBinContent(max_bin)

    return x_max,max

"""

def parse_value(t, v):
    if 'INT' in t:
        return int(v)
    elif t == "BOOL":
        if v == 'y':
            return True
        else:
            return False
    elif t in ("FLOAT", "DOUBLE"):
        return float(v)
    elif t in ("CHAR", "STRING", "LINK"):
        return v
    else:
        print(t)
        return v
    
def parse_odb(odb):
    if odb.tag in ("dir", "odb"):
        return {c.attrib['name'] : parse_odb(c) for c in odb}
    elif odb.tag == "key":
        t = odb.attrib['type']
        return parse_value(t, odb.text)
    elif odb.tag == "keyarray":
        t = odb.attrib['type']
        return [parse_value(t, c.text) for c in odb]

PMTmap = {
    (-2, 1): (1,0),
    ( 0, 1): (1,1),
    ( 2, 1): (1,2),
    (-3, 0): (1,3),
    (-1, 0): (1,4),
    ( 1, 0): (2,0),
    ( 3, 0): (2,1),
    (-2,-1): (2,2),
    ( 0,-1): (2,3),
    ( 2,-1): (2,4),
}

def getPMTfromFile(file):
    
    odb = parse_odb(ET.fromstring(str(file.Get("odb").odbString)[:-1]))    
    x = odb["Equipment"]['ISEL']["Variables"]["Output"][0]
    y = odb["Equipment"]['ISEL']["Variables"]["Output"][1]
    if file.Get("odb").runNumber <= 1276:
        xref = 13500
        yref = 22540
    elif file.Get("odb").runNumber >= 1623:
        xref = 12900
        yref = 22400
    else:
        NotImplementedError()
    
    i = round(((x-xref) / 4000)*2) #top/borrom row: -2,0,2, middle row: -3,-1,1,3
    j = round(-(y-yref) / 4000) #1,0,-1 from top to bottom

    amc, channel = PMTmap[i,j]
    return amc, channel


amcs = [1,2]
channelTags = [0,1,2,3,4]

crystalmap = {
    (1,0):0,
    (1,1):1,
    (1,2):2,
    (1,3):3,
    (1,4):4,
    (2,0):5,
    (2,1):6,
    (2,2):7,
    (2,3):8,
    (2,4):9
}

class rundata:
    def __init__(self,energy, runs, runend = None, focused = True, indir = '/home/pioneer/dqm/nearline/'):
        if type(runs) == list or type(runs) == np.ndarray:
            if runend != None:
                raise TypeError("If you import a list, runend should be None.")
        elif runend:
            if type(runs) != int or type(runend) != int:
                raise TypeError("runstart and rundend both have to be int if both are given.")
            runs = np.arange(runs,runend+1)
        else:
            if type(runs) != int:
                raise TypeError("runs has to be list or int.")
            runs = [runs]
        self.runs = runs
        self.energy = energy
        self.focused = focused

        self.load_chains(indir)
        
        #deleting old canvas and surpressing root canvas creation message
        oldcanvas = r.gROOT.GetListOfCanvases().FindObject("defaultcanvas")
        if oldcanvas:
            oldcanvas.Close()
            del oldcanvas
        self.defaultcanvas = r.TCanvas(f"defaultcanvas") 

    def load_chains(self,indir):
        self.ch = {}
        for run in self.runs:
            self.ch[run] = load_chain([run],indir)
        if self.focused:
            self.rundict = {}
            self.runtitles = {}
            for run in self.runs:
                self.ch[run].GetEntry(self.ch[run].GetEntries()-1)
                file = self.ch[run].GetFile()
                amc, channel = getPMTfromFile(file)
                file.Close()
                if (amc, channel) not in self.rundict.keys():
                    self.rundict[amc,channel] = [run]
                    self.runtitles[amc,channel] = f"Run {run}"
                else:
                    self.rundict[amc,channel].append(run)
                    self.runtitles[amc,channel] += f", Run {run}"

    def plot_mosaic(self, histdict, titledict = None, legendstring = None, xlabel = None, ylabel = None, plotkwargs = {}):
        fig,ax = plt.subplot_mosaic(layout_string,figsize=(15,10), sharex=True, sharey=True)

        for (amc, channel),hist in histdict.items():
            axi = ax[channel_map[(amc,channel)]]
            if titledict:
                axi.set_title(titledict[amc,channel])

            hm = uproot.from_pyroot(hist.Clone("Clone")).to_hist()
            
            hm.plot(ax = axi, **plotkwargs)

            if xlabel != None:
                axi.set_xlabel("")
            if ylabel != None:
                axi.set_ylabel("")
            
            if legendstring:
                just_text_in_legend(axi,legendstring.format(amc = amc, channel = channel))

            #c = r.TCanvas()
            #histdict[amc, channel].Draw()
            #c.Draw()

        fig.tight_layout()
        return fig,ax
    
    def get_hists(self,run,dimensions,
                  name,title,hrange,nbins,
                  plotstring,filter):
        histdict = {}
        if run == "focused":
            if not self.focused:
                raise NotImplementedError("The rundata is not focused on a crystal")
            amcchannellist = self.rundict.keys()
        else:
            amcchannellist = np.array(np.meshgrid(amcs,channelTags)).T.reshape(-1,2)
        for (amc,channel) in amcchannellist:
            iname = f"{name}{amc},{channel}"
            oldhist = r.gDirectory.FindObject(iname)
            if oldhist:
                oldhist.Delete()
                del oldhist
            if dimensions == 1:
                histdict[amc, channel] = r.TH1F(iname,title,nbins,*hrange)
            elif dimensions == 2:
                histdict[amc, channel] = r.TH2F(iname,title,nbins,*hrange[0:2],nbins,*hrange[2:4])
            else:
                raise NotImplementedError("No histograms for dimension bigger 2")
            
            if run == "focused":
                runlist = self.rundict[amc,channel]
            else:
                runlist = [run]

            for runi in runlist:
                self.ch[runi].Draw(f"{plotstring}>>+{iname}",filter.format(amc = amc,channel = channel))

        return histdict

    def get_focused_hists(self,minh = 2e3,maxh = 32e3):        
        plotstring = "lyso_integrals.integral"
        filter = "lyso_integrals.amcNum == {amc} && lyso_integrals.channelTag == {channel}"

        histdict = self.get_hists("focused",1,
                                  "fhistogram","Histogram of lyso_integrals",(minh,maxh),50,
                                  plotstring,filter)
        
        return histdict
    
    def plot_focused_integrals(self, histdict = None):
        if not histdict:
            histdict = self.get_focused_hists()
        
        fig, ax = self.plot_mosaic(histdict,titledict = self.runtitles, 
                                   legendstring = "amc: {amc}\nchannel: {channel}",
                                   xlabel='Amplitude')
        
        return fig,ax
    
    def fit_crystalball(self,hist):
        s = r.TSpectrum(3)
        npeaksfound = s.Search(hist,2,"",0.10)
        xpeaks, ypeaks = [],[]
        for ipeak in range(npeaksfound):
            xposi,yposi = s.GetPositionX()[ipeak], s.GetPositionY()[ipeak]
            xpeaks.append(xposi)
            ypeaks.append(yposi)
            #print(ipeak,xposi)
        xmax, maxvalue = max(xpeaks), ypeaks[np.argmax(xpeaks)]
        minfit,maxfit = xmax-4000, xmax + 2000
        #gausfit = r.TF1("gausfit","gaus",xmax,maxfit)
        crytalballfit = r.TF1("crytalballfit","crystalball",minfit,maxfit)
        crytalballfit.SetParameters(maxvalue,xmax,1500,0.1,20000)

        #hist.Fit("gausfit", "R")
        hist.Fit("crytalballfit", "R")
        #Ng,m,sigma = gausfit.GetParameter(0), gausfit.GetParameter(1), gausfit.GetParameter(2)
        peak = crytalballfit.GetParameter(1)
        sigma = crytalballfit.GetParameter(2)
        
        xfit = np.linspace(minfit,maxfit)
        yfit = np.array([crytalballfit.Eval(xi) for xi in xfit])
        return xfit,yfit,peak,sigma,xpeaks,ypeaks


    def get_and_plot_calibration_constants(self):
        histdict = self.get_focused_hists()
        fig,ax = self.plot_focused_integrals(histdict)
        figtxt,axtxt = plt.subplot_mosaic(layout_string,figsize=(3,2.25), sharex=True, sharey=True)

        cconstantdict = {}
        peakpositiondict = {}
        sigmadict = {}

        #sort the dict so that the reference crystal is first
        amcref,channelref = 2,0
        sf = lambda item: (np.abs(item[0][0]-amcref),np.abs(item[0][1]-channelref))

        for (amc, channel),hist in sorted(histdict.items(),key = sf):

            fitresult = self.fit_crystalball(hist)
            xfit,yfit,peak,sigma,xpeaks,ypeaks = fitresult
            peakpositiondict[amc,channel] = peak
            sigmadict[amc,channel] = sigma
            try:
                cconstant = peak/peakpositiondict[2,0] #divide by the peak of the crystal 5
            except:
                print("No constant for reference crystal, can't calculate calibration constant")
                cconstant = 1
            cconstantdict[amc,channel] = cconstant

            axi = ax[channel_map[(amc,channel)]]
            axi.plot(xfit,yfit,'r')
            axi.scatter(xpeaks,ypeaks , marker='*', s=100, color='red')
            just_text_in_legend(axi,f"calibc: {cconstant:.3f} \namc: {amc}\nchannel: {channel}")

            axitxt = axtxt[channel_map[(amc,channel)]]
            axitxt.text(0.5, 0.5, f"{cconstant:.3f}",ha='center', va='center')
            axitxt.axis('off')

            square = RegularPolygon((0.5, 0.5), numVertices=4, radius=0.5, orientation = np.pi/4, alpha=0.4, edgecolor='k')
            axitxt.add_patch(square)

            cmap = plt.get_cmap('viridis')

            # Normalize the calibrationc value to map it to the colormap
            norm_cal = plt.Normalize(vmin=0.5, vmax=1.2)
            color = cmap(norm_cal(cconstant))
            square.set_facecolor(color)

            #c = r.TCanvas()
            #histdict[amc, channel].Draw()
            #c.Draw()

        figtxt.tight_layout(pad = 0,h_pad = 0, w_pad = 0)
        #plt.show()

        #print("peak postitions: ",peakpositiondict)
        #print("calibration constants: ",cconstantdict)
        return fig, ax, figtxt, axtxt, cconstantdict, peakpositiondict, sigmadict
    
    def write_cconstans_to_csv(self,cconstantdict,peakpositiondict,sigmadict,outdir = outdir, replaceFile = False):

        filename = 'cconstants.csv'
        filepath = outdir + filename

        #write header if file doesn't exist or should be replaced
        if not os.path.isfile(filepath) or replaceFile:
            with open(filepath, mode='w') as cconstants_file:
                header_writer = csv.writer(cconstants_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_NONNUMERIC)
                header_writer.writerow(["energy","runs","crystal","amc","channel","cconstant","peak","sigma"])

        with open(filepath, mode='a') as cconstants_file:
            cconstants_writer = csv.writer(cconstants_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_NONNUMERIC)
            for amc in amcs:
                for channel in channelTags:
                    if (amc, channel) in cconstantdict.keys():
                        cconstants_writer.writerow([self.energy,self.rundict[amc,channel],crystalmap[amc,channel],amc,channel,cconstantdict[amc,channel],peakpositiondict[amc,channel],sigmadict[amc,channel]])
    
    def plot_all_integrals(self,minh = 2e3, maxh = 32e3):
        #maxi = 0
        #for run in self.runs:
        #    maxi = self.ch[run].GetMaximum("lyso_integrals.integral")
        #    if maxi > maxint: maxint = maxi
        figl,axl = [],[]
        for run in self.runs:
            plotstring = "lyso_integrals.integral"
            filter = "lyso_integrals.amcNum == {amc} && lyso_integrals.channelTag == {channel}"

            histdict = self.get_hists(run,1,
                                      "ihist","integrals",(minh,maxh),100,
                                      plotstring,filter)

            fig,ax = self.plot_mosaic(histdict, xlabel = '')

            fig.suptitle(f"Run {run}", fontsize=20)
            figl.append(fig)
            axl.append(ax)
            #break
        return figl,axl
        
    def plot_linearity_hists(self,focused = None):
        if focused == None:
            focused = self.focused
        
        if focused:
            runs = ["focused"]
            if not self.focused:
                raise NotImplementedError("The data is not focused on a crystal. Please put focused to False.")
            titledict = self.runtitles
        else:
            figs,axs = [],[]
            runs = self.runs
            titledict = None

        for run in runs:
            plotstring = "lyso_integrals.integral:lyso_integrals.amplitude"
            filter = "lyso_integrals.amcNum == {amc} && lyso_integrals.channelTag == {channel}"
            histdict = self.get_hists(run,2,
                                    "lhistogram","linearity histogram",(0,800,0,50000),200,
                                    plotstring,filter)

            plotkwargs = {'norm': 'log', 'cbar' : False}        
            fig, ax = self.plot_mosaic(histdict,titledict = titledict, 
                                    xlabel='Amplitude', ylabel = 'Integral',
                                    plotkwargs = plotkwargs)
            
            if focused:
                figs,axs = fig,ax
            else:
                fig.suptitle(f"Run {run}", fontsize=20)
                figs.append(fig)
                axs.append(ax)
        return figs,axs

    def plot_hodo_hists(self):
        for run in self.runs:
            hhp_sum = None

            files = self.ch[run].GetListOfFiles()
            for TChainEl in files:
                current_file = r.TFile.Open(TChainEl.GetTitle())

                # Access the histogram from the file
                hist_hhp = current_file.Get("hists/positions/hodoscope_max_hit_channel_positions")
                
                try:
                    # If it's the first file, clone the histogram to sum_histogram
                    if not hhp_sum:
                        hhp_sum = hist_hhp.Clone("hhp_sum")
                        
                        #Don't delete the Histogram after closing the file
                        hhp_sum.SetDirectory(0)
                    else:
                        # Add the current histogram to sum_histogram
                        try:
                            hhp_sum.Add(hist_hhp)
                        except TypeError:
                            print(f"Warning: Histogram of {current_file} couldn't be added")
                except:
                    print("Some null pointer thing (bad file?)")

                # Close the current file
                current_file.Close()

            # Project onto the X-axis
            projection_x = hhp_sum.ProjectionX()

            # Project onto the Y-axis
            projection_y = hhp_sum.ProjectionY()

            hi = uproot.from_pyroot(hhp_sum).to_hist()
            hix = uproot.from_pyroot(projection_x).to_hist()
            hiy = uproot.from_pyroot(projection_y).to_hist()

            hi.view()[hi.view() == 0] = np.nan

            hi.plot()
            xlim = (-6,6)
            ylim = (-6,6)
            plt.title(f"Hit Position in Hodoscope, run {run}")
            plt.xlim(*xlim)
            plt.ylim(*ylim)
            plt.show()

            fig,ax = plt.subplots(2)
            hix.plot(ax = ax[0])
            ax[0].set_xlim(*xlim)
            ax[0].set_title("x projection")

            hiy.plot(ax = ax[1])
            ax[1].set_xlim(*ylim)
            ax[1].set_title("y projection")
            fig.tight_layout()
            plt.show()

            del hhp_sum
            del current_file