import sys
import os 
import json 
import numpy as np 
import pandas

# ONLINE_DIR = '/home/jlab/testbeam_example_files/online/'
# BASE_DIR = '/home/jlab/testbeam_example_files/nearline/'
# LOG_DIR = '/home/jlab/testbeam_example_files/nearline_logs/'

ONLINE_DIR = '/home/pioneer/pioneer/pioneer-teststand/online/'
BASE_DIR   = '/home/pioneer/pioneer/pioneer-teststand/nearline/'
LOG_DIR    = '/home/pioneer/pioneer/pioneer-teststand/nearline_logs/'

def process_odb_json_for_runlog(fi):
    with open(fi,'r') as fin:
        odb = json.load(fin)
    # print(odb)
    runinfo = odb['Runinfo']
    output = {}
    try:
        output = {
            'Run':        runinfo['Run number'] ,
            'Start Time': pandas.to_datetime(runinfo['Start time']).strftime('%Y-%m-%d %H:%M:%S') ,
            'Stop Time':  pandas.to_datetime(runinfo['Stop time']).strftime('%Y-%m-%d %H:%M:%S') ,
        }
        output['NSubruns'] = int(odb['Logger']['Channels']['0']['Settings']['Current filename'].split('_')[-1].split('.mid')[0])
        output['Type']     = odb['Experiment']['Run Parameters']['Run Type']
        output['Comment']  = odb['Experiment']['Run Parameters']['Comment']
        output['Shifters'] = odb['Experiment']['Run Parameters']['Operator']
    except:
        # print('Warning: Unable to fully process:', fi)
        pass
        # continue
    return output

files = [os.path.join(ONLINE_DIR, x) for x in os.listdir(ONLINE_DIR) if '.json' in x[-7:] and 'last' not in x]
for i, fi in enumerate(files):
    run = int(fi.split('run')[1].split('.json')[0])
    if run < 300:
        continue
    ding = process_odb_json_for_runlog(fi)
    print(ding)
    command = f'''psql -d 'pioneer_online' -U 'pioneer_writer' -c "INSERT INTO online VALUES({run}, '{ding['Stop Time']}',' {ding['Comment']}', '{ding['Stop Time']}', NULL, '{ding['Start Time']}', NULL, '{ding['Shifters']}', NULL, NULL, NULL, NULL, '{ding['Type']}' )   ON CONFLICT DO NOTHING;" '''
    print(command)
    os.system(command)