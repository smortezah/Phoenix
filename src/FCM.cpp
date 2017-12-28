//todo: nokte: har vaght segmentation fault (core dump) dad, avalin jayee
//todo: ke bayad check she, gozoshtane "lock/unlock" tu functione
//todo: compress ya buildModel hast

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>      // setw, setprecision
#include <cstring>      // memset, memcpy
#include <cmath>        // std::round
#include <stdlib.h>
#include <vector>

#include "FCM.h"
#include "ArithEncDec.h"

using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::ifstream;
using std::getline;
using std::to_string;
using std::ios;
using std::memset;
using std::memmove;
using std::fixed;
using std::setprecision;
using std::fill_n;
using std::round;
using std::chrono::high_resolution_clock;


/*******************************************************************************
    Constructor
*******************************************************************************/
FCM::FCM ()
{
    n_threads  = DEFAULT_N_THREADS;
    gamma      = DEFAULT_GAMMA;
    decompFlag = false;
}


/*******************************************************************************
    Build reference(s) model
*******************************************************************************/
void FCM::buildModel (const vector<string> &refsNames,
                      bool invRepeat, u8 ctxDepth, u16 modelIndex)
{
    u8 refsNo = (u8) refsNames.size();    // # references
    
    // Check if reference(s) file(s) cannot be opened, or are empty
    ifstream refsIn[refsNo];
    for (u8 i = refsNo; i--;)
    {
        refsIn[i].open(refsNames[i], ios::in);
        if (!refsIn[i])        // Error occurred while opening file(s)
        {
            cerr << "The file '" << refsNames[i]
                 << "' cannot be opened, or it is empty.\n";
            refsIn[i].close(); // Close file(s)
            return;              // Exit this function
        }
    }
    
    u64 context;                // Context (integer) that slides in the dataset
    u64 maxPlaceValue = POWER5[ctxDepth];
    u64 befMaxPlaceValue = POWER5[ctxDepth-1];
    u64 invRepContext;          // Inverted repeat context (integer)
//    u32 n_div = 0;              // No. divisions for no. syms at table/hash tbl
    
    u64 iRCtxCurrSym;           // Concat of IR context and current symbol
    u8  currSymInt;             // Current symbol integer
    
    string refLine;             // Keep each line of the file
    
    switch (compMode)      // Build model based on 't'=table, or 'h'=hash table
    {
        case 't':
        {
            u64 tableSize = maxPlaceValue * ALPH_SUM_SIZE;
            u64 *table = new u64[tableSize];
//            u16 *table = new u16[ tableSize ];
            memset(table, 0, sizeof(table[0]) * tableSize);  // 0 init. table
///            fill_n(table, tableSize, 0);
            u64 rowIndex;        // To update table
            
            for (u8 i = refsNo; i--;)
            {
                // Reset in the beginning of each reference file
                context = 0;
                invRepContext = maxPlaceValue - 1;
                
                while (getline(refsIn[ i ], refLine))
                {
                    // Fill table by no. occurrences of symbols A,C,N,G,T
                    for (string::iterator lineIt = refLine.begin();
                         lineIt != refLine.end(); ++lineIt)
                    {
                        currSymInt = symCharToInt(*lineIt);
                        
                        if (invRepeat)    // Considering IRs to update table
                        {
                            // Concatenation of IR context and current symbol
                            iRCtxCurrSym = invRepContext +
                                   (IR_MAGIC_NUM - currSymInt) * maxPlaceValue;
                            // Update inverted repeat context (integer)
                            invRepContext = (u64) iRCtxCurrSym / ALPH_SIZE;
                            
                            // Update table
                            rowIndex = invRepContext * ALPH_SUM_SIZE;
                            ++table[ rowIndex + iRCtxCurrSym % ALPH_SIZE ];
                            
                            ++table[ rowIndex + ALPH_SIZE ];    // 'sum' col
                            // Update 'sum' col, then check for overflow
//                            if (++table[ rowIndex + ALPH_SIZE ]
//                                >= MAX_N_BASE_SUM)
//                            {
///                                ++n_div;           // Count no. of divisions
//                                for (u8 j = ALPH_SUM_SIZE; j--;)
//                                    table[ rowIndex + j ] >>= 1;
//                            }
                        }
                        
                        rowIndex = context * ALPH_SUM_SIZE;
                        ++table[ rowIndex + currSymInt ];  // Update table
                        ++table[ rowIndex + ALPH_SIZE ];   // Update 'sum' col
//                        if (++table[ rowIndex + ALPH_SIZE ] >= MAX_N_BASE_SUM)
//                        {
//                            ++n_div;               // Count no. of divisions
//                            for (u8 j = ALPH_SUM_SIZE; j--;)
//                                table[ rowIndex + j ] >>= 1;
//                        }
                            
                        // Update context.
                        // (rowIndex - context) == (context * ALPH_SIZE)
///         context = (u64) (rowIndex - context + currSymInt) % maxPlaceValue;
///         context = (u64) (rowIndex - context) % maxPlaceValue + currSymInt;
                        context =(u64) (context % befMaxPlaceValue) * ALPH_SIZE
                                         + currSymInt;
                    }
                }   // End while
            }   // End for
            
            //todo: test
//          for (int j = 0; j < 5; ++j)
//          {
//              for (int k = 0; k < 5; ++k)
//                  cout << table[ j * 6 + k ] << '\t';
//              cout << '\n';
//          }
//          cout<<n_div;
            
            
            // Set table
            mut.lock();   this->setTable(table, modelIndex);   mut.unlock();
        }   // End case
            break;
        
        case 'h':       // Adding 'sum' column, makes hash table slower
        {
            htable_t hashTable;
//            array< u16, ALPH_SIZE > hTRowArray;
//            u32 sumRow;
            array< u64, ALPH_SIZE > hTRowArray;
            u64 sumRow;
            
            for (u8 i = refsNo; i--;)
            {
                // Reset in the beginning of each reference file
                context = 0;
                invRepContext = maxPlaceValue - 1;
                
                while (getline(refsIn[ i ], refLine))
                {
                    // Fill hash table by no. occurrences of syms A,C,N,G,T
                    for (string::iterator lineIt = refLine.begin();
                         lineIt != refLine.end(); ++lineIt)
                    {
                        currSymInt = symCharToInt(*lineIt);
                        
                        // Considering IRs to update hash table
                        if (invRepeat)
                        {
                            // Concatenation of IR context and current symbol
                            iRCtxCurrSym = invRepContext +
                                    (IR_MAGIC_NUM - currSymInt) * maxPlaceValue;
                            // Update inverted repeat context (integer)
                            invRepContext = (u64) iRCtxCurrSym / ALPH_SIZE;
                            
                            // Update hash table considering IRs
                            ++hashTable[ invRepContext ]
                                       [ iRCtxCurrSym % ALPH_SIZE ];
//                            if (++hashTable[ invRepContext ]
//                                           [ iRCtxCurrSym % ALPH_SIZE ]
//                                >= MAX_N_BASE)
//                            {
///                                ++n_div;          // Count no. of divisions
//                                for (u8 j = ALPH_SIZE; j--;)
//                                    hashTable[ invRepContext ][ j ] >>= 1;
//                            }
                        }
                        
                        // Update hash table
                        ++hashTable[ context ][ currSymInt ];
                        
                        /* todo: bebin tu sharayete yeksan ba table, inja o
                         * unja chand bar taghsim mishe?
                         * NRC ha alan yeki nist
                         */
                        hTRowArray = hashTable[ context ];
                        
                        //todo test
//                        for (int k = 0; k < 5; ++k)
//                        {
//                         cout<<hTRowArray[k]<<' ';
//                        }
//                        cout<<'\n';
                        
                        
//                        sumRow = 0;  for (u16 u : hTRowArray)  sumRow += u;
//                        sumRow = 0;  for (u64 u : hTRowArray)  sumRow += u;
    
                        //todo test
//                        cout<<sumRow<<' ';
//                        cout<<'\n';
                        
    
//                        if (sumRow >= MAX_N_BASE_SUM)
//                        {
//                            ++n_div;            // count no. of divisions
//                            for (u8 j = ALPH_SIZE; j--;)
//                                hashTable[ context ][ j ] >>= 1;
//                        }
    
                        // Update context.
                        // (rowIndex - context) == (context * ALPH_SIZE)
////       context = (u64) (rowIndex - context + currSymInt) % maxPlaceValue;
////       context = (u64) (rowIndex - context) % maxPlaceValue + currSymInt;
                        context =(u64) (context % befMaxPlaceValue) * ALPH_SIZE
                                        + currSymInt;
                    }
                }
            }   // End for
            
            
            //todo test
//            for (htable_t::iterator it = hashTable.begin();
//                 it != hashTable.end(); ++it)
//            {
//                cout << it->first << "\t";
//                for (u16 i : it->second)    cout << i << "\t";
//                cout << '\n';
//            }
//            cout<<n_div;
            
            // Set hash table
            mut.lock(); this->setHashTable(hashTable, modelIndex); mut.unlock();
        }   // End case
            break;
        
        default:    break;
    }   // End switch
    
    for (u8 i = refsNo; i--;)   refsIn[ i ].close();       // Close file(s)
}


