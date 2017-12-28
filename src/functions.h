#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H

#include <iostream>
#include <getopt.h>     // Parsing command line
#include <fstream>
#include <vector>
#include <thread>
#include <cmath>
#include <algorithm>    // std::remove

#include "def.h"
#include "FCM.h"

using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::ifstream;
using std::invalid_argument;
using std::thread;
using std::ios;


/*******************************************************************************
    Parse command line
*******************************************************************************/
void commandLineParser (int argc, char **argv, FCM &mixModel)
{
    // Using these flags, if both short and long arguments are entered, just one
    // of them is considered
    static int h_flag;                  // Option 'h' (help)
    static int v_flag;                  // Option 'v' (verbose)
    static int d_flag;                  // Option 'd' (decompress)
    
    // Mandatory arguments
    bool m_flag = false;                // Model(s) parameters entered
    bool r_flag = false;                // Reference(s) file name entered
    bool t_flag = false;                // Target(s) file name entered
    string strModelsParameters = "";    // Argument of option 'm'
    string refFilesNames       = "";    // Argument of option 'r'
    string tarFilesNames       = "";    // Argument of option 't'
    
    int c;                              // Deal with getopt_long()
    int option_index;                   // Option index stored by getopt_long()
    
    opterr = 0;  // Force getopt_long() to remain silent when it finds a problem
    
    static struct option long_options[] =
    {
        {"help",      no_argument,       &h_flag, (int) 'h'}, // Help
        {"verbose",   no_argument,       &v_flag, (int) 'v'}, // Verbose
        {"decompress",no_argument,       &d_flag, (int) 'd'}, // Decompress
        {"model",     required_argument, 0,             'm'}, // Model(s)
        {"reference", required_argument, 0,             'r'}, // Ref. file(s)
        {"target",    required_argument, 0,             't'}, // Tar. file(s)
        {"nthreads",  required_argument, 0,             'n'}, // # threads >= 1
        {"gamma",     required_argument, 0,             'g'}, // 0 <= gamma < 1
        {0,           0,                 0,               0}
    };
    
    while (1)
    {
        option_index = 0;   // getopt_long() stores the option index here.
        
        c = getopt_long(argc, argv, ":hAvdm:r:t:n:g:",
                        long_options, &option_index);
             
        if (c == -1)    break;  // Detect the end of the options.
        
        switch (c)
        {
            case 0:
                // If this option set a flag, do nothing else now.
                if (long_options[ option_index ].flag != 0) break;
                
                cout << "option '" << long_options[ option_index ].name
                     << "'\n";
                if (optarg) cout << " with arg " << optarg << '\n';
                break;
            
            case 'h':   // Usage guide
                h_flag = 1;
                help();
                break;
            
            case 'v':   // Verbose mode
                v_flag = 1;
                break;
            
            case 'd':   // Decompression mode
                d_flag = 1;
                mixModel.setDecompFlag(true);
                break;
            
            case 'm':   // Needs model(s) parameters
                m_flag = true;
                strModelsParameters = (string) optarg;  // Keep model(s) params
                break;
            
            case 'r':   // Needs reference file(s) name(s)
                r_flag = true;
                refFilesNames = (string) optarg;       // Keep ref. files names
                break;
            
            case 't':   // Needs target file(s) name(s)
                t_flag = true;
                tarFilesNames = (string) optarg;       // Keep tar. files names
                break;
            
            case 'n':   // Needs an integer argument
                try
                {
                    u8 nThr = (u8) stoi((string) optarg);
                    mixModel.n_threads = (nThr<1 ? DEFAULT_N_THREADS : nThr);
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Error: Option 'n' ('nthreads') has an "
                            "invalid argument.\n";
                    return;
                }
                break;
            
            case 'g':   // Needs a double argument
                try
                {
                    double gamma = stod( (string) optarg );
                    mixModel.setGamma( (gamma < 0 || gamma >= 1)
                                       ? DEFAULT_GAMMA : gamma );
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "ERROR: Option 'g' ('gamma') has an "
                            "invalid argument.\n";
                    return;
                }
                break;
            
            case ':':   // Missing option argument
                cerr << "ERROR: Option '" << (char) optopt
                     << "' requires an argument.\n";
                break;
            
            case '?':   // Invalid option
            default:
                cerr << "ERROR: Option '" << (char) optopt
                     << "' is invalid.\n";
                break;
        }
    }
    
    // Save target file(s) name(s)
    if (t_flag)
    {
        string::iterator begIter = tarFilesNames.begin();
        string::iterator endIter = tarFilesNames.end();
        // All target files names but the last one
        for (string::iterator it = begIter; it != endIter; ++it)
        {
            if (*it == ',')
            {
                mixModel.pushTarAddr( string(begIter, it) );
                begIter = it + 1;
            }
        }
        mixModel.pushTarAddr( string(begIter, endIter) );     // last tar. name
        
        /*  Slower
        u8 tarIndex = (u8) tarFilesNames.size();
        // save all target files names except the last one
        for (u8 i = tarIndex; i--;)
        {
            if (tarFilesNames[ i ] == ',')
            {
                mixModel.pushBackTarAddresses(
                        tarFilesNames.substr(i + 1, tarIndex - i - 1));
                tarIndex = i;
            }
        }
        // save last target file name
        mixModel.pushTarAddr(tarFilesNames.substr(0, tarIndex));
        */
    }
    
    // Save reference file(s) name(s)
    if (r_flag)
    {
        string::iterator begIter = refFilesNames.begin();
        string::iterator endIter = refFilesNames.end();
        // All reference files names but the last one
        for (string::iterator it = begIter; it != endIter; ++it)
        {
            if (*it == ',')
            {
                mixModel.pushRefAddr( string(begIter, it) );
                begIter = it + 1;
            }
        }
        mixModel.pushRefAddr(string(begIter, endIter));     // Last ref. name
        
        /*  Slower
        u8 refIndex = (u8) refFilesNames.size();
        // save all reference files names except the last one
        for (u8 i = refIndex; i--;)
        {
            if (refFilesNames[ i ] == ',')
            {
                mixModel.pushBackRefAddresses(
                        refFilesNames.substr(i + 1, refIndex - i - 1));
                refIndex = i;
            }
        }
        // save last reference file name
        mixModel.pushRefAddr(refFilesNames.substr(0, refIndex));
        */
    }
    
    // Save model(s) parameters and process the model(s)
    if (m_flag)
    {
        vector< string > vecModelsParams;    // Parameters for different models
        string::iterator begIter = strModelsParameters.begin();
        string::iterator endIter = strModelsParameters.end();
        // All models parameters but the last one
        for (string::iterator it = begIter; it != endIter; ++it)
        {
            if (*it == ':')
            {
                vecModelsParams.push_back( string(begIter, it) );
                begIter = it + 1;
            }
        }
        // Last model parameters
        vecModelsParams.push_back( string(begIter, endIter) );
                                                               
        vector< string > modelParams;               // Params for each model
        u8 n_models = (u8) vecModelsParams.size();  // Number of models
        mixModel.n_models = n_models;             // Set number of models
//        mixModel.setN_models(n_models);             // Set number of models
                                                 cout<<mixModel.n_models;
        for (u8 n = n_models; n--;)
        {
            modelParams.clear();                    // Reset vector modelParams
            
            begIter = vecModelsParams[ n ].begin();
            endIter = vecModelsParams[ n ].end();
            // All paramaeters for each model but the last one
            for (string::iterator it = begIter; it != endIter; ++it)
            {
                if (*it == ',')
                {
                    modelParams.push_back( string(begIter, it) );
                    begIter = it + 1;
                }
            }
            // Parameters for the last model
            modelParams.push_back( string(begIter, endIter) );
                                                               
            // Set model(s) parameters
            mixModel.pushParams((bool) stoi(modelParams[ 0 ]),  // IR
                                (u8) stoi(modelParams[ 1 ]),    // Ctx depth
                                (u16) stoi(modelParams[ 2 ]));  // Alpha denom.
        }
        
        // Set compression mode: 't'=table, 'h'=hash table
        // 5^k_1 + 5^k_2 + ... > 5^12 ==> mode: hash table
        u64 cmpModeSum = 0;
        for(u8 k : mixModel.getCtxDepth()) cmpModeSum = cmpModeSum + POWER5[k];
        const char compressionMode = (cmpModeSum > POWER5[TABLE_MAX_CTX])
                                     ? 'h' : 't';
        mixModel.setCompMode(compressionMode);
        
        // Initialize vector of tables or hash tables
        compressionMode == 'h' ? mixModel.initHashTables()
                               : mixModel.initTables();
    }
    
    // Print any remaining command line arguments (not options).
    if (optind < argc)
    {
        cerr << "Error: non-option ARGV-element(s): ";
        while (optind < argc)   cerr << argv[ optind++ ] << " ";
        cerr << '\n';
    }
    
}   // End commandLineParser


