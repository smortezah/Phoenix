#include <iostream>
#include <fstream>

#include "messages.h"
#include "def.h"

using std::cout;


/*******************************************************************************
    show usage guide
*******************************************************************************/
void Messages::help () const
{
    cout                                                                << '\n'
       << "NAME"                                                        << '\n'
       << "      Phoenix v " << VERSION                                 << '\n'
       << "      A compression-based method "                           << '\n'
                                                                        << '\n'
       << "AUTHORS"                                                     << '\n'
       << "      Morteza Hosseini    seyedmorteza@ua.pt"                << '\n'
       << "      Diogo Pratas        pratas@ua.pt"                      << '\n'
       << "      Armando J. Pinho    ap@ua.pt"                          << '\n'
                                                                        << '\n'
        << "SYNOPSIS"                                                   << '\n'
        << "      ./phoenix [OPTION]... -m [MODELS] "
                                       "-r [REFERENCES] -t [TARGETS]"   << '\n'
                                                                        << '\n'
        << "SAMPLE"                                                     << '\n'
        << "      ./phoenix -n 4 -d -m 0,11,100:1,9,10 "
                                   "-r HS21,HSY -t PT19,PT21,PTY"       << '\n'
                                                                        << '\n'
        << "OPTIONS"                                                    << '\n'
        << "      -h,  --help"                                          << '\n'
        << "           usage guide"                                     << '\n'
                                                                        << '\n'
        << "      -m [<i1>,<k1>,<a1>:<i2>,<k2>,<a2>:...],  "
                                         "--model [<i1>,<k1>,<a1>:...]" << '\n'
        << "           context models -- MANDATORY (at least one)"      << '\n'
        << "           <i>:  inverted repeat (1=apply, 0=do not apply)" << '\n'
        << "           <k>:  context-order size"                        << '\n'
        << "           <a>:  1/alpha"                                   << '\n'
        << "           EXAMPLE: -m 0,11,100:1,9,10"                     << '\n'
                                                                        << '\n'
        << "      -r [REFERENCES],  --reference [REFERENCES]"           << '\n'
        << "           reference files -- MANDATORY (at least one)"     << '\n'
        << "           EXAMPLE: -r ref1,ref2"                           << '\n'
                                                                        << '\n'
        << "      -t [TARGETS],  --target [TARGETS]"                    << '\n'
        << "           target files -- MANDATORY (at least one)"        << '\n'
        << "           EXAMPLE: -t tar1,tar2,tar3"                      << '\n'
                                                                        << '\n'
        << "      -d,  --decompress"                                    << '\n'
        << "           decompress"                                      << '\n'
                                                                        << '\n'
        << "      -n [INTEGER],  --nthreads [INTEGER]"                  << '\n'
        << "           number of threads (default: 2)"                  << '\n'
                                                                        << '\n'
        << "      -g [FLOAT],  --gamma [FLOAT]"                         << '\n'
        << "           gamma (default: 0.95)."                          << '\n'
        << "           requires a float number (0 <= g < 1)"            << '\n'
                                                                        << '\n'
        << "      -v,  --verbose"                                       << '\n'
        << "           verbose mode (more information)"                 << '\n'
                                                                        << '\n'
        << "COPYRIGHT"                                                  << '\n'
        << "      Copyright (C) " << DEV_YEARS << ", IEETA, University "
                                                           "of Aveiro." << '\n'
        << "      This is a Free software, under GPLv3. You may redistribute\n"
        << "      copies of it under the terms of the GNU - General Public  \n"
        << "      License v3 <http://www.gnu.org/licenses/gpl.html>. There  \n"
        << "      is NOT ANY WARRANTY, to the extent permitted by law." << '\n';
    
    exit(1);
}


/*******************************************************************************
    show verbose mode
*******************************************************************************/
void Messages::verbose () const
{
    cout << "verbose mode.\n";  // TODO: for test
}
