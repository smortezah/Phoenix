#include "hash.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "def.h"
#include "functions.h"



//#include <array>
//#include <vector>
//#include <algorithm>
//#include <cmath>
//#include <iterator>

//#include "bitio.h"
//#include "arith.h"
//#include "arith_aux.h"


/***********************************************************
    constructor
************************************************************/
Hash::Hash () {}


/***********************************************************
    build hash table
************************************************************/
hashTable_t Hash::hashTableBuild (std::string strDataset, bool isInvertedRepeat)
{
    hashTable_t hTable;
    
    // context, that slides in the dataset
    std::string context(CONTEXT_DEPTH, 'A');

    // add "AA..." (with the size of CONTEXT_DEPTH) to the beginning of the dataset
    strDataset = context + strDataset;
    

//    int counters[4];
//    memset(counters, 0, sizeof(counters[ 0 ]) * 4);
//    int totalCount = 0;
//    int symbol = 0;
//    FILE *writer = fopen("mori", "w");


    hTable.insert( {context, {0, 0, 0, 0}} );   // initialize hash table with 0'z

    // fill hash table by number of occurrences of symbols A, C, T, G, N
    for (size_t i = CONTEXT_DEPTH; i < strDataset.size(); ++i)
    {
        switch (strDataset[ i ])
        {
            case 'A':
                ++(hTable[ context ])[ 0 ];  // increment number of 'A's. order: {A, C, T, G, N}
        
                // if inverted repeat option is selected in command line,
                // hash table considers inverted repeats for getting updated, too
                if (isInvertedRepeat)
                {
                    std::string invRepeat = context + "A";
    
                    // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                    for (char& ch : invRepeat)
                        ch = (ch == 'A') ? 'T' :
                             (ch == 'C') ? 'G' :
                             (ch == 'T') ? 'A' :
                             (ch == 'G') ? 'C' :
                             'N';
    
                    // invert the string
                    std::reverse( invRepeat.begin(), invRepeat.end() );
    
                    // inverted repeat context
                    std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
    
                    switch (invRepeat[ invRepeat.size() - 1 ])
                    {
                        case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                        case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                        case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                        case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                        case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                        default:                                            break;
                    }
                }

//                counters[ 0 ] += ALPHA_DENUMERATOR * table[ index ][ 0 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            case 'C':
                ++(hTable[ context ])[ 1 ];  // increment number of 'C's. order: {A, C, T, G, N}
                
                // if inverted repeat option is selected in command line,
                // hash table considers inverted repeats for getting updated, too
                if (isInvertedRepeat)
                {
                    std::string invRepeat = context + "C";
            
                    // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                    for (char& ch : invRepeat)
                        ch = (ch == 'A') ? 'T' :
                             (ch == 'C') ? 'G' :
                             (ch == 'T') ? 'A' :
                             (ch == 'G') ? 'C' :
                             'N';
            
                    // invert the string
                    std::reverse( invRepeat.begin(), invRepeat.end() );
            
                    // inverted repeat context
                    std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
            
                    switch (invRepeat[ invRepeat.size() - 1 ])
                    {
                        case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                        case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                        case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                        case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                        case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                        default:                                            break;
                    }
                }

//                counters[ 1 ] += ALPHA_DENUMERATOR * table[ index ][ 1 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            case 'T':
                ++(hTable[ context ])[ 2 ];  // increment number of 'T's. order: {A, C, T, G, N}
                
                // if inverted repeat option is selected in command line,
                // hash table considers inverted repeats for getting updated, too
                if (isInvertedRepeat)
                {
                    std::string invRepeat = context + "T";
            
                    // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                    for (char& ch : invRepeat)
                        ch = (ch == 'A') ? 'T' :
                             (ch == 'C') ? 'G' :
                             (ch == 'T') ? 'A' :
                             (ch == 'G') ? 'C' :
                             'N';
            
                    // invert the string
                    std::reverse( invRepeat.begin(), invRepeat.end() );
            
                    // inverted repeat context
                    std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
            
                    switch (invRepeat[ invRepeat.size() - 1 ])
                    {
                        case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                        case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                        case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                        case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                        case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                        default:                                            break;
                    }
                }

//                counters[ 2 ] += ALPHA_DENUMERATOR * table[ index ][ 2 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            case 'G':
                ++(hTable[ context ])[ 3 ];  // increment number of 'G's. order: {A, C, T, G, N}
        
                // if inverted repeat option is selected in command line,
                // hash table considers inverted repeats for getting updated, too
                if (isInvertedRepeat)
                {
                    std::string invRepeat = context + "G";
            
                    // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                    for (char& ch : invRepeat)
                        ch = (ch == 'A') ? 'T' :
                             (ch == 'C') ? 'G' :
                             (ch == 'T') ? 'A' :
                             (ch == 'G') ? 'C' :
                             'N';
            
                    // invert the string
                    std::reverse( invRepeat.begin(), invRepeat.end() );
            
                    // inverted repeat context
                    std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
            
                    switch (invRepeat[ invRepeat.size() - 1 ])
                    {
                        case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                        case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                        case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                        case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                        case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                        default:                                            break;
                    }
                }

//                counters[ 3 ] += ALPHA_DENUMERATOR * table[ index ][ 3 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;
    
            case 'N':
                ++(hTable[ context ])[ 4 ];  // increment number of 'N's. order: {A, C, T, G, N}
        
                // if inverted repeat option is selected in command line,
                // hash table considers inverted repeats for getting updated, too
                if (isInvertedRepeat)
                {
                    std::string invRepeat = context + "N";
            
                    // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                    for (char& ch : invRepeat)
                        ch = (ch == 'A') ? 'T' :
                             (ch == 'C') ? 'G' :
                             (ch == 'T') ? 'A' :
                             (ch == 'G') ? 'C' :
                             'N';
            
                    // invert the string
                    std::reverse( invRepeat.begin(), invRepeat.end() );
            
                    // inverted repeat context
                    std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
            
                    switch (invRepeat[ invRepeat.size() - 1 ])
                    {
                        case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                        case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                        case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                        case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                        case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                        default:                                            break;
                    }
                }

//                counters[ 3 ] += ALPHA_DENUMERATOR * table[ index ][ 3 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            default: break;
        }

//        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                     (4 * ALPHA_NUMERATOR);
//
//        memset(counters, 0, sizeof(counters[ 0 ]) * 4);

        // slide in the dataset
        context = strDataset.substr(i - CONTEXT_DEPTH + 1, CONTEXT_DEPTH);
    }
    
    return hTable;
}


/***********************************************************
    print hash table
************************************************************/
void Hash::hashTablePrint (hashTable_t hTable)
{
    std::cout << "\tA\tC\tT\tG\tN\n"
              << "\t------------------------------------\n";
    for (hashTable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
    {
        std::cout << it->first << "\t";
        for (int i : it->second)    std::cout << i << "\t";
        std::cout << "\n";
    }
}