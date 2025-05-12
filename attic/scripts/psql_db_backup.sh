#!/bin/bash
source ~/.bashrc
cd /home/pioneer/pioneer/pioneer-teststand/packages/experiment/gm2daq/environment_setup
source setup_environment.sh -a

output_dir="/data2/database_backup"
# output_dir="/home/jlab/github/test-beam-2023-unpacker/scripts"
time=$(date +%s)
file_name="db_backup_pioneer_online_${time}.tar"

export PGPASSWORD="pioneer_reader" 
/home/pioneer/pioneer/pioneer-teststand/packages/psql/postgresql-16.1/install/bin/pg_dump -U pioneer_reader -F t pioneer_online > ${output_dir}/${file_name}
echo "Saved to: ${output_dir}/${file_name}"
