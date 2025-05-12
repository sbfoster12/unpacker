import os
import sys

input_file = sys.argv[1]
assert os.path.exists(input_file)

with open(input_file, 'r') as fin:
    lines = fin.readlines()
line = lines[2].split(",")
this_run = int(line[1])
this_id  = int(line[0])
print(this_run, this_id)

update_command = f'''UPDATE channel_mapping SET run_stop = {this_run-1} WHERE configuration_id = {this_id-1};'''
print(update_command)

load_command = f'''COPY channel_mapping(configuration_id, run_start, run_stop, amcNum, wfd5, channel,  \
    crystal_id, pmt_id, high_voltage_id, hv_cable_label,  \
    signal_cable_label, channel_type, channel_id) \
    FROM '{input_file}' \
    DELIMITER ',' \
    CSV HEADER;'''

for ci in [update_command, load_command]:
    psql_command = f'''psql -d pioneer_online -c "{ci}" '''

    # print(load_command)
    print(psql_command)
    os.system(psql_command) 