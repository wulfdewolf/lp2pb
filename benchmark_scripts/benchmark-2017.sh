#!/bin/bash

for F in ./instances/*.lp;
do 
{   
    #1
    # gringo | clasp
    sed 's|CALL|'"gringo encoding.asp $F -o smodels \| ..\/..\/runlim -t 1500 -s 14336 ..\/..\/clasp"'|g' ../scripts/benchmark.sh >> benchmark1_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| clasp/g" benchmark1_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark1_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark1_${F##*/}.pbs;
    #qbus benchmark1_${F##*/}.pbs;

    #2
    # gringo | lp2pb | roundingsat
    sed 's|CALL|'"gringo encoding.asp $F -o smodels \| ..\/..\/lp2pb \| ..\/..\/runlim -t 1500 -s 14336 ..\/..\/roundingsat"'|g' ../scripts/benchmark.sh >> benchmark2_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| lp2pb \| roundingsat/g" benchmark2_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark2_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark2_${F##*/}.pbs;
    #qbus benchmark2_${F##*/}.pbs;

    #3
    # gringo | lp2normal | lp2lp2 | lp2sat | roundingsat
    sed 's|CALL|'"gringo encoding.asp $F -o smodels \| ..\/..\/lp2normal \| ..\/..\/lp2lp2 \| ..\/..\/lp2sat \| ..\/..\/runlim -t 1500 -s 14336 ..\/..\/roundingsat"'|g' ../scripts/benchmark.sh >> benchmark3_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| lp2normal \| lp2lp2 \| lp2sat \| roundingsat/g" benchmark3_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark3_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark3_${F##*/}.pbs;
    #qbus benchmark3_${F##*/}.pbs;

    #4
    # gringo | lp2normal | lp2lp2 | clasp
    sed 's|CALL|'"gringo encoding.asp $F -o smodels \| ..\/..\/lp2normal \| ..\/..\/lp2lp2 \| ..\/..\/runlim -t 1500 -s 14336 ..\/..\/clasp"'|g' ../scripts/benchmark.sh >> benchmark4_${F##*/}.pbs;
    sed -i "s/SETUP/gringo \| lp2normal \| lp2lp2 \| clasp/g" benchmark4_${F##*/}.pbs;
    sed -i "s|FAMILY|${PWD##*/}|g" benchmark4_${F##*/}.pbs;
    sed -i "s|INSTANCE|${F##*/}|g" benchmark4_${F##*/}.pbs;
    #qbus benchmark4_${F##*/}.pbs;
}
done
