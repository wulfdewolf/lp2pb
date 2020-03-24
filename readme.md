# lp2pb

Lp2pb is a translation tool that translates ground ASP (smodels) to PB-constraints (.opb).


## Manual (linux)

### Compilation

The translator consists of multiple c++ files.
Install 'make' and call it inside the root folder.
```shell
make
```

### Executables

This tool makes use of 3 other tools to translate its input; lp2sat and lp2lp ([see](http://www.tcs.hut.fi/Software/lp2sat/), lp2normal([see](https://research.ics.aalto.fi/software/asp/lp2normal/))  
The executables for these should be in your PATH.  
By calling:
```shell
make path
```
the latest versions (at the moment) are added to your ~/bin folder.   
Make sure this folder is included in your PATH variable. 
```shell
export PATH=~/bin:$PATH
```
or add following line to ~/.bashrc:
```shell
export PATH="/home/$USER/bin:$PATH"
```
restart your terminal or call:
```shell
source ~/.bashrc
```

If you would want another version for any of these tools you can put them in your PATH yourself using the correct names:
- lp2lp2
- lp2normal
- lp2sat

### Usage

The translator expects smodels input, see Appendix B [here](http://www.tcs.hut.fi/Software/smodels/lparse.ps.gz).

To run the application:
```
./lp2pb <-i inputfile> <-o outputfile>
```
Input and output files are optional, if not provided standard input and output is used.

### Citation
