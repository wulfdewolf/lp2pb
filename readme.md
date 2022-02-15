# lp2pb

Lp2pb is a translation tool that translates ground ASP (lparse smodels) to PB-constraints (.OPB).


## Manual (linux)

### Compilation

The translator consists of multiple c++ files.
Install 'make' and call it inside the root folder.
```shell
make
```

### Executables

This tool makes use of 3 other tools to translate its input; [lp2sat, lp2lp ](http://www.tcs.hut.fi/Software/lp2sat/) and [lp2normal](https://research.ics.aalto.fi/software/asp/lp2normal/).  
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
OR add following line to ~/.bashrc:
```shell
export PATH="/home/$USER/bin:$PATH"
```
and restart your terminal or call:
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

W. De Wulf, B. Bogaerts. LP2PB: Translating Answer Set Programs into Pseudo-Boolean Theories. ICLP Technical Communications 2020: 206-219
