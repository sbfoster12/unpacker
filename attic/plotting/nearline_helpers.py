import ROOT as r
import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import uproot 
import hist
import matplotlib
import xml.etree.ElementTree as ET

r.gSystem.Load("../lib/libdataProducts.so")
r.gSystem.Load("../lib/libunpackers.so")

#r.gSystem.Load("/home/pioneer/kolja/test-beam-2023-unpacker/lib/libdataProducts.so")
#r.gSystem.Load("/home/pioneer/kolja/test-beam-2023-unpacker/lib/libunpackers.so")


def just_text_in_legend(ax,text):
    handles = [matplotlib.patches.Rectangle((0, 0), 1, 1, fc="white", ec="white", lw=0, alpha=0)]
    labels = [text]
    ax.legend(handles, labels, handlelength=0,handletextpad=0)

def get_nearline_directory():
    match os.uname()[1]:
        case 'SB3':
            return '../bin/'
        case 'pioneer-nuci':
            return '/home/pioneer/dqm/nearline/'
        case _:
            raise NotImplementedError

    

def load_chain(runs, path='../bin/', run_string='RUN'):
    ch = r.TChain("tree")
    for run in runs:
        pathi = os.path.join(path,f'*{run}_*root')
        ch.Add(pathi)
    return ch

class LoopCut:
    def __init__(self, t, cut=''):
        self.n = t.Draw('Entry$', cut,"goff")
        self.i = 0
        self.t = t
        print("Found", self.n, 'matches')
    
    def __iter__(self):
        return self 
    
    def __next__(self):
        if(self.i < self.n):
            self.t.GetEntry(int(self.t.GetV1()[self.i]))
        else:
            raise StopIteration
        self.i += 1

def plot_waveform(wf,ax=None, label = None):
    if(ax is None):
        fig,ax = plt.subplots()
    else:
        fig = plt.gcf()
    plt.sca(ax)
        
    trace = np.array(wf.trace)
    samples = range(len(trace))
    peak_location = samples[list(trace).index(np.amax(trace))]
    ped = np.full_like(samples, wf.pedestalLevel)
    plt.plot(samples,trace,label = label) #color='C0', 
    plt.plot([peak_location, peak_location], [wf.pedestalLevel, np.amax(trace)], ':', color='xkcd:grey')
    plt.plot(samples,ped, color='C2')
    plt.fill_between(samples, ped-wf.pedestalStdev, ped+wf.pedestalStdev, alpha=0.2, color='C2')
    title = f'Run/Subrun: {wf.runNum}/{wf.subRunNum}\nAMC Number: {wf.amcNum}\nChannel: {wf.channelTag}\nEvent: {wf.eventNum}'
    if not ax.get_legend() and label == None:
        just_text_in_legend(ax,title) 
    elif label != None:
        ax.legend(title=title)
    return fig,ax

def draw_waveform(w):
    fig,ax = plt.subplots()
    x = w.raw.GetObject()
    ding = np.array(x.trace, dtype=np.short)

    base = np.average(ding[:10])
    # if np.amax(ding) > base + 5:
    print(x)
    print(base, np.amax(ding))
    label=f'{x.crateNum=}\n{x.amcNum=}\n{x.channelTag=}\n{x.eventNum=}\n{w.fullintegral=}\n{x.pedestalLevel=}\n{x.pedestalStdev=}'
    plt.plot(ding,label=label)

    # c = x.DrawWaveform(x.MakeTGraph())
    # c.Draw()

    plt.legend()
    plt.grid()
    # plt.show()

    return fig,ax

