run=$1 
file_dir="/home/pioneer/pioneer/pioneer-teststand/online"

for num in `seq -w 00000 00500`;
do
    FILE=${file_dir}/run00${run}_${num}.mid.lz4
    OUTFILE=./run00${run}_${num}.root
    echo "Looking for file: $FILE"
    if [ -f "$FILE" ]; then
        echo "File $FILE exists. Processing"
        ./pioneer_unpacker $FILE 0 ./detector_mapping.json $OUTFILE > process_local.log 2>&1
    else 
        echo "File $FILE does not exist."
        break;
    fi
done