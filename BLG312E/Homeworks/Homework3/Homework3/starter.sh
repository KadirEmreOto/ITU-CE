#!/bin/bash
gcc applicant.c -o applicant
chmod +x applicant
gcc hr.c -o hr -pthread
chmod +x hr
gcc trial.c -o trial -pthread
chmod +x trial
./applicant $2
./hr $1 &
./trial $1 &
