# unpacker/nearline for 2023 PSI lyso test beam

This repository contains the unpacker and nearline code used in the 2023 PSI lyso test beam.

The unpacker takes the raw midas files and parses the CR midas data bank to extract the digitized waveforms. 

The nearline runs the unpacker and does some reconstruction using these digitized waveforms. It produces a root file with a ROOT TTree. Each entry in the tree corresponds to a single event where each event is one triggered readout of all active channels. The tree contains branchs of collections of dataProducts or single dataProducts. The dataProduct are custom C++ classes that derive from ROOT's TObject.

So far the nearline reconstruction involves these steps:

* **Correct even/odd jitter** in waveforms, which arises from using two interleaved 400 MSPS digitizers to 800 MSPS, and each digitizer can have a different pedetal
* **Integrate waveforms** to extract waveform integral and amplitude. TODO: fill in algorithm details
* **Energy/time calibrattion** as of 11/21/2023, calibration is still in progress
* **Pileup tagging** TODO: fill in algorithm details
* **Lyso clustering** TODO: fill in algorithm details
* **Make hodoscope events** in two modes: 1) max_x and may_x integrals and 2) weighted x and y integrals

# Update history

November 23, 2023

* Change default branch from main to develop
* Add new member variable to Hodoscope data product to flag negative integral events (containsNegativeIntegrals)
* Update detector_mapping.json configuration file to grab energy calibration by detecetorName and detectorNum now
* Put in hodoscope calibration constants based on calibration Run 546
* Clean up serializer class to better handle the configuration file. Still need to work on getting it saved in the file

# Dependencies 

You will need the following packages

* ROOT: https://root.cern/install/
* ZLIB: https://www.zlib.net
* eigen: https://eigen.tuxfamily.org/index.php?title=Main_Page
* boost: https://www.boost.org

# Building/installing

Clone the repository: https://github.com/PIONEER-Experiment/test-beam-2023-unpacker

The default branch is develop.

To build:

```console
mkdir build  
cd build  
cmake ..  
```

To install:

```console
make install
```

# Developing new features

When developing new features for this repository, please try to follow these guidelines.

The default branch for the repository is develop, so you'll want to implement your changes on a feature branch and then merge those changes into develop. So, first, make a new feature branch off of develop where you'll implement your changes:

```console
git checkout develop
git branch feature/YOUR-FEATURE
```

Push your local branch to remote to setup a remote branch:

```console
git push origin feature/YOUR-FEATURE
```

Implement your new feature and once you are happy, you can submit a pull request or merge into develop:

```console
git checkout develop
git merge feature/YOUR-FEATURE
```

Resolve any merge conflicts and push your new feature to develop:
```console
git push
```

