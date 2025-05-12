daq_alarm_hook="https://hooks.slack.com/services/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"
message=$1
data={"text":\"${message}\"}
echo Posting with json data: $data
curl -X POST -H 'Content-type: application/json' \
    --data "${data}" \
    ${daq_alarm_hook}