/*******************************************************************************
    Compress target(s) based on reference(s) model
*******************************************************************************/
void FCM::compress (const string &tarName)
{
//    ArithEncDec arithObj;   // To work with arithmetic encoder/decoder class
    
    // Alpha and ALPH_SIZE*alpha: used in P numerator and denominator
    double alpha[n_models], sumAlphas[n_models];
    for (u16 i = n_models; i--;)
    {
        alpha[ i ] = (double) 1 / alphaDens[ i ];
        sumAlphas[ i ] = ALPH_SIZE * alpha[ i ];
    }
    
    // Open target file
    ifstream tarIn(tarName, ios::in);
    
    mut.lock();//========================================================
    if (!tarIn)                     // Error occurred while opening file
    {
        cerr << "The file '" << tarName
             << "' cannot be opened, or it is empty.\n";
        tarIn.close();              // Close file
        return;                         // Exit this function
    }
    mut.unlock();//======================================================
    
    u64 maxPlaceValue[n_models];
    for (u16 i = n_models; i--;) maxPlaceValue[ i ] = POWER5[ ctxDepths[i] ];
    
    // Context(s) (integer) sliding through the dataset
    u64    tarContext[n_models];   fill_n(tarContext, n_models, 0);
    string tarLine;              // Keep each line of the file
    u64    nSym;                 // No. symbols (n_s). in probability numerator
    u64    sumNSym;      // Sum of no. symbols (sum n_a). in probability denom.
    double prob_i = 0.0;         // Probability of a symbol for each model
//    double rawWeight[n_models];  // Weight before normalization. init: 1/M
//    double weight[n_models]; fill_n(weight, n_models, (double) 1 / n_models);
    double probability;          // Final probability of a symbol
    double sumOfEntropies = 0;   // Sum of entropies for different symbols
//    u64    file_size = fileSize(tarName);// Size of file, including '\n'
    u64    symsNo = countSymbols(tarName);  // Number of symbols
    double averageEntropy = 0;   // Average entropy (H)
//    double sumOfWeights;         // Sum of weights. used for normalization
//    double freqsDouble[ALPH_SIZE];      // Frequencies of each symbol (double)
//    int    freqs[ALPH_SIZE];     // Frequencies of each symbol (integer)
//    int    sumFreqs;             // Sum of frequencies of each symbol
    u8     currSymInt;           // Current symbol in integer format
    
    /*
    // Using macros make this code slower
    #define X \
         ((compMode == 'h') ? (hashTable[ tarContext ][ currSymInt ]) \
                            : (table[ tarContext*ALPH_SUM_SIZE + currSymInt ]))
    #define Y(in) do { \
                (compMode == 't') \
                ? in = table[ tarContext * ALPH_SUM_SIZE + ALPH_SIZE ] \
                : in = 0; for (u64 u : hashTable[ tarContext ]) in += u; \
              } while ( 0 )
    */
    
    // Build TAR.co filename
    size_t lastSlash_Tar = tarName.find_last_of("/");    // Position last /
    string tarNamePure = tarName.substr(lastSlash_Tar + 1);
//    const char *tar = (tarNamePure + COMP_FILETYPE).c_str(); // string -> char*
//
//    mut.lock();//========================================================
//    remove(tar);                      // Remove pre-existing compressed file(s)
//
//    FILE *Writer = fopen(tar, "w");   // To save compressed file
//    mut.unlock();//======================================================
//
//    //todo: test
///    _bytes_output = 0;                  // Output bytes
//    arithObj.startoutputtingbits();       // Start arithmetic encoding process
//    arithObj.start_encode();
//
//    // Model(s) properties, being sent to decoder as header
//    arithObj.WriteNBits(WATERMARK, 26,Writer); // WriteNBits just writes header
//    arithObj.WriteNBits(symsNo,    46, Writer);   // Number of symbols, in byte
//    arithObj.WriteNBits((u64) (gamma * 65536),  32, Writer);    // Gamma
//    arithObj.WriteNBits(n_models,  16, Writer);   // Number of models
//    for (u16 n = 0; n < n_models; ++n)
//    {
//        arithObj.WriteNBits((u8) invRepeats[n], 1,  Writer);    // IRs
//        arithObj.WriteNBits(ctxDepths[n],       16, Writer);    // Ctx depths
//        arithObj.WriteNBits(alphaDens[n],       16, Writer);    // Alpha denoms
//    }
//    arithObj.WriteNBits((u64) compMode,         16, Writer);    // Comp. mode
//
///     = _bytes_output;     // [n_bits/8]

    switch (compMode)
    {
        case 't':
        {
            u64 rowIndex;                   // Index of a row in the table

            while (getline(tarIn, tarLine))
            {
                // Table includes the no. occurrences of symbols A,C,N,G,T
                for (string::iterator lineIt = tarLine.begin();
                     lineIt != tarLine.end(); ++lineIt)
                {
//                    fill_n(freqsDouble, ALPH_SIZE, 0);  // Reset array of freqs

                    // Integer version of the current symbol
                    currSymInt = symCharToInt(*lineIt);

                    probability = 0;
//                    sumOfWeights = 0;

                    for (u16 i = n_models; i--;)
                    {
                        rowIndex = tarContext[ i ] * ALPH_SUM_SIZE;

//                        // Frequencies (double)
//                        for (u8 j = ALPH_SIZE; j--;)
//                           freqsDouble[j] += weight[i] * tables[i][rowIndex+j];

                        nSym = tables[ i ][ rowIndex + currSymInt ]; // No.syms
///                          nSym = X;

                        // Sum of number of symbols
                        sumNSym = tables[ i ][ rowIndex + ALPH_SIZE ];
///                          Y(sumNSyms);
                        // P(s|c^t)
                        prob_i = (nSym + alpha[i]) / (sumNSym + sumAlphas[i]);
//                        // P_1*W_1 + P_2*W_2 + ...
//                        probability = probability + weight[ i ] * prob_i;
//
//                        // Weight before normalization
//                        //todo: fastPow(1,0.95) != 1.
//                        //todo. halate n_models=1 bas joda she
//                        rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
//                        // Sum of weights. used for normalization
//                        sumOfWeights = sumOfWeights + rawWeight[ i ];

                        // Update context
                    //(rowIndex - tarContext[i]) = (tarContext[i] * ALPH_SIZE)
                        tarContext[ i ]
                                = (u64) (rowIndex - tarContext[i] + currSymInt)
                                          % maxPlaceValue[ i ];
                    }
//                    // Update weights
//                    for (u8 i = n_models; i--;)
//                        weight[ i ] = rawWeight[ i ] / sumOfWeights;

                    // sum( log_2 P(s|c^t) )
                    sumOfEntropies = sumOfEntropies + log2(prob_i);
//                    sumOfEntropies = sumOfEntropies + log2(probability);
//
//                    // Frequencies (integer)
//                    for (u8 j = ALPH_SIZE; j--;)
//                        freqs[j] =(int) (1 + (freqsDouble[j] * DOUBLE_TO_INT));
//
//                    sumFreqs = 0;   for (int f : freqs) sumFreqs += f;
//
                    //todo test
//                    for (int j = 0; j < 5; ++j)
//                      cout << freqsDouble[ j ] << ' '; cout<<'\n';
//                    for (int j = 0; j < 5; ++j)
//                      cout << freqs[ j ] << ' '; cout<<'\n';
//                    cout << sumFreqs;

//                    // Arithmetic encoding
//                    arithObj.AESym(currSymInt, freqs, sumFreqs, Writer);
                }   // End for
            }   // End while
        }   // End case
            break;

        case 'h':
        {
            // Hash table row array -- to save a row of hTable
            array< u64, ALPH_SIZE > hTRowArray;
//            array< u16, ALPH_SIZE > hTRowArray;
            
            u64 tarCtxI = 0;

            while (getline(tarIn, tarLine))
            {
                // Hash table includes no. occurrences of symbols A,C,N,G,T
                for (string::iterator lineIt = tarLine.begin();
                     lineIt != tarLine.end(); ++lineIt)
                {
//                    fill_n(freqsDouble, ALPH_SIZE, 0); // reset array of freqs
                    
                    // Integer version of the current symbol
                    currSymInt = symCharToInt(*lineIt);
                    
                    probability = 0;
//                    sumOfWeights = 0;
                    
                    for (u16 i = n_models; i--;)
                    {
                        //todo. in lock va3 case table ham check she, age niaz
                        //todo. bood, bezar
                        // Save the row of hash table into an array
                        tarCtxI = tarContext[ i ];
                        mut.lock();//==========================================
                        hTRowArray = hashTables[ i ][ tarCtxI ];
                        mut.unlock();//========================================
                        
//                        // Frequencies (double)
//                        for (u8 j = ALPH_SIZE; j--;)
//                            freqsDouble[ j ] += weight[ i ] * hTRowArray[ j ];

                        // Sum of number of symbols
                        sumNSym = 0;    for (u64 u : hTRowArray) sumNSym += u;
///                        Y(sumNSym);
                        nSym = hTRowArray[ currSymInt ];   // Number of symbols
///                          nSym = X;
///                          X(nSym);
                        // P(s|c^t)
                        prob_i = (nSym + alpha[i]) / (sumNSym + sumAlphas[i]);
//                        // P_1*W_1 + P_2*W_2 + ...
//                        probability += weight[ i ] * prob_i;
//
//                        // Weight before normalization
//                        rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
//                        // Sum of weights. used for normalization
//                        sumOfWeights = sumOfWeights + rawWeight[ i ];

                        /* TODO: ba update e tarContext[i], hash table mixModel
                         * ro ham update mikone, chon object mixModel ba
                         * reference (&mixModel) be in tabe' (compress)
                         * ferestade mishe. varesh darim NRC ghalat mishe */
                        // update context
                        tarContext[ i ] = (u64) (tarCtxI*ALPH_SIZE + currSymInt)
                                                % maxPlaceValue[i];
                    }

//                    // Update weights
//                    for (u8 i = n_models; i--;)
//                        weight[ i ] = rawWeight[ i ] / sumOfWeights;

                    // sum( log_2 P(s|c^t) )
                    sumOfEntropies = sumOfEntropies + log2(prob_i);
//                    sumOfEntropies = sumOfEntropies + log2(probability);
//
//                    // Frequencies (integer)
//                    for (u8 j = ALPH_SIZE; j--;)
//                        freqs[j] =(int) (1 + (freqsDouble[j] * DOUBLE_TO_INT));
//
//                    sumFreqs = 0;   for (int f : freqs) sumFreqs += f;
//
//                    // Arithmetic encoding
//                    arithObj.AESym(currSymInt, freqs, sumFreqs, Writer);
                }   // End for
            }   // End while
        }   // End case
            break;

        default:    break;
    }   // End switch

//    arithObj.finish_encode(Writer);
//    arithObj.doneoutputtingbits(Writer);    // Encode the last bit
//    fclose(Writer);                         // Close compressed file

    tarIn.close();                      // Close target file

    averageEntropy = (double) (-1) * sumOfEntropies / symsNo;

    // Print reference and target file names
    u8 refsAdressesSize = (u8) getRefAddr().size();
    size_t lastSlash_Ref[refsAdressesSize];
    string refNamesPure[refsAdressesSize];
    for (u8 i = refsAdressesSize; i--;)
    {
        lastSlash_Ref[ i ] = getRefAddr()[ i ].find_last_of("/");
        refNamesPure[ i ]  = getRefAddr()[ i ].substr(lastSlash_Ref[ i ] + 1);
    }
    
    
    high_resolution_clock::time_point exeFinishTime =
            high_resolution_clock::now();   // Record end time
    
    mut.lock();//========================================================
    for (u8 i = 0; i < refsAdressesSize-1; ++i) cout << refNamesPure[i] << ',';
    // Calculate duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - startTime;

    cout << refNamesPure[ refsAdressesSize - 1 ] << '\t'
         << tarNamePure << '\t'
         //       << std::fixed << setprecision(5) << averageEntropy << '\t'
         << std::fixed << setprecision(4) << averageEntropy / LOG2_ALPH_SIZE
         << '\t' // NRC
         << getIR()[0]  << '\t'
         << (int) getCtxDepth()[0] << '\t'
         << std::fixed << setprecision(4) << alpha[0] << '\t'
         << std::fixed << setprecision(3) << elapsed.count()
         << '\n';

///    cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
    mut.unlock();//======================================================
}


