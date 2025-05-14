# Unpacker Library

A C++ library for unpacking midas data.

---

## Build Instructions

### Prerequisites
- [CMake]
- [ROOT]


### Building the Library

```bash
# Clone the repository
git clone https://github.com/sbfoster12/unpacker.git

# Build the respository
cd unpacker
./scripts/build.sh
```

## A simple example

### Setup the envirnoment
To configure the environment variables, navigate to the root of the unpacker repository and run:

```bash
source scripts/setenv.sh
```
This will set things like an `UNPACKER_PATH`, `ROOT_INCLUDE_PATH` and `LD_LIBRARY_PATH`. 

### Run the example
To run an example you will need a midas file. I assume you have such a file here `/PATH/TO/MIDAS/FILE`. Then do:

```bash
cd examples
root -q simple_unpacker.C+\(\”/PATH/TO/MIDAS/FILE\”,0\)
```
Running the `root` command will produce an output root file in the same directory that you ran the command. The second argument to `simple_unpacker.C` is the verbosity setting.

### Analyze the output in a jupyter notebook
Open the jupyter notebook `simple_analysis.ipynb` in the `examples` directory. I use VScode, so that looks like

```bash
code simple_analysis.ipynb
```

If you open the notebook from the same session where you ran the `setenv.sh` script, then the envirnoment variables should be usable by the notebook (these are needed to get the paths to the libraries). If this does not happen, you will need to manually set the `unpacker_path` variable in the notebook. This should be set to the path of the repository on your machine. You will also need to set the path to the root file that you previously made.

The notebook is simple. It produces some diagnostic information and a plot of the traces for all the channels that were active.