run=$1 
#rsync -avh --progress pidaq:/home/pioneer/pioneer/pioneer-teststand/online/*${run}_*mid.lz4 .
get_midas_runs $1
for num in `seq -w 00000 00500`;
do
    FILE=./run00${run}_${num}.mid.lz4
    echo "Looking for file: $FILE"
    if [ -f "$FILE" ]; then
        echo "File $FILE exists. Processing"
        time ./pioneer_unpacker $FILE 0 ./detector_mapping.json > output.log 2>&1
    else 
        echo "File $FILE does not exist."
        break;
    fi
done