/*******************************************************************************
    Read header for decompression
*******************************************************************************/
void FCM::extractHeader (const string &tarName)
{
    ArithEncDec arithObj;   // To work with arithmetic encoder/decoder class
    
    // Build TAR.co filename
    size_t lastSlash_Tar = tarName.find_last_of("/");       // Position
    string tarNamePure = tarName.substr(lastSlash_Tar + 1);
    const char *tarCo = (tarNamePure + COMP_FILETYPE).c_str(); // string->char*
    FILE *Reader = fopen(tarCo, "r");    // To process the compressed file

    // Starting
    arithObj.startinputtingbits();       // Start arithmetic decoding process
    arithObj.start_decode(Reader);
    
    // Extract header information
    if (arithObj.ReadNBits(26, Reader) != WATERMARK)      // Watermark check-in
    {
        cerr << "ERROR: Invalid compressed file!\n";
        exit(1);
    }
    arithObj.ReadNBits(46, Reader);     // File size
    this->setGamma( round((double) arithObj.ReadNBits(32, Reader)/65536 * 100)
                    / 100 );            // Gamma
    u64 no_models = (u64) arithObj.ReadNBits(16, Reader);  // Number of models
    this->n_models = (u8) no_models;
//    this->setN_models((u8) no_models);
    bool ir;    u8 k;   u16 aD;
    for (u8 n = 0; n < no_models; ++n)
    {
        ir = (bool) arithObj.ReadNBits(1,  Reader);
        k  = (u8)   arithObj.ReadNBits(16, Reader);
        aD = (u16)  arithObj.ReadNBits(16, Reader);
        this->pushParams(ir, k, aD);    // ir, ctx depth, alpha denom
    }
    char compMode = (char) arithObj.ReadNBits(16, Reader);  // Compression mode
    this->setCompMode(compMode);
    // Initialize vector of tables/hash tables
    compMode == 'h' ? this->initHashTables() : this->initTables();
    
    // Finishing
    arithObj.finish_decode();
    arithObj.doneinputtingbits();       // Decode last bit
    fclose(Reader);                     // Close compressed file
}


