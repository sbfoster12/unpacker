#!/bin/bash

# this is a dangerous script. What we need to do is:
# Find the list of the oldest files in the online directory
# if that file exists on data1 and data2, and the hashes of all 3 files agree
# then we can delete the original file

#!/bin/bash

# Directory names
dir1="/home/pioneer/pioneer/pioneer-teststand/online"
dir2="/data1/online"
dir3="/data2/online"

# Loop over every file in the 'online' directory
for file in "$dir1"/*mid.lz4
do
  # Only process files, not directories
  if [ -f "$file" ]
  then
    # Get the base name of the file
    base=$(basename "$file")

    file2="$dir2/$base"
    file3="$dir3/$base"

    if [ -f "$file2" ] && [ -f "$file3" ]
    then
        # Compute the SHA-256 hash of the files in each directory
	      echo "Found $file / $file2 / $file3"
	      # echo `sha256sum "$file"`
        hash1=$(sha256sum "$file" | awk '{ print $1 }')
        hash2=$(sha256sum "$file2" | awk '{ print $1 }')
        hash3=$(sha256sum "$file3" | awk '{ print $1 }')

        # Compare the hashes
        if [ "$hash1" == "$hash2" ] && [ "$hash1" == "$hash3" ]
        then
            echo "Hashes match for $base -> we can delete."
            echo "   -> $hash1 | $hash2 | $hash3"
            rm -fv $file
        else
            echo "Warning: hashes for $base did not match..."
        fi
    fi
  fi
done
