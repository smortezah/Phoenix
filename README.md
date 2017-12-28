<p align="center"><img src="img/logo.png" alt="Phoenix" width="264" border="0" /></p>
<br>

Phoenix is ...

## INSTALL
Get Phoenix and make the project, using:
```bash
git clone https://github.com/smortezah/Phoenix.git
cd Phoenix
cmake .
make
```


## RUN
```commandline
./phoenix [OPTION]... -m [MODELS] -r [REFERENCES] -t [TARGETS]
```


### OPTIONS
To see the possible options, type:
```bash
./phoenix -h
```
which provides the following:
```
SYNOPSIS
      ./phoenix [OPTION]... -m [MODELS] -r [REFERENCES] -t [TARGETS]

SAMPLE
      ./phoenix -n 4 -d -m 0,11,100:1,9,10 -r HS21,HSY -t PT19,PT21,PTY

OPTIONS
      -h,  --help
           usage guide

      -m [<i1>,<k1>,<a1>:<i2>,<k2>,<a2>:...],  --model [<i1>,<k1>,<a1>:...]
           context models -- MANDATORY (at least one)
           <i>:  inverted repeat (1=apply, 0=do not apply)
           <k>:  context-order size
           <a>:  1/alpha
           EXAMPLE: -m 0,11,100:1,9,10

      -r [REFERENCES],  --reference [REFERENCES]
           reference files -- MANDATORY (at least one)
           EXAMPLE: -r ref1,ref2

      -t [TARGETS],  --target [TARGETS]
           target files -- MANDATORY (at least one)
           EXAMPLE: -t tar1,tar2,tar3

      -d,  --decompress
           decompress

      -n [INTEGER],  --nthreads [INTEGER]
           number of threads (default: 2)

      -g [FLOAT],  --gamma [FLOAT]
           gamma (default: 0.95).
           requires a float number (0 <= g < 1)

      -v,  --verbose
           verbose mode (more information)
```


## CITE
Please cite the following papers, if you use Phoenix:
* M. Hosseini, D. Pratas and A.J. Pinho, "On the role of inverted repeats in DNA sequence similarity," *11'th International Conference on Practical Applications of Computational Biology & Bioinformatics (PACBB)*, Springer, June 2017.


## ISSUES
Please let us know if there is any [issues](https://github.com/smortezah/Phoenix/issues).


## LICENSE
Phoenix is under GPL v3 license. For more information, click 
[here](http://www.gnu.org/licenses/gpl-3.0.html).