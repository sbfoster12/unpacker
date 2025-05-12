#!/bin/bash

echo "Beginning hadd process..."
source ~/.bashrc
cd /home/pioneer/pioneer/pioneer-teststand/packages/experiment/gm2daq/environment_setup
source setup_environment.sh -a

source /home/pioneer/pioneer/pioneer-teststand/packages/ROOT/root_install/bin/thisroot.sh

cd /home/pioneer/pioneer/pioneer-teststand/packages/unpacking/scripts
echo "Running python script..."
python -u hadd_iteratively.py | tee hadd.log

echo "All done!"