def plot_integral(w,ax=None,subtract_pedestal=False,
                    color_offset=0,plot_amplitude=True, plot_restricted=True, 
                    plot_pedestal=True, plot_full=True, draw_legend=True
):
    if(ax is None):
        fig,ax = plt.subplots()
    else:
        fig = plt.gcf()
    # plt.sca(ax)
        
    wf = w.raw.GetObject()
    offset = w.pedestalLevel if subtract_pedestal else 0.0
    trace = np.array(wf.trace, dtype=float) - offset
    samples = range(len(trace))
    peak_location = samples[list(trace).index(np.amax(trace))]
    ped = np.full_like(samples, wf.pedestalLevel, dtype=float)-offset
    if(plot_full):
        ax.plot(samples,trace,color=f'C{color_offset+0}', label=f'Full Integral: {w.fullintegral:.2f}')
    if(plot_restricted):
        cut = np.full_like(trace, False)
        cut[w.integration_window.first:w.integration_window.second] = True
        ax.plot(samples,np.where(cut, trace, np.nan), color=f'C{color_offset+1}', label=f'CH{w.channelTag}: Rest. Integral: {w.integral:.2f}')
    if(plot_amplitude):
        ax.plot([peak_location, peak_location], [wf.pedestalLevel-offset, np.amax(trace)], ':', color='xkcd:grey', label=f'Amplitude: {w.amplitude:.2f}')
    if(plot_pedestal):
        ax.plot(samples,ped, color=f'C{color_offset+2}')
        ax.fill_between(samples, ped-wf.pedestalStdev, ped+wf.pedestalStdev, alpha=0.2, color=f'C{color_offset+2}')
    title = f'Run/Subrun: {wf.runNum}/{wf.subRunNum}\nAMC Number: {wf.amcNum}\nChannel: {wf.channelTag}\nEvent: {wf.eventNum}'
    if(draw_legend):
        ax.legend(title=title)
    # plt.show()
    return fig,ax

def plot_pulse_fit(inti, f,ax = None):
    if(ax is None):
        fig,ax = plt.subplots()
    else:
        fig = plt.gcf()
    plt.sca(ax)
    wf = inti.waveform.GetObject()
    spline = f.Get(f"splines/spline_{inti.amcNum}_{inti.channelTag}")
    print(wf,spline)
    trace = np.array(wf.trace)
    samples = range(len(trace))
    ped = np.full_like(samples, wf.pedestalLevel)
    plt.plot(samples,trace,color='C0')
    plt.plot(inti.times, np.array(inti.amplitudes)+inti.pedestalLevel, "r*")
    ding = np.full_like(samples,inti.pedestalLevel,dtype=float)
    label = '$\chi^{2}=$'+f'{inti.chi2:.2f}\nConverged: {bool(inti.converged)}\n'
    for i, ti in enumerate(inti.times):
        ai = inti.amplitudes[i]
        label += f'Pulse {i+1} : t={ti:.2f} | A={ai:.2f}\n'
        # thisamp = np.array([inti.amplitudes[i]*spline.Eval(x-inti.times[i]) if (x-inti.times[i] >spline.GetXmin() and x-inti.times[i] < spline.GetXmax()) else 0.0 for x in samples ])
        thisamp = np.array([inti.amplitudes[i]*spline.Eval(x-inti.times[i]) for x in samples ])
        plt.plot(samples, thisamp+ped, color=f'C{i+3}')
        ding += thisamp
    plt.plot(samples,ding, 'C1',label=label[:-1])
    # plt.grid(
    # )
    plt.legend()
    return fig,ax

channel_map = {
    (1,0):'A',
    (1,1):'B',
    (1,2):'C',
    (1,3):'D',
    (1,4):'E',
    (2,0):'F',
    (2,1):'G',
    (2,2):'H',
    (2,3):'I',
    (2,4):'J',
}
layout_string = '.AABBCC.\nDDEEFFGG\n.HHIIJJ.'

def plot_event(integrals,channel_map=channel_map,layout=layout_string):
    # fig,ax = plt.subplots(2,3,figsize=(15,7))
    fig,ax = plt.subplot_mosaic(layout,figsize=(15,10), sharex=True, sharey=True)
    energy = 0
    for inti in integrals:
        # if(inti.amcNum != 1):
        #     continue
        amc = inti.amcNum
        channel = inti.channelTag
        axi = ax[channel_map[(amc,channel)]]
        plot_integral(inti, axi)
        energy += inti.integral
    plt.suptitle(f'Total integral: {energy:.2f}')
    plt.tight_layout()
    # plt.show()
    return fig,ax

layout_stringp = '.AABBCC.\nDDEEFFGG\nZHHIIJJ.'

def plot_event_wf(waveforms,channel_map=channel_map,layout=layout_stringp):
    # fig,ax = plt.subplots(2,3,figsize=(15,7))
    fig,ax = plt.subplot_mosaic(layout,figsize=(15,10), sharex=True, sharey=True)
    energy = 0
    for wfi in waveforms:
        # if(wfi.amcNum != 1):
        #     continue
        amc = wfi.amcNum
        channel = wfi.channelTag
        axi = ax[channel_map[(amc,channel)]]
        plot_waveform(wfi, axi)
        # energy += inti.integral
    # plt.suptitle(f'Total integral: {energy:.2f}')
    plt.tight_layout()
    # plt.show()
    return fig,ax

