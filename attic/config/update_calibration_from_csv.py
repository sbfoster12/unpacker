import os
import sys
import pandas as pd
import json
import time
import numpy as np

input_file = sys.argv[1]
assert os.path.exists(input_file)

if(len(sys.argv) > 2):
    output_file = sys.argv[2]
else:
    output_file = 'nearline_config.json'

with open(output_file,'r') as fin:
    config = json.load(fin)
# print(config)

# this_time =f'{int(time.time())}'
# os.system(f'cp {output_file} {output_file.replace(".json","_archive_"+this_time+".json")}')

df = pd.read_csv(input_file)
# print(df.head())

calib_mapping = {}
if ('calibration' in config) :
    calib_mapping = config['calibration']


#loop over calibrations
for i, row in df.iterrows():

    detectorName = row['detectorName']
    detectorNum = row['detectorNum']

    #first check that this detector exists in the current file
    if (detectorName not in calib_mapping):
        calib_mapping[detectorName] = []

    #loop over the detectors for this type
    entryExists=False
    for detector in calib_mapping[detectorName]: 
        if ( (detector['detectorName'] == row['detectorName'])
            & (detector['detectorNum'] == row['detectorNum'])):
            entryExists=True
            detector['energy_calib'] = row['energy_calib']
            detector['time_calib'] = row['time_calib']
            detector['time_offset'] = row['time_offset']

    #if there isn't an existing entry, make a new one
    if not entryExists:
        #didn't find it, so add a new entry
        new_entry={
            "detectorName" : row['detectorName'],
            "detectorNum" : row['detectorNum'],
            "energy_calib" : row['energy_calib'],
            "time_calib" : row['time_calib'],
            "time_offset" : row['time_offset']
        }
        calib_mapping[detectorName].append(new_entry)

config['calibration'] = calib_mapping 
print("Updated calibratin in", output_file)
with open(output_file,'w') as fout:
    json.dump(config, fout, indent=2)
