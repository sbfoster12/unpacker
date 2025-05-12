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

detector_mapping = {}
if ('detectors' in config) :
    detector_mapping = config['detectors']

#loop over mapping and update the ones in the input config
for i, row in df.iterrows():

    detectorName = row['detectorName']
    detectorNum = row['detectorNum']

    #skip those with -1 slot for the mapping
    if row['slot'] == -1:
        continue;

    #first check that this detector exists in the current file
    if (detectorName not in detector_mapping):
        detector_mapping[detectorName] = []

    #loop over the detectors for this type
    entryExists=False
    for detector in detector_mapping[detectorName]: 
        if ( (detector['detectorName'] == detectorName)
            & (detector['detectorNum'] == detectorNum)):
            entryExists=True
            detector['crateNum'] = 0
            detector['amcSlotNum'] = row['slot']
            detector['channelNum'] = row['channel']
            detector['x'] = row['x']
            detector['y'] = row['y']

    #if there isn't an existing entry, make a new one
    if not entryExists:
        #didn't find it, so add a new entry
        new_entry={
            "detectorName": row['detectorName'],
            "detectorNum": row['detectorNum'],
            "crateNum": 0,
            "amcSlotNum": row['slot'],
            "channelNum": row['channel'],
            'x': row['x'],
            'y': row['y']
        }

        position_mapping[detectorName].append(new_entry)

config['detectors'] = detector_mapping 
print("Updating detector mapping in", output_file)
with open(output_file,'w') as fout:
   json.dump(config, fout, indent=2)