#!/bin/bash
# set -x

for num in 1 2 3 4 5 6 9 10; do

  rider=`printf Rider%02d $num`
  odbedit -e DAQ -c "set \"/Equipment/AMC1300/Settings/${rider}/Board/Enabled\" true "
done


for num in `seq 1 2`; do

  rider=`printf Rider%02d $num`
  odbedit -e DAQ -c "set \"/Equipment/AMC1300/Settings/${rider}/Board/Enabled\" true "
done

# set +x