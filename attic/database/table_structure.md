```psql
CREATE TABLE nearline_processing(
    run_number int,
    subrun_number int,
    nearline_processed bool,
    midas_file_date timestamp,
    midas_filesize float,
    start_event_number int,
    end_event_number int,
    process_date timestamp,
    midas_file_location varchar(200),
    nearline_file_location varchar(200),
    nearline_log_location varchar(200),
    nearline_exit_status int,
    PRIMARY KEY (run_number, subrun_number)
);

CREATE TABLE online(
    run_number int PRIMARY KEY,
    time timestamp,
    comment varchar(300),
    Stoptime timestamp,
    totalBytes int,
    Starttime timestamp,
    diskLevel real,
    crew varchar(200),
    dataDir varchar(200),
    nEvents int,
    nBytes int,
    nFiles int,
    quality varchar(5)
);

CREATE TABLE channel_mapping(
    configuration_id int,
    run_start int,
    run_stop int,
    amcNum int,
    wfd5 int,
    channel int,
    crystal_id int,
    pmt_id int,
    high_voltage_id int,
    hv_cable_label varchar(100),
    signal_cable_label varchar(100),
    channel_type varchar(20),
    channel_id varchar(200),
    PRIMARY KEY (configuration_id, run_start, amcNum, wfd5, channel)
);

CREATE TABLE slow_control(
    time timestamp,
    current_run int,
    channel_id varchar(200),
    reading_type varchar(80),
    reading float
);
```