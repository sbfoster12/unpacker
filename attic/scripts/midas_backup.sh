#!/bin/bash

# script to run periodacally and back up the data from midas to the drives /data{1,2}/
# keep things in sync between the drives manually, rather than in a raid configuration,
#      in case we need to change to having the two be seperate
# J. LaBounty

MIDAS_OUTPUT_DIR="/home/pioneer/pioneer/pioneer-teststand/online"
NEARLINE_DIR="/home/pioneer/pioneer/pioneer-teststand/nearline"
NEARLINE_LOG_DIR="/home/pioneer/pioneer/pioneer-teststand/nearline_logs"
MIDAS_COPY_DIR="/data1/online/"
MIDAS_COPY_DIR_2="/data2/online/"
NEARLINE_COPY_DIR="/data2/"

NEARLINE_DIR="/home/pioneer/pioneer/pioneer-teststand/nearline"
NEARLINE_LOG_DIR="/home/pioneer/pioneer/pioneer-teststand/nearline_logs"
NEARLINE_COPY_DIR="/data2/"

# only search for files which are > 30 minutes old
# file_list=`find "${MIDAS_OUTPUT_DIR}" -type f -mmin +30`
file_list=`find "${MIDAS_OUTPUT_DIR}" -type f -mmin +30 -mmin -600`
echo $file_list
echo $file_list | wc -l

# echo rsync -avh ${file_list} $MIDAS_COPY_DIR
time rsync -avh ${file_list} $MIDAS_COPY_DIR
time rsync -avh $MIDAS_COPY_DIR $MIDAS_COPY_DIR_2

time rsync -avh $NEARLINE_DIR $NEARLINE_COPY_DIR
time rsync -avh $NEARLINE_LOG_DIR $NEARLINE_COPY_DIR
