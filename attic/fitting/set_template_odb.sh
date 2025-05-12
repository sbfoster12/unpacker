#!/bin/bash
# set -x

# length="800" #nominal setting
length="3600" #template setting
# length="800000" #extended setting, max we could do...

for num in `seq 1 2`; do

  rider=`printf Rider%02d $num`
  odbedit -e DAQ -c "set \"/Equipment/AMC1300/Settings/${rider}/Board/Async CBuf Waveform Length\" $length"
done

# set +x