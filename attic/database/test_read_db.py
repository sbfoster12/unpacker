import os
import sys 
# import psycopg2
import sqlalchemy
import pandas 

# conn = psycopg2.connect(
#     host="localhost",
#     database="pioneer_online",
#     user="pioneer_reader",
#     # password="Abcd1234"
# )

# df = pandas.read_sql('select * from online order by run_number desc;', con=conn)
# print(df.head())

db_url = sqlalchemy.URL.create(
    "postgresql",
    username="pioneer_reader",
    # password="kx@jj5/g",  # plain (unescaped) text
    host="localhost",
    database="pioneer_online",
)
print(db_url)
reader_engine = sqlalchemy.create_engine(db_url)
print(reader_engine)

command = "select * from slow_control limit 10;"
with reader_engine.connect() as connection:
    df = pandas.read_sql(command, connection)
print(df.head())