/*******************************************************************************
    Decompress target(s) based on reference(s) model
*******************************************************************************/
void FCM::decompress (const string &tarName)
{
    ArithEncDec arithObj;   // To work with arithmetic encoder/decoder class
    
    // Build TAR.co and TAR.de filenames
    size_t lastSlash_Tar = tarName.find_last_of("/");     // Position
    string tarNamePure = tarName.substr(lastSlash_Tar + 1);
    // Compressed file. convert string to char*
    const char *tarCo  = (tarNamePure + COMP_FILETYPE).c_str();
    const char *tarDe  = (tarNamePure + DECOMP_FILETYPE).c_str();
    FILE       *Reader = fopen(tarCo, "r");   // To process the compressed file
    
    mut.lock();//========================================================
    remove(tarDe);                  // Remove pre-existing decompressed file(s)
//    FILE *Reader = fopen(tarCo, "r");      // To process the compressed file
    FILE *Writer = fopen(tarDe, "w");      // To save decompressed file
    mut.unlock();//======================================================
    
    i32 idxOut = 0;
    char *outBuffer = (char *) calloc(BUFFER_SIZE, sizeof(uint8_t));

    arithObj.startinputtingbits();        // Start arithmetic decoding process
    arithObj.start_decode(Reader);
    
    // Extract header information
    arithObj.ReadNBits(26, Reader);     // Watermark
    u64 symsNo = (u64) arithObj.ReadNBits(46, Reader);  // Number of symbols
    arithObj.ReadNBits(32, Reader);     // Gamma
    arithObj.ReadNBits(16, Reader);                     // Number of models
    u8 no_models = this->getN_models();
    for (u8 n = 0; n < no_models; ++n)
    {
        arithObj.ReadNBits(1,  Reader);                 // Inverted repeats
        arithObj.ReadNBits(16, Reader);                 // Context depths
        arithObj.ReadNBits(16, Reader);                 // Alplha denoms
    }
    arithObj.ReadNBits(16, Reader);                     // Compression mode
    
    // Alpha and ALPH_SIZE*alpha: used in P numerator and denominator
    double alpha[no_models], sumAlphas[no_models];
    for (u8 i = no_models; i--;)
    {
        alpha[ i ] = (double) 1 / alphaDens[ i ];
        sumAlphas[ i ] = ALPH_SIZE * alpha[ i ];
    }
    
    u64 maxPlaceValue[no_models];
    for (u8 i = no_models; i--;)  maxPlaceValue[ i ] = POWER5[ ctxDepths[i] ];
    
    // Context(s) (integer) sliding through the dataset
    u64    tarContext[no_models];  fill_n(tarContext, no_models, 0);
    string tarLine;               // Keep each line of the file
    u64    nSym;                  // No. syms (n_s). in probability numerator
    u64    sumNSym;         // Sum of no. syms (sum n_a). in probability denom.
    double prob_i;                // Probability of a symbol for each model
    double rawWeight[no_models];  // Weight before normalization. init: 1/M
    double weight[no_models]; fill_n(weight, no_models, (double) 1/no_models);
    double sumOfWeights;          // Sum of weights. used for normalization
    double freqsDouble[ALPH_SIZE];// Frequencies of each symbol (double)
    int    freqs[ALPH_SIZE];      // Frequencies of each symbol (integer)
    int    sumFreqs;              // Sum of frequencies of each symbol
    u8     currSymInt;            // Current symbol in integer format
    
    switch (compMode)
    {
        case 't':
        {
            u64 rowIndex = 0;     // Index of a row in the table
            
            for (int k = 0; k < symsNo; ++k)
            {
                fill_n(freqsDouble, ALPH_SIZE, 0);    // Reset array of freqs
                
                // Decode first symbol
                for (u8 i = no_models; i--;)
                {
                    rowIndex = tarContext[ i ] * ALPH_SUM_SIZE;
    
                    for (u8 j = ALPH_SIZE; j--;)
                        freqsDouble[ j ] +=
                              weight[i] * this->getTables()[i][ rowIndex + j ];
                }
                // Frequencies (integer)
                for (u8 j = ALPH_SIZE; j--;)
                    freqs[ j ] = (int) (1 + freqsDouble[j] * DOUBLE_TO_INT);
                
                sumFreqs = 0;   for (int f : freqs) sumFreqs += f;
                // Arithmetic decoding
                currSymInt
                     = (u8) arithObj.ADSym(ALPH_SIZE, freqs, sumFreqs, Reader);
                
                outBuffer[ idxOut ] = symIntToChar(currSymInt);   // Out buffer
                
                if (++idxOut == BUFFER_SIZE)
                {   // Write output
                    fwrite(outBuffer, 1, (size_t) idxOut, Writer);
                    idxOut = 0;
                }
                
                sumOfWeights = 0;
                
                for (u8 i = no_models; i--;)
                {
                    rowIndex = tarContext[ i ] * ALPH_SUM_SIZE;
                    
                    nSym = tables[ i ][ rowIndex + currSymInt ];    // # syms
                    // Sum of number of symbols
                    sumNSym = tables[ i ][ rowIndex + ALPH_SIZE ];
                    // P(s|c^t)
                    prob_i = (nSym + alpha[ i ]) / (sumNSym + sumAlphas[ i ]);
                    
                    // Weight before normalization
                    rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
                    // Sum of weights. used for normalization
                    sumOfWeights = sumOfWeights + rawWeight[ i ];
                    
                    // Update context
                    //(rowIndex - tarContext[i]) = (tarContext[i] * ALPH_SIZE)
                    tarContext[ i ]
                                = (u64) (rowIndex - tarContext[i] + currSymInt)
                                          % maxPlaceValue[ i ];
                }
                // Update weights
                for (u8 i = no_models; i--;)
                    weight[ i ] = rawWeight[ i ] / sumOfWeights;
            }   // End for

            if (idxOut != 0)    fwrite(outBuffer, 1, (size_t) idxOut, Writer);
        }   // End case
            break;
        
        case 'h':
        {
            // Hash table row array -- to save a row of hTable
            array< u64, ALPH_SIZE > hTRowArray;
//            array< u16, ALPH_SIZE > hTRowArray;
            
            for (int k = 0; k < symsNo; ++k)
            {
                fill_n(freqsDouble, ALPH_SIZE, 0);    // Reset array of freqs
                
                // Decode first symbol
                for (u8 i = no_models; i--;)
                {
                    // Save the row of hash table into an array
                    hTRowArray = this->getHashTables()[ i ][ tarContext[ i ] ];
                    
                    for (u8 j = ALPH_SIZE; j--;)
                        freqsDouble[ j ] += weight[ i ] * hTRowArray[ j ];
                }
                // Frequencies (integer)
                for (u8 j = ALPH_SIZE; j--;)
                    freqs[ j ] = (int) (1 + freqsDouble[j] * DOUBLE_TO_INT);
                
                sumFreqs = 0;
                for (int f : freqs) sumFreqs += f;        // Sum of frequencies
                
                // Arithmetic decoding
                currSymInt
                     = (u8) arithObj.ADSym(ALPH_SIZE, freqs, sumFreqs, Reader);
                
                outBuffer[ idxOut ] = symIntToChar(currSymInt);   // Out buffer
                
                if (++idxOut == BUFFER_SIZE)
                {   // Write output
                    fwrite(outBuffer, 1, (size_t) idxOut, Writer);
                    idxOut = 0;
                }
                
                sumOfWeights = 0;
                
                for (u8 i = no_models; i--;)
                {
                    hTRowArray = this->getHashTables()[ i ][ tarContext[ i ]];
                    
                    sumNSym = 0; for (u64 u : hTRowArray) sumNSym = sumNSym + u;
                    nSym = hTRowArray[ currSymInt ];        // # syms
                    // P(s|c^t)
                    prob_i = (nSym + alpha[i]) / (sumNSym + sumAlphas[i]);
                    
                    // Weight before normalization
                    rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
                    // Sum of weights. used for normalization
                    sumOfWeights = sumOfWeights + rawWeight[ i ];
                    
                    // Update context
                   // (rowIndex - tarContext[i]) = (tarContext[i] * ALPH_SIZE)
                    tarContext[ i ]
                            = (u64) (tarContext[i] * ALPH_SIZE + currSymInt)
                                      % maxPlaceValue[ i ];
                }
                // Update weights
                for (u8 i = no_models; i--;)
                    weight[ i ] = rawWeight[ i ] / sumOfWeights;
            }   // End for
            
            if (idxOut != 0)    fwrite(outBuffer, 1, (size_t) idxOut, Writer);
        }   // End case
            break;
        
        default:    break;
    }   // End switch
    
    arithObj.finish_decode();
    arithObj.doneinputtingbits();    // Decode last bit
    fclose(Reader);                  // Close compressed file
    fclose(Writer);                  // Close decompressed file
}


