#!/bin/bash

# this is a dangerous script. What we need to do is:
# Find the list of the oldest files in the online directory
# if that file exists on data1 and data2, and the hashes of all 3 files agree
# then we can delete the original file

#!/bin/bash

# Directory names
dir1="/home/pioneer/pioneer/pioneer-teststand/nearline"
# dir2="/data1/online"
dir3="/data2/nearline"

# Loop over every file in the 'online' directory
for file in "$dir1"/*root
do
  # Only process files, not directories
  if [ -f "$file" ]
  then
    # Get the base name of the file
    base=$(basename "$file")

    file3="$dir3/$base"

    if [ -f "$file3" ]
    then
        # Compute the SHA-256 hash of the files in each directory
        hash1=$(sha256sum "$file" | awk '{ print $1 }')
        # hash2=$(sha256sum "$file2" | awk '{ print $1 }')
        hash3=$(sha256sum "$file3" | awk '{ print $1 }')

        # Compare the hashes
        if  [ "$hash1" == "$hash3" ]
        then
            echo "Hashes match for $base -> we can delete."
            echo "   -> $hash1 | $hash3"
            echo "   -> $file | $file3"
            rm -fv $file
        else
            echo "Warning: hashes for $base did not match..."
        fi
    fi
  fi
done