Clean up your feature branch and make an archive (instructions below are taken from here: https://gist.github.com/zkiraly/c378a1a43d8be9c9a8f9)

Create a tag:
```console
git tag archive/YOUR-FEATURE YOUR-FEATURE
```

Delete the feature branch:
```console
git branch -d YOUR-FEATURE
```

Push the branch deletion to origin:
```console
git push origin :YOUR-FEATURE
```

Push the tags:
```console
git push --tags
```

If you need to recover a tag, you can checkout the tag using:
```console
git checkout -b feature/YOUR-FEATURE archive/YOUR-FEATURE
```

# Running the nearline
To run the nearline over a midas file, do the following:

```console
./pioneer_unpacker MIDAS_FILE.mid.lz4 0 detector_mapping.json
```

* pioneer_unpacker is located in the repositories bin directory
* MIDAS_FILE.mid.lz4 is the midas file you are running over
* 0 is the unpacker verbosity
* detector_mapping.json is the configuration file and is located in the python directory. Be careful that use the correct detector_mapping.json file for a given run range (as of 11/21/2023, making these run specific files is a work in progress)

# Doing analysis on the ROOT files
To do analysis on the ROOT files, you will need access to the dataProducts library. Here are a couple of use cases

## Interactive ROOT session

You can run the following lines or put them in an environment setup script:

export UNPACKER_PATH=/path/to/test-2023-unpacker
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${UNPACKER_PATH}/lib
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:${UNPACKER_PATH}/src/dataProducts

When you run root interactively, you should now have access to all the dataProducts, both the member variables and the methods.

## In python

```python
import ROOT as r  
r.gSystem.Load("/path/to/library")
```
# Nearline file contents

* `vector<dataProducts::ChannelConfig> channelConfigs`
* `dataProducts::ODB odb`
* `TProcessID ProcessID0`
* `TTree    tree`
* `TTree    tree`
* `TDirectoryFile hists`
* `TString  config`

The tree contains the following branches:

* WFD5Headers
* channelHeaders
* waveformHeaders
* lyso_waveforms
* nai_waveforms
* t0_waveform
* hodo_x_waveforms
* hodo_y_waveforms
* veto_waveform
* rf
* rf_waveforms
* monitor_waveforms
* other_waveforms
* lyso_integrals
* nai_integrals
* t0_integral
* hodo_x_integrals
* hodo_y_integrals
* veto_integral
* monitor_integrals
* other_integrals
* lyso_integrals_calibrated
* hodo_x_integrals_calibrated
* hodo_y_integrals_calibrated
* lyso_integrals_seeded
* pileup_tag
* clusters
* clusters_seeded
* hodoscope
* hodoscope_calibrated

# Data products

All the dataProducts can be found in the src/dataProduct directory. Here is a list of the important data products, detailing their member variables and methods.

### DataProduct

Description

* Base class for all data products, derives from ROOT's TObject

Member variables

* None

Methods

### ChannelConfig

Description

* Configuration of the detector to digitizer mapping, pedestal values for each digitizer, and energy & time calibration

Member variables

* `std::string detectorName;`
* `int detectorNum;`
* `int crateNum;`
* `int amcSlotNum;`
* `int channelNum;`
* `double x;`
* `double y;`
* `double pedestal;`
* `double nSigmaIntegration;`
* `std::tuple<int,int> presamplesWindowSize;`
* `double integralSizeCutoff;`
* `double timingCalibration;`
* `double timingShift;`
* `bool isNonLinear;`
* `dataProducts::LinearCalibrationHolder linearCalib;`
* `dataProducts::NonLinearCalibrationHolder nonLinearCalib;`
* `bool doEnergyCalibration;`

Methods

* `double GetTimingCalibration()`
* `double GetTimingShift()`
* `void Show() const override`

### odb

Description

* Contains the ODB dump in the Begin Of Run (BOR) in the midas file. This corresponds to midas event_id=32768 (see bottom of https://daq00.triumf.ca/MidasWiki/index.php/Event_Structure)

Member variables

* `std::string odbString`: ODB dump stored in xml
* `int runNumber`: midas run number
* `int subRunNumber`: midas sub run number
* `std::map<int,int> digitizationFrequencies`: waveform digitization frequencies (slot number-> digitization frequency in MHz)
* `int xStage`: x position of the stage (no conversion from ODB parameter)
* `int xStage`: y position of the stage (no conversion from ODB parameter)

Methods
* `void ParseODB()`: parse the ODB string and fills the classes member variables

### Waveform

Description

* Stores the digitized traces for a particular channel

Member variables

* `int runNum`
* `int subRunNum`

* `int crateNum` : crate number is always 0 because we only have one crate
* `int amcNum` : the slot number (1->12) in the crate (i.e. which digitizer)
* `int channelTag`: the channel (0->4) for this digitizer (each WFD5 has 5 channels)

* `int eventNum`: midas serial_number
* `int waveformIndex`: waveform index within this event, always 0 in the circular buffer mode, but in the async mode will go from 0 up to number of hardware triggers per readout trigger

* `unsigned int length`: length of the trace

* `double pedestalLevel`: pedestal level of the trace
* `double pedestalStdev`: standard deviation of the trace's pedestal

* `int x`: x position of detector
* `int y`: y position of detector
* `bool is_clipping`: flag if the waveform exceeds the max ADC value 

* `int firstSampleNum`: first sample number of the trace

* `unsigned long clockCounter`: 40 MHz TTC trigger timestamp, relative to run start


* `int preTriggerLength`: number of samples taken before the trigger 

* `std::vector<short> trace`: digitized waveform

* `std::string detectorType`: name of the detector, e.g. "lyso", "hodo_x", "hodo_y", etc.
* `int detectorNum`: detector number, e.g. 0->9 for lyso "reading like a book" if you are the beam

* `TRef raw`: ROOT TRef pointing to a waveform from which this waveform was created

Methods

* `void CheckClipping()`:
* `void JitterCorrect(int offset)`:
* `void CalculatePedestal(int nsamples)`:
* `double PeakToPeak()`:

* `ChannelID GetID()`: gets channel ID defines as (crateNum, slot, channel)

## Coming soon..

### WaveformIntegral

Description

Member variables

Methods

### WaveformFit

Description

Member variables

Methods

### IntegralCluster

Description

Member variables

Methods

### RFWaveform

Description

* Contains information about the RF. Does a phase fit assuming that the frequency is fixed at 50.6 MHz (configurable in `detector_mapping.json`). The raw phase is stored in the `phase` variable, while the one which is relative to `T0` is stored in `corrected_phase`.

Member variables

* frequency [`double`] : The RF frequency, fixed by the config file
* phase [`double`] : The uncorrected phase of the RF waveform relative to the 0th sample of the trace
* amplitude [`double`] : The amplitude of the fitted RF signal
* pedestalLevel [`double`] : any offset of the signal from 0
* corrected_phase [`double`] : The phase of the RF signal relative to the peak time of the T0 pulse
* t0_in_trigger_window [`double`] : Whether the peak time of the T0 pulse was found in the expected integration window (from `seeded_integration.trigger_low/trigger_high`)

Methods
* IdentifyPhase : Methods to identify the phase with a sine fit
* FitSine : Methods to identify the phase with a sine fit
* CorrectPhaseT0 : Corrects the phase of the RF signal relative to the peak of a specified waveform.