/*******************************************************************************
    Check if original and decompressed files are identical
*******************************************************************************/
bool areFilesEqual (const string &first, const string &second)
{
    ifstream firstFile  (first,  ios::in);  // Open first file
    ifstream secondFile (second, ios::in);  // Open second file
    
    // Error occurred while opening files
    if (!firstFile)
    {
        cerr << "ERROR: The file '" << first
             << "' cannot be opened, or it is empty.\n";
        exit(1);
    }
    else if (!secondFile)
    {
        cerr << "ERROR: The file '" << second
             << "' cannot be opened, or it is empty.\n";
        exit(1);
    }
    
    // Keep each line as well as all of the first and second files
    string firstLine, secondLine, firstStr = "", secondStr = "";
    
    // Remove '\n' from first and second files and save them in a string
    while ( getline(firstFile, firstLine) )
    {
        firstLine.erase( std::remove(firstLine.begin(), firstLine.end(), '\n'),
                         firstLine.end() );
        firstStr += firstLine;
    }
    while ( getline(secondFile, secondLine) )
    {
        secondLine.erase( std::remove(secondLine.begin(),secondLine.end(),'\n'),
                          secondLine.end() );
        secondStr += secondLine;
    }
    
    firstFile.close();  secondFile.close(); // Close files
    
    // If files are identical, return true, otherwise return false
    return (firstStr == secondStr);
}


/*******************************************************************************
    Check if file opened correctly
*******************************************************************************/
/*
//bool isFileCorrect (const string &fileName)
bool Functions::isFileCorrect (ifstream &fileIn)
{
    // check if file doesn't exist or is empty
    if (!fileIn)
    {
//        cerr << "The file cannot be opened, or it is empty.\n";
        fileIn.close(); // close file
        return false;   // error occurred while opening file
    }
//    // check if file is empty
//    else if (fileIn.peek() == ifstream::traits_type::eof())
//    {
//        cerr << "The File is empty.\n";
//        fileIn.close(); // close file
//        return false;   // error occurred while opening file
//    }

    return true;    // file opened correctly
}
*/

#endif //PHOENIX_FUNCTIONS_H