#!/bin/bash
sudo killall -s SIGINT "applicant"
sudo killall -s SIGINT "hr"
sudo killall -s SIGINT "trial"
pkill -9 "applicant"
pkill -9 "hr"
pkill -9 "trial"
ipcs -S | grep KEO | cut -f3 -d' ' | xargs -I {} sudo ipcrm -S {}
