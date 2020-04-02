#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:40:00
#PBS -l pmem=16gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

../../runlim -o runlim_INSTANCE_SETUP.txt -r 1500 -s 14336 SCRIPT ARG >> solver_INSTANCE_SETUP.txt


## LOCK
exec {lock_fd}>../../lock/mylockfile || exit 1
flock -n "$lock_fd"
 
# solution
if cat solver_INSTANCE_SETUP.txt | grep -E "OPTIMUM";
then 
    printf "1," >> ../../result.csv
elif cat solver_INSTANCE_SETUP.txt | grep "UNSAT";
then
    printf "2," >> ../../result.csv
elif cat solver_INSTANCE_SETUP.txt | grep "SAT";
then 
    printf "3," >> ../../result.csv
elif cat solver_INSTANCE_SETUP.txt | grep "UNKNOWN";
then
    printf "4," >> ../../result.csv
else 
    printf "5," >> ../../result.csv
fi

# outcode 
cat runlim_INSTANCE_SETUP.txt | sed -n -e 's/^.*result //p' | tr -d \\n >> ../../result.csv
printf "," >> ../../result.csv 

# time
cat runlim_INSTANCE_SETUP.txt | sed -n -e 's/^.*real //p' | tr -d \\n >> ../../result.csv
printf "," >> ../../result.csv

# status
cat runlim_INSTANCE_SETUP.txt | sed -n -e 's/^.*status //p' | tr -d \\n >> ../../result.csv
printf "," >> ../../result.csv

# memusage
cat runlim_INSTANCE_SETUP.txt | sed -n -e 's/^.*space //p' | tr -d \\n >> ../../result.csv
printf "," >> ../../result.csv

# setup
printf "SETUP," >> ../../result.csv;

# problem family
printf '%s' "FAMILY," >> ../../result.csv;

# instance
printf "INSTANCE\n" >> ../../result.csv;
 
## UNLOCK
flock -u "$lock_fd"

# remove temp files
rm runlim_INSTANCE_SETUP.txt
rm solver_INSTANCE_SETUP.txt
