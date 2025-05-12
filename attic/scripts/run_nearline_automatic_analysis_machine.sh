#!/bin/bash

# source ~/.bashrc
# cd /home/pioneer/pioneer/pioneer-teststand/packages/experiment/gm2daq/environment_setup
# source setup_environment.sh -a
# export PATH=/home/pioneer/pioneer/pioneer-teststand/packages/psql/postgresql-16.1/install/bin:$PATH
# source /home/pioneer/pioneer/pioneer-teststand/packages/ROOT/root_install/bin/thisroot.sh
eval "$(/home/pioneer/dqm/miniconda/bin/conda shell.bash hook)"
conda activate pioneer_nearline

DAQ_MIDAS_OUTPUT_DIR="/data2/online"
DAQ_NEARLINE_OUTPUT_DIR="/data2/nearline"
# DAQ_NEARLINE_OUTPUT_DIR="/home/pioneer/pioneer/pioneer-teststand/nearline"
DAQ_LOG_OUTPUT_DIR="/home/pioneer/pioneer/pioneer-teststand/nearline_logs"

MIDAS_OUTPUT_DIR="/home/pioneer/dqm/online"
NEARLINE_OUTPUT_DIR="/home/pioneer/dqm/nearline"
LOG_OUTPUT_DIR="/home/pioneer/dqm/nearline_logs"

DETECTOR_MAPPING_FILE="detector_mapping.json"
VERBOSITY=0
PROCESS_DIR="/home/pioneer/dqm/unpacking/bin"

cd $PROCESS_DIR
echo "Running from:" $PROCESS_DIR

rsync -avh pioneer@daq:${DAQ_LOG_OUTPUT_DIR} ${LOG_OUTPUT_DIR}

file_list_new=`ssh pioneer@daq find "${DAQ_MIDAS_OUTPUT_DIR}" -type f -mmin +1 -mmin -30 -name '*.mid.lz4' | shuf`
file_list_old=`ssh pioneer@daq find "${DAQ_MIDAS_OUTPUT_DIR}" -type f -mmin +30 -name '*.mid.lz4' | shuf`
echo New files: $file_list_new
echo Old files: $file_list_old
file_list=" $file_list_new $file_list_old "
echo "Found files:" $file_list

currentTime=$(date +%s) 
maxTime=$(($currentTime + 60*5 )) 

for i in $file_list;
do
    thisTime=$(date +%s)
	echo "processing file: $i at time $thisTime / $maxTime"
    #break out of the loop if it has lasted too long
    if [ $thisTime -gt $maxTime ]; then
        # send_to_slack "Warning on g2nearline1: Exceeded maximum time for one script. Stopping here."
	    echo "Timeout. All done."
        break
    fi

    NGM2_1=$(ps aux | grep "pioneer_unpac" | wc -l)
    NGM2=$((NGM2_1 - 1))
    echo 'Total nearline processes:' $NGM2

    if [ ${NGM2} -gt 3 ]; then
        echo "More than allowed number of nearline instances. Skip this file."
        continue
    fi

    FILE=$(echo ${i} | cut -d/ -f4)    
    echo 'Filename:' $FILE

    numbers=($(echo $FILE | grep -oP '\d{5}'))
    RUN_STR=${numbers[0]}
    SUBRUN_STR=${numbers[1]}

    RUN=$((10#${numbers[0]}))
    SUBRUN=$((10#${numbers[1]}))

    echo 'Run number:' $RUN / $RUN_STR
    echo 'Subrun number:' $SUBRUN / $SUBRUN_STR

    ROOT_FILE=${NEARLINE_OUTPUT_DIR}/nearline_hists_run${RUN_STR}_${SUBRUN_STR}.root
    LOG_FILE=${LOG_OUTPUT_DIR}/nearline_run${RUN_STR}_${SUBRUN_STR}.log

    if [ -f ${LOG_FILE} ]; then
        echo "File ${LOG_FILE} exists, not going to process twice!"
        continue
    fi

    LOCAL_COPY="/home/pioneer/dqm/online/${FILE}"
    rsync -avh pioneer@daq:$i $LOCAL_COPY

    SIZE=$(du -m $LOCAL_COPY | cut -f1)
    echo 'Size:' $SIZE

    MIDAS_FILE_DATE=`stat -c '%.26z' $LOCAL_COPY`
    echo 'Creation date:' $MIDAS_FILE_DATE

    echo "pioneer_unpacker $LOCAL_COPY $VERBOSITY $DETECTOR_MAPPING_FILE $ROOT_FILE" 
    time ./pioneer_unpacker $LOCAL_COPY $VERBOSITY $DETECTOR_MAPPING_FILE $ROOT_FILE > $LOG_FILE 2>&1
    NEARLINE_EXIT=$?
    echo "Process exited with status ${NEARLINE_EXIT}"
    echo "Processed ${i} to ${ROOT_FILE}"
    echo "     -> ${LOG_FILE}"
    CURRENT_TIME=`date`

    # update the psql database
    psql_string="INSERT INTO nearline_processing(run_number, subrun_number, nearline_processed, midas_file_date, \
        midas_filesize, process_date, midas_file_location, nearline_file_location, nearline_log_location, \
        nearline_exit_status) \
        VALUES( $RUN, $SUBRUN, true, '$MIDAS_FILE_DATE', $SIZE, CURRENT_TIMESTAMP, \
        '${LOCAL_COPY}', '${ROOT_FILE}', '${LOG_FILE}', ${NEARLINE_EXIT} );"
    echo $psql_string >> ${LOG_FILE}
    echo $psql_string 
    psql -h localhost -U pioneer_writer -d pioneer_online -c "$psql_string" >> ${LOG_FILE} 

    rsync -avh $LOG_FILE pioneer@daq:$DAQ_LOG_OUTPUT_DIR
    rsync -avh $ROOT_FILE pioneer@daq:$DAQ_NEARLINE_OUTPUT_DIR
    rm -f $LOCAL_COPY

    # break;

done 

echo "All done!"
