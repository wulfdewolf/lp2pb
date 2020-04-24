#!/usr/bin/env bash

#PBS -l nodes=1:ppn=1:ivybridge
#PBS -l walltime=00:25:00
#PBS -l pmem=15gb

cd $PBS_O_WORKDIR

module purge
module load libreadline/8.0-GCCcore-8.3.0

# Cores take a lot of space so don't make them
ulimit -c 0

printf "time,space,outcode,status,solution,SETUP,FAMILY,INSTANCE\n" > result_INSTANCE_SETUP.csv;
../../runlim -r 1203 -s 12288 -o "$TMPDIR"/run_INSTANCE_SETUP.txt SCRIPT ARG > "$TMPDIR"/out_INSTANCE_SETUP.txt

# time
time=$(cat "$TMPDIR"/run_INSTANCE_SETUP.txt | grep -oP '(?<=real ).*');
sed -i 's|time|'"$time"'|g' result_INSTANCE_SETUP.csv;

# space
space=$(cat "$TMPDIR"/run_INSTANCE_SETUP.txt | grep -oP '(?<=space ).*');
sed -i 's|space|'"$space"'|g' result_INSTANCE_SETUP.csv;

# outcode
outcode=$(cat "$TMPDIR"/run_INSTANCE_SETUP.txt | grep -oP '(?<=result ).*');
sed -i 's|outcode|'"$outcode"'|g' result_INSTANCE_SETUP.csv;

# status
status=$(cat "$TMPDIR"/run_INSTANCE_SETUP.txt | grep -oP '(?<=status ).*');
sed -i 's|status|'"$status"'|g' result_INSTANCE_SETUP.csv;

if echo "$status" | grep ok;
# FINISHED
then 
    if cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep "UNSAT";
    then
        sed -i "s|solution|1|g" result_INSTANCE_SETUP.csv;
    elif cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep "OPTIMUM";
    then 
        # Roundingsat
        if echo SETUP | grep 2 || echo SETUP | grep 3 || echo SETUP | grep 4 || echo SETUP | grep 5;
        then 
            opt=$(cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep -oP '(?<=o )[0-9]+');
            sed -i 's|solution|2\/'"$opt"'|g' result_INSTANCE_SETUP.csv;
        # Clasp
        else 
            opt=$(cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization : )\w+');
            sed -i 's|solution|2\/'"$opt"'|g' result_INSTANCE_SETUP.csv;
        fi
    elif cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep "SAT";
    then 
        sed -i "s|solution|3|g" result_INSTANCE_SETUP.csv;
    elif cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep "UNKNOWN";
    then
        sed -i "s|solution|4|g" result_INSTANCE_SETUP.csv;
    else 
        sed -i "s|solution|6|g" result_INSTANCE_SETUP.csv;
    fi
# STOPPED
elif cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep 'bounds' || cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization: )\w+';
then 
    if echo SETUP | grep 2 || echo SETUP | grep 3 || echo SETUP | grep 4 || echo SETUP | grep 5;
    then 
        opt=$(cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep -oP '(?<=c bounds ).*(?=>=)' | tail -1 | tr -d " ")
        sed -i 's|solution|3\/'"$opt"'|g' result_INSTANCE_SETUP.csv;
    else
        opt=$(cat "$TMPDIR"/out_INSTANCE_SETUP.txt | grep -oiP '(?<=Optimization: )\w+' | tail -1);
        sed -i 's|solution|3\/'"$opt"'|g' result_INSTANCE_SETUP.csv;
    fi
else sed -i "s|solution|5|g" result_INSTANCE_SETUP.csv;
fi

# remove temp files
rm "$TMPDIR"/out_INSTANCE_SETUP.txt
rm "$TMPDIR"/run_INSTANCE_SETUP.txt