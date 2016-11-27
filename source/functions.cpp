#include "functions.h"

#include <iostream>
#include <getopt.h>     // for command line parsing
#include <string>
#include <fstream>

#include "messages.h"
#include "hash.h"


/***********************************************************
    constructor
************************************************************/
Functions::Functions () {}


/***********************************************************
    parse command line
************************************************************/
int32_t Functions::commandLineParser (int argc, char **argv)
{
    Messages messageObj;    // object for showing messages
    Hash hashObj;           // object of hash table
    
    // using these flags, if both short and long arguments
    // are entered, just one of them is considered
    static int h_flag;  // option 'h' (help)
    static int A_flag;  // option 'A' (about)
    static int v_flag;  // option 'v' (verbose)
    static int i_flag;  // option 'i' (inverted_repeat)
    
    bool t_flag = false;                // target file name entered
    bool r_flag = false;                // reference file name entered
    std::string targetFileName = "";    // argument of option 't'
    std::string referenceFileName = ""; // argument of option 'r'
    
    int c;              // deal with getopt_long()
    int option_index;   // option index stored by getopt_long()

    opterr = 0;         // force getopt_long() to remain silent when it finds a problem

    static struct option long_options[] =
            {
                    {"help",            no_argument, &h_flag, (int) 'h'},   // help
                    {"about",           no_argument, &A_flag, (int) 'A'},   // About
                    {"verbose",         no_argument, &v_flag, (int) 'v'},   // verbose
                    {"inverted_repeat", no_argument, &i_flag, (int) 'i'},   // inverted_repeat
                    {"number",    required_argument,       0,       'n'},   // number (integer)
                    {"fnumber",   required_argument,       0,       'd'},   // number (float)
                    {"target",    required_argument,       0,       't'},   // target file
                    {"reference", required_argument,       0,       'r'},   // reference file
                    {0, 0, 0, 0}
            };
    
    while (1)
    {
        /* getopt_long() stores the option index here. */
        option_index = 0;

        c = getopt_long(argc, argv, ":hAvin:d:t:r:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[ option_index ].flag != 0)
                    break;

                std::cout << "option '" << long_options[ option_index ].name << "'\n";
                if (optarg)
                    std::cout << " with arg " << optarg << "\n";
                break;

            case 'h':   // show usage guide
                h_flag = 1;
                messageObj.help();
                break;
    
            case 'A':   // show About Phoenix
                A_flag = 1;
                messageObj.about();
                break;

            case 'v':   // verbose mode
                v_flag = 1;
                messageObj.verbose();
                break;
    
            case 'i':   // inverted_repeat mode
                i_flag = 1;
                break;

            case 'n':   // needs an integer argument
                try
                {
                    messageObj.number(std::stoi((std::string) optarg));    //for test
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cerr << "Option 'n' ('number') has an invalid argument.\n";
                }
                break;
    
            case 'd':   // needs a float argument
                try
                {
                    messageObj.fnumber( std::stof((std::string) optarg) );   //for test
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cerr << "Option 'd' ('fnumber') has an invalid argument.\n";
                }
                break;
    
            case 't':   // needs target file name
                t_flag = true;
                targetFileName = (std::string) optarg; // keep argument = target file name
                break;
    
            case 'r':   // needs reference file name
                r_flag = true;
                referenceFileName = (std::string) optarg; // keep argument = reference file name
                break;
                
            case ':':   /* missing option argument */
                std::cerr << "Option '" << (char) optopt << "' requires an argument.\n";
                break;

            case '?':   /* invalid option */
            default:
                std::cerr << "Option '" << (char) optopt << "' is invalid.\n";
                break;
        }
    }
    
    
    if (t_flag)
    {


        std::ifstream targetFile(targetFileName, std::ios::in);   // open file

        if (!targetFile)
            std::cerr << "File '" << targetFileName << "' could not be opened.\n";
        else if (targetFile.peek() == std::ifstream::traits_type::eof())
            std::cerr << "File '" << targetFileName << "' is empty.\n";

        // context, that slides in the dataset
        std::string context(CONTEXT_DEPTH, 'A');
        std::string strLine;    // keep each line
        std::getline(targetFile, strLine);
        strLine = context + strLine;

        hashTable_t hTable;
        hTable.insert( {context, {0, 0, 0, 0, 0}} );   // initialize hash table with 0'z

        bool isFirstTime = true;
        hashTable_t hTableForPrint;
        hTableForPrint
                = hashObj.hashTableUpdate(hTable, context, strLine,
                                          (bool) i_flag, isFirstTime);
        
        while (!targetFile.eof())
        {
            context = strLine.substr(strLine.size() - CONTEXT_DEPTH, CONTEXT_DEPTH);
            std::getline(targetFile, strLine);
    
            isFirstTime = false;
            hTableForPrint = hashObj.hashTableUpdate(hTableForPrint, context, strLine,
                                                     (bool) i_flag, isFirstTime);
        }
        
        hashObj.hashTablePrint(hTableForPrint);   // print hash table

        targetFile.close(); // close file












//        std::string targetFile = Functions::fileRead(targetFileName);
//        if (targetFile != "")
//        {
//            // build a hash table for the input file, considering inverted repeat mode
//            hashTable_t hTable = hashObj.hashTableBuild(targetFile, (bool) i_flag);
//            hashObj.hashTablePrint(hTable);   // print hash table
//        }
    }
    
    if (r_flag)
    {
//        std::string referenceFile = Functions::fileRead(referenceFileName);
//        if (referenceFile != "")
//        {
//            // build a hash table for the input file, considering inverted repeat mode
//            hashTable_t hTable = hashObj.hashTableBuild(referenceFile, (bool) i_flag);
//            hashObj.hashTablePrint(hTable);   // print hash table
//        }
    }
    
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        std::cerr << "non-option ARGV-element(s): ";
        while (optind < argc)
            std::cerr << argv[ optind++ ] << " ";
        std::cerr << std::endl;
    }
}


/***********************************************************
    read a file
************************************************************/
std::string Functions::fileRead (const std::string& fileName)
{
    std::ifstream myFile(fileName, std::ios::in);   // open file
    
    if (!myFile)
    {
        std::cerr << "File \"" << fileName << "\" could not be opened.\n";
        return "";
    }
    
    std::string strEachLine;    // keep each line
    std::string strDataset;     // keep the whole dataset
    
    // read file line by line
    while (std::getline(myFile, strEachLine))
        strDataset += strEachLine;
    
    myFile.close(); // close file
    
    return strDataset;
}
