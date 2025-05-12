#!/bin/bash
#
#  send warning to slack if disk space on the nearline machine is low
#
CAPACITY_LIMIT=$1

source ~/.bash_profile
cd /home/daq/database/

if [ "$CAPACITY_LIMIT" == "" ]
then
    CAPACITY_LIMIT=98   # default limit
fi

#directory is hardcoded in here as /datalocal
SPACEUSED=$(df -k /datalocal | awk '{gsub("%",""); capacity=$3}; END {print capacity}')
CAPACITY=$(df -k /datalocal | awk '{gsub("%",""); capacity=$5}; END {print capacity}')

echo "Disk is " $CAPACITY " % full. Limit is " $CAPACITY_LIMIT "."

echo `date "+%s"` $CAPACITY $SPACEUSED >> diskSpaceTracker.log 

if [ $CAPACITY -gt $CAPACITY_LIMIT ]
then
    echo $(echo "Disk space on g2nearline1 is low (" $CAPACITY " % Full)")
    send_to_slack "<@U97R9JUGN> Disk space on g2nearline1 is low ( ${CAPACITY}% Full). Alert limit set to >${CAPACITY_LIMIT}%."
    # send_to_slack "<@U97R9JUGN> Disk space on g2nearline1 is low ( ${CAPACITY}% Full). Alert limit set to >${CAPACITY_LIMIT}%. Attempting to resolve automatically..."
    # /usr/bin/python /home/daq/artDev_run4/utilities/disk_space/clear_space.py  
fi

