# from datetime import datetime, timedelta
import pandas
import numpy as np 
import os

base = {
    'TEMP':30,
    'VOLT':1100,
}

reftime = pandas.to_datetime("2023-10-22 10:00:00")
for channel in range(10):
    channel_id = f'example_channel_{channel:02}'
    for reading_type in ['TEMP', 'VOLT']:
        for offset in range(0,10000,30):
            current_run = 200 + offset//500
            time = reftime + pandas.Timedelta(seconds=offset+np.random.randint(-5,5))
            reading = base[reading_type] + np.random.random()*base[reading_type]/10.
            print(channel_id, current_run, reading_type, time,reading)
            command = f'''
psql -d 'pioneer_online' -U 'pioneer_writer' -c "INSERT INTO slow_control \
    VALUES( '{time.strftime('%Y-%m-%d %H:%M:%S')}', {current_run}, '{channel_id}', '{reading_type}', {reading}  )   \
    ON CONFLICT DO NOTHING;"
            '''
            print(command)
            os.system(command)

    # for channel in range(10):


# time = 
# channel_id = 
# reading_type = 
# reading = 
# command = f'''psql -d 'pioneer_online' -U 'pioneer_writer' -c "INSERT INTO online VALUES({run}, '{ding['Stop Time']}',' {ding['Comment']}', '{ding['Stop Time']}', NULL, '{ding['Start Time']}', NULL, '{ding['Shifters']}', NULL, NULL, NULL, NULL, '{ding['Type']}' )   ON CONFLICT DO NOTHING;" '''
