#!/bin/bash

# for 2017 benchmarks --> instances in subfolder, no selected instances file

for F in ./instances/*.lp;
do 
{   
    #1
    # gringo | clasp
    sed 's|SCRIPT|..\/..\/call-clasp.sh|g' ../../benchmark_scripts/rbenchmark.sh >> benchmark1_${F##*/}.pbs;
    sed -i 's|ARG|'"$F"'|g' benchmark1_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| clasp/g" benchmark1_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark1_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark1_${F##*/}.pbs;
    qbus benchmark1_${F##*/}.pbs;

    #2
    # gringo | lp2pb | roundingsat
    sed 's|SCRIPT|..\/..\/call-with-lp2pb.sh|g' ../../benchmark_scripts/rbenchmark.sh >> benchmark2_${F##*/}.pbs;
    sed -i 's|ARG|'"$F"'|g' benchmark2_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| lp2pb \| roundingsat/g" benchmark2_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark2_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark2_${F##*/}.pbs;
    qbus benchmark2_${F##*/}.pbs;

    #3
    # gringo | lp2normal | lp2lp2 | lp2sat | roundingsat
    sed 's|SCRIPT|..\/..\/call-without-lp2pb.sh|g' ../../benchmark_scripts/rbenchmark.sh >> benchmark3_${F##*/}.pbs;
    sed -i 's|ARG|'"$F"'|g' benchmark3_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| lp2normal \| lp2lp2 \| lp2sat \| roundingsat/g" benchmark3_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark3_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark3_${F##*/}.pbs;
    qbus benchmark3_${F##*/}.pbs;

    #4
    # gringo | lp2normal | lp2lp2 | clasp
    sed 's|SCRIPT|..\/..\/call-clasp-simple.sh|g' ../../benchmark_scripts/rbenchmark.sh >> benchmark4_${F##*/}.pbs;
    sed -i 's|ARG|'"$F"'|g' benchmark4_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| lp2normal \| lp2lp2 \| clasp/g" benchmark4_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark4_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark4_${F##*/}.pbs;
    qbus benchmark4_${F##*/}.pbs;
}
done
