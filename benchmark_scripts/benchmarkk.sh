#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:30:00
#PBS -l pmem=14gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0


# args: time-limit space-limit command resfile 

# memory limit is in MB
tlim=$1
let hardtlim=3+$tlim
let mlim=1024*$2
let hardmlim=$mlim+50000

command=$3
res_file=$4
err_file="$res_file.err"

hash=`echo -n $res_file | md5sum | awk '{ print $1 }'`

if [ $tlim -gt 0 ]; then
  ulimit -Ht $hardtlim -St $tlim # NOTE: hard limit apparently needs to be first argument!? -Ht $hardtlim
fi
if [ $mlim -gt 0 ]; then
  ulimit -Hv $hardmlim -Sm $mlim # NOTE: hard limit apparently needs to be first argument!? -Hv $hardmlim
fi

# Note the use of the tmp file, to avoid problems with time writing to $res_file
(/usr/bin/time --format="command: %C\ncpu time: %U\nmax memory: %M\n-----OUTPUT-----" --output=$res_file $command 2> $err_file) 2>&1 > /tmp/$hash
ret=$?
cat /tmp/$hash >> $res_file
rm /tmp/$hash
exit $ret