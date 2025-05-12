import subprocess
import glob
import os
import sys
import pprint

def get_run_from_file(file):
    # Split the file path by '/' and get the last element
    filename = file.split('/')[-1]

    # Split the filename by 'run' and get the second part
    run_number = filename.split('run')[1].split('.')[0]

    return int(run_number)

def get_mapping_file_from_run(run):
    return f'detector_mapping/mapping_run{run}.csv'

def get_calibration_file_from_run(run):
    return f'energy_calibration/calibration_run{run}.csv'

def get_run_ranges(file_paths):

    run_numbers = []

    for file_path in file_paths:
        run_numbers.append(get_run_from_file(file_path))

    run_numbers = list(set(run_numbers))
    run_numbers.sort()

    run_ranges = []
    
    for i in range(len(run_numbers) - 1):
        run_ranges.append((run_numbers[i], run_numbers[i + 1]-1))
    run_ranges.append((run_numbers[len(run_numbers)-1],1000000000))

    return run_ranges

#Get the csv files
mapping_files=glob.glob(get_mapping_file_from_run("*"))
calibration_files=glob.glob(get_calibration_file_from_run("*"))
file_paths=mapping_files+calibration_files

#Get the run ranges
run_ranges_mapping = get_run_ranges(mapping_files)
run_ranges_calibration = get_run_ranges(calibration_files)
run_ranges = get_run_ranges(file_paths)

run_range_file_map = {}

#loop over run ranges
for run_range in run_ranges:
    this_mapping_file=''
    this_calibration_file=''

    #mapping
    found_file=False
    #loop over mapping files
    for run_range_mapping in run_ranges_mapping:

        if ( (run_range[0] <= run_range_mapping[1]) & (run_range_mapping[0] <= run_range[1])):
            this_mapping_file=get_mapping_file_from_run(run_range_mapping[0])
            found_file=True
            break

    if not found_file:
        this_mapping_file=''

    #calibration
    found_file=False
    #loop over mapping files
    for run_range_calibration in run_ranges_calibration:

        if ( (run_range[0] <= run_range_calibration[1]) & (run_range_calibration[0] <= run_range[1])):
            this_calibration_file=get_calibration_file_from_run(run_range_calibration[0])
            found_file=True
            break

    if not found_file:
        this_calibration_file=''

    run_range_file_map[run_range] = (this_mapping_file,this_calibration_file)

# print(run_range_file_map)
print("These are the run ranges:")
pprint.pprint(run_range_file_map)
input("Press Enter to continue...")

#Make the config files for each run range
for key in run_range_file_map:
    run=key[0]
    mapping_file=run_range_file_map[key][0]
    calibration_file=run_range_file_map[key][1]

    if not os.path.exists(mapping_file):
        continue;

    if not os.path.exists(calibration_file):
        continue;

    # mapping_file, calibration_file get_files_from_run(run,mapping_files,calibration_files)
    os.system(f'cp nearline_config.json nearline_config_run{run}.json')
    os.system(f'python3 update_mapping_from_csv.py {mapping_file} nearline_config_run{run}.json')
    os.system(f'python3 update_calibration_from_csv.py {calibration_file} nearline_config_run{run}.json')

    input("Press Enter to continue...")