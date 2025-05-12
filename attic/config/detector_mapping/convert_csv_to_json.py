import json
import csv
import sys
import os

csv_file = sys.argv[1]
assert os.path.exists(csv_file)

json_file = os.path.splitext(csv_file)[0]+".json"

data = []
with open(csv_file, 'r') as csvfile:
    csvreader = csv.DictReader(csvfile)
    for row in csvreader:
        data.append(row)

with open(json_file, 'w') as jsonfile:
    json.dump(data, jsonfile, indent=2)