# lp2pb

a translation tool that translates ASP programs to PB constrains


## Manual (linux)

### Compilation

The translator consists of multiple c++ files.
Install 'make' and call it inside the root folder.
```shell
make
```

### Usage

The translator expects smodels input, [see here](http://www.tcs.hut.fi/Software/smodels/lparse.ps.gz).

To run the application:
```
./lp2pb <-i inputfile> <-o outputfile>
```
Input and output files are optional, if not provided standard input and output is used.

### Executables

### Citation