/*******************************************************************************
    Convert char base to integer (u8): ACNGT -> 01234
*******************************************************************************/
inline u8 FCM::symCharToInt (char charSym) const
{
    switch (charSym)
    {
        case 'A':   return 0;
        case 'C':   return 1;
        case 'T':   return 4;
        case 'G':   return 3;
        default:    return 2;
        
//        case 'N':   return 2;
//        default:
//            cerr << "ERROR: unknown symbol '" << charSym << "'\n";
//            exit(1);
    }
    
    /*
    switch (ch)
    {
        case 'C':   return (u8) 3;
        case 'N':   return (u8) 2;
        default:    return (u8) (ch % ALPH_SIZE);
    }
     */
}


/*******************************************************************************
    Convert inteter (u8) base to char: 01234 -> ACNGT
*******************************************************************************/
inline char FCM::symIntToChar (u8 intSym) const
{
    switch (intSym)
    {
        case 0: return 'A';
        case 1: return 'C';
        case 4: return 'T';
        case 3: return 'G';
        case 2: return 'N';
        default: cerr << "ERROR: unknown integer '" << intSym << "'\n"; exit(1);
    }
}


/*******************************************************************************
    Fast power
    By Martin Ankerl
    http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp
*******************************************************************************/
inline double FCM::fastPow (double base, double exponent)
{
    union
    {
        double d;
        int x[2];
    } u = {base};
    
    u.x[1] = (int) (exponent * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    
    return u.d;
}


/*******************************************************************************
    Count number of symbols in a file
*******************************************************************************/
inline u64 FCM::countSymbols (const string &fileName)
{
    ifstream fileIn(fileName, ios::in); // Open file
    
    // Error handling
    if (!fileIn)
    {
        cerr << "The file '" << fileName
             << "' cannot be opened, or it is empty.\n";
        fileIn.close();                 // Close file
        exit(1);                        // Exit this function
    }
    
    string fileLine;    // Each line of file
    u64 symsNo = 0;     // Number of symbols
    
    while (getline(fileIn, fileLine))
        for (string::iterator it = fileLine.begin(); it != fileLine.end(); ++it)
            ++symsNo;
    
    return symsNo;
}


/*******************************************************************************
    Size of file, in bytes
*******************************************************************************/
inline u64 FCM::fileSize (const string &fileName)
{
    // ios::ate seeks to end immediately after opening
    ifstream fileIn( fileName, ios::in | ios::ate );
    return (u64) fileIn.tellg();
}


/*******************************************************************************
    Print hash table
*******************************************************************************/
inline void FCM::printHashTable (u8 idx) const
{
    htable_t hT = this->getHashTables()[ idx ];
    for (auto it = hT.begin(); it != hT.end(); ++it)
    {
        cout << it->first << "\t";
        for (u64 i : it->second)    cout << i << "\t";
        cout << '\n';
    }

//**********************************************************************

//    cout
//         << " >>> Context order size:\t" << (u16) contextDepth << '\n'
//         << " >>> Alpha denominator:\t\t" << (u16) alphaDenom << '\n'
//         << " >>> Inverted repeat:\t\t" << (invertedRepeat
//                                           ? "Considered" : "Not considered")
//         << '\n'
//         << " >>> file address:\t"
//         << "\n\n";
//
//    cout << "\tA\tC\tN\tG\tT"
//         //              << "\tP_A\tP_C\tP_N\tP_G\tP_T"
//         << '\n'
//         << "\t-----------------------------------"
//         //              << "------------------------------------------"
//         << '\n';
//
//    for (htable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
//    {
//        cout << it->first;
//        cout << "\t";
//        for (u64 i : it->second)    cout << i << "\t";
//        cout << '\n';
//    }
//    cout << '\n';
    
}


/*******************************************************************************
    Getters and setters
*******************************************************************************/
const vector<bool>&   FCM::getIR         () const   { return invRepeats;       }
const vector<u8>&     FCM::getCtxDepth   () const   { return ctxDepths;        }
const vector<string>& FCM::getTarAddr    () const   { return tarAddr;          }
const vector<string>& FCM::getRefAddr    () const   { return refAddr;          }
u64**                 FCM::getTables     () const   { return tables;           }
//u16**                 FCM::getTables     () const   { return tables;         }
htable_t*             FCM::getHashTables () const   { return hashTables;       }
bool  FCM::getDecompFlag                 () const   { return decompFlag;       }
u16   FCM::getN_models                   () const   { return n_models;         }
//u32   FCM::getN_div                      () const   { return n_div;          }
void  FCM::initTables     ()             { tables = new u64* [n_models];       }
//void  FCM::initTables     ()            { tables = new u16* [n_models];      }
void  FCM::initHashTables ()             { hashTables = new htable_t[n_models];}
void  FCM::setDecompFlag  (bool dF)      { FCM::decompFlag = dF;               }
void  FCM::setCompMode    (char cM)      { compMode = cM;                      }
//void  FCM::setN_models    (u16 n)        { n_models = n;                       }
void  FCM::setGamma       (double g)     { gamma = g;                          }
//void  FCM::setN_div       (u32 nD)      { n_div = nD;                        }
void  FCM::pushTarAddr    (const string &tFAs)      { tarAddr.push_back(tFAs); }
void  FCM::pushRefAddr    (const string &rFAs)      { refAddr.push_back(rFAs); }
void  FCM::setTable       (u64 *tbl, u8 idx)        { tables[ idx ] = tbl;     }
//void  FCM::setTable       (u16 *tbl, u8 idx)         { tables[ idx ] = tbl;  }
void  FCM::setHashTable   (const htable_t &ht, u8 idx) { hashTables[idx] = ht; }
void  FCM::pushParams     (bool iR, u8 ctx, u16 aD) { invRepeats.push_back(iR);
                            ctxDepths.push_back(ctx); alphaDens.push_back(aD); }