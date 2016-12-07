#include <iostream>
#include <fstream>

#include "messages.h"
#include "def.h"


/***********************************************************
    constructor
************************************************************/
Messages::Messages () {}


/***********************************************************
    show usage guide
************************************************************/
void Messages::help () const
{
    std::cout                                                                   << "\n"
            << "Synopsis:"                                                      << "\n"
            << "    phoenix [OPTION]... -m [MODELs] -t [TARGET] -r [REFERENCE]" << "\n"
                                                                                << "\n"
            << "Options:"                                                       << "\n"
            << "    -h,  --help"                                                << "\n"
            << "         usage guide"                                           << "\n"
                                                                                << "\n"
            << "    -V,  --version"                                             << "\n"
            << "         version number"                                        << "\n"
                                                                                << "\n"
            << "    -v,  --verbose"                                             << "\n"
            << "         verbose mode (more information)"                       << "\n"
                                                                                << "\n"
            << "    -m [<rt>,<s>,<a>,<i>:...],  --model [<rt>,<s>,<a>,<i>:...]" << "\n"
            << "         context model(s). Seperate different models with :"    << "\n"
            << "         <rt>: determine reference (r) or target (t) model,"    << "\n"
            << "         <s>:  context-order size,"                             << "\n"
            << "         <a>:  1/alpha,"                                        << "\n"
            << "         <i>:  inverted repeat (1=use, 0=don't use)."           << "\n"
            << "         EXAMPLE 1: -m r,4,1000,1"                              << "\n"
            << "         EXAMPLE 2: -m t,18,1,0:r,13,100,0"                     << "\n"
                                                                                << "\n"
            << "    -n [NUMBER],  --number [NUMBER]"                            << "\n"
            << "         number of something."                                  << "\n"
            << "         requires an integer number (NUMBER)"                   << "\n"
                                                                                << "\n"
            << "    -d [NUMBER],  --fnumber [NUMBER]"                           << "\n"
            << "         number of something."                                  << "\n"
            << "         requires a float number (NUMBER)"                      << "\n"
                                                                                << "\n"
            << "    -t [TARGET],  --target [TARGET]"                            << "\n"
            << "         target file address"                                   << "\n"
                                                                                << "\n"
            << "    -r [REFERENCE],  --reference [REFERENCE]"                   << "\n"
            << "         reference file"                                        << "\n"
                                                                                << "\n";
}


/***********************************************************
    show About Phoenix
************************************************************/
void Messages::about () const
{
    std::cout                                                               << "\n"
            << "PHOENIX v" << VERSION_PHOENIX << "." << RELEASE_PHOENIX     << "\n"
            << "================"                                           << "\n"
            << "A compression-based method"                                 << "\n"
                                                                            << "\n"
            << "Morteza Hosseini, Diogo Pratas, Armando J. Pinho"           << "\n"
            << "Copyright (C) 2016-2017 University of Aveiro"               << "\n"
                                                                            << "\n"
            << "This is a Free software, under GPLv3. You may redistribute" << "\n"
            << "copies of it under the terms of the GNU - General Public"   << "\n"
            << "License v3 <http://www.gnu.org/licenses/gpl.html>. There"   << "\n"
            << "is NOT ANY WARRANTY, to the extent permitted by law."       << "\n"
                                                                            << "\n";
}


/***********************************************************
    show verbose mode
************************************************************/
void Messages::verbose () const
{
    std::cout << "verbose mode.\n"; // TODO for test
}


/***********************************************************
    show integer number mode
************************************************************/
void Messages::number (int num) const
{
    std::cout << "integer number mode (n=" << num << ")\n"; // TODO for test
}


/***********************************************************
    show float number mode
************************************************************/
void Messages::fnumber (float num) const
{
    std::cout << "float number mode (d=" << num << ")\n"; // TODO for test
}
