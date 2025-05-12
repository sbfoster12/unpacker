import os
import sys
import pandas
import json
import time
import numpy as np

input_file = sys.argv[1]
assert os.path.exists(input_file)

if(len(sys.argv) > 2):
    output_file = sys.argv[2]
else:
    output_file = '../python/detector_mapping.json'

with open(output_file,'r') as fin:
    config = json.load(fin)
print(config)

this_time =f'{int(time.time())}'
os.system(f'cp {output_file} {output_file.replace(".json","_archive_"+this_time+".json")}')

df = pandas.read_csv(input_file)
print(df.head())

detector_types = df['channel_type'].unique()
print(f'{detector_types=}')
new_mapping = {x:[] for x in detector_types}
for i, row in df.loc[~df['channel_type'].isna()].iterrows():
    # print(row['channel_type'])
    # if np.isnan(row['channel_type']):
    #     continue
    new_mapping[row['channel_type']].append(
        {
            'x':row['channel_x'],
            'y':row['channel_y'],
            'detectorName':row['channel_type'],
            'detectorNum':int(row['channel_id']),
            'crateNum':row['amcNum'],
            'amcSlotNum':row['wfd5'],
            'channelNum':row['channel'],
        }
    )



config['detectors'] = new_mapping 
with open(output_file,'w') as fout:
    json.dump(config, fout, indent=2)
