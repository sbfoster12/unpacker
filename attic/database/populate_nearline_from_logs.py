import os
import sys
import numpy as np 
import pandas
from datetime import datetime 

BASE_DIR = '/home/jlab/testbeam_example_files/nearline/'
LOG_DIR = '/home/jlab/testbeam_example_files/nearline_logs/'

files = [os.path.join(LOG_DIR,x) for x in os.listdir(LOG_DIR) if '.log' in x[-5:]]
for i, fi in enumerate(files):
    try:
        run = int(fi.split('_run')[-1].split('_')[0])
        subrun = int(fi.split('_')[-1].split('.log')[0])
    except:
        # print(fi)
        continue

    if run < 310:
        continue
    nl = fi.replace(LOG_DIR, BASE_DIR).replace('.log',".root").replace("nearline_run", 'nearline_hists_run')
    mtime = datetime.fromtimestamp(os.path.getmtime(fi)).strftime('%Y-%m-%d %H:%M:%S')
    command = f'''psql -d 'pioneer_online' -U 'pioneer_writer' -c \
"INSERT INTO nearline_processing \
(run_number, subrun_number, nearline_processed, process_date, nearline_file_location, nearline_log_location) \
VALUES({run}, {subrun}, {os.path.exists(nl)}, '{mtime}',' {nl if os.path.exists(nl) else "NULL"}', '{fi}' ) ON CONFLICT DO NOTHING;" '''
    print(command)
    os.system(command)
    # if(os.path.exists(nl)):
    #     print(i, fi, run, subrun, nl)