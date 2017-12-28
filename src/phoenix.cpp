#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>       // time
#include <iomanip>      // setw, setprecision
#include <thread>
#include <cmath>
#include <stdint.h>

#define __STDC_FORMAT_MACROS

#include <inttypes.h>

#if defined(_MSC_VER)
    #include <io.h>
#else
    #include <unistd.h>
#endif

#include "def.h"
#include "functions.h"
#include "FCM.h"

using std::cout;
using std::chrono::high_resolution_clock;
using std::setprecision;
using std::thread;


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
    auto startTime = high_resolution_clock::now();    // Start time

    FCM mixModel;       // Object on memory stack
    mixModel.startTime = startTime;

    // Parse the command line
    commandLineParser(argc, argv, mixModel);

//    thread *arrThread;
//
//    // Build reference(s) model(s) -- multithreaded
//    u16 n_models   = mixModel.n_models;
//    u16 n_threads  = mixModel.getN_threads();
//    u16 arrThrSize = (n_models > n_threads)
//                     ? n_threads : n_models;  // Size of threads array
//    arrThread = new thread[arrThrSize];
//    for (u16 i = 0; i < n_models; i += arrThrSize)
//    {
//        //TODO: aya jaygozini vase sharte "i+j < n_models" hast?
//        for (u16 j = 0; j < arrThrSize && i + j < n_models; ++j)
//            arrThread[j] = thread( &FCM::buildModel, &mixModel,
//                                   mixModel.(),
//                                   mixModel.invRepeats[i+j],
//                                   mixModel.ctxDepths[i+j], i+j );
//        for (u16 j = 0; j < arrThrSize && i+j < n_models; ++j)
//            if (arrThread[j].joinable())
//                arrThread[j].join();
//    }
//    delete[] arrThread;
//    /*
//    // compress target(s) using reference(s) model -- multithreaded
//    // max cores in current machine
//    u16 MAX_N_THREADS = (u16) thread::hardware_concurrency();
//    // N_FREE_THREADS considered for other jobs in current system
//    u16 n_threads_available = (u16) ( !MAX_N_THREADS
//                                      ? DEFAULT_N_THREADS - N_FREE_THREADS
//                                      : MAX_N_THREADS - N_FREE_THREADS );
//    u16 n_targets = (u16) mixModel.tarAddr.size(); //up to 2^16=65536 tars
//
//    u16 arrThrSize = (n_targets > n_threads_available)
//                      ? n_threads_available : n_targets;
//    thread *arrThread = new thread[arrThrSize];       // array of threads
//    */
//
//    // Compress target(s) using reference(s) model(s) -- multithreaded
//    u16 n_targets = (u16) mixModel.tarAddr.size();  // Up to 2^16=65536 targets
//    // Modify threads array size
//    arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
//    arrThread = new thread[arrThrSize];                // Resize threads array
//    for (u16 i = 0; i < n_targets; i += arrThrSize)
//    {
//        for (u16 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//            arrThread[ j ] = thread( &FCM::compress, &mixModel,
//                                     mixModel.tarAddr[i+j] );
//        for (u16 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//            if (arrThread[j].joinable())
//                arrThread[j].join();
//    }
//    delete[] arrThread;
//
////    // Decompress
////    if (mixModel.decompFlag)
////    {
////        FCM decModel;
////        // Reference(s) and target(s) address(es)
////        for (string s : mixModel.())  decModel.refAddr.push_back(s);
////        for (string s : mixModel.tarAddr)  decModel.tarAddr.push_back(s);
////
////        // Extract header information
////        decModel.extractHeader(decModel.tarAddr[ 0 ]);
////
////        // Build reference(s) model(s) -- multithreaded
////        n_models   = mixModel.n_models;
////        n_threads  = mixModel.getN_threads();      // Set based on command line
////        arrThrSize = (n_models > n_threads)
////                     ? n_threads : n_models;       // Size of threads array
////        arrThread = new thread[arrThrSize];
////        for (u16 i = 0; i < n_models; i += arrThrSize)
////        {
////            for (u16 j = 0; j < arrThrSize && i + j < n_models; ++j)
////                arrThread[ j ] = thread( &FCM::buildModel, &decModel,
////                                         decModel.refAddr,
////                                         decModel.invRepeats[i+j],
////                                         decModel.ctxDepths[i+j], i + j );
////            for (u16 j = 0; j < arrThrSize && i + j < n_models; ++j)
////                if (arrThread[j].joinable())
////                    arrThread[j].join();
////        }
////        delete[] arrThread;
////
////        // Decompress target(s) using reference(s) model(s) -- multithreaded
////        // Modify thread array size
////        arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
////        arrThread = new thread[arrThrSize];
////        for (u16 i = 0; i < n_targets; i += arrThrSize)
////        {
////            for (u16 j = 0; j < arrThrSize && i + j < n_targets; ++j)
////                arrThread[ j ] = thread( &FCM::decompress, &decModel,
////                                        decModel.tarAddr[i+j] );
////            for (u16 j = 0; j < arrThrSize && i + j < n_targets; ++j)
////                if (arrThread[j].joinable())
////                    arrThread[j].join();
////        }
////        delete[] arrThread;
////
////        // Check equality of decompressed & tar. files (check lossless comp.)
////        for (string s : decModel.tarAddr)
////            if (!areFilesEqual(s, s + DECOMP_FILETYPE))
////            {
////                cerr << "Lossless compression/decompression of '" << s
////                     << "' failed.\n";
////                exit(1);
////            }
////        u8 tarsNo = (u8) decModel.tarAddr.size();
////        size_t lastSlashPos;
////        string tarNamesPure[tarsNo];
////        for (u8 i = tarsNo; i--;)
////        {
////            lastSlashPos = decModel.tarAddr[ i ].find_last_of("/");
////            tarNamesPure[i] = decModel.tarAddr[i].substr(lastSlashPos + 1);
////        }
////        cout << "Lossless compression and decompression of '";
////        for (int i = 0; i < tarsNo-1; ++i) cout << tarNamesPure[ i ] << "', '";
////        cout << tarNamesPure[ tarsNo - 1 ] << "' was successful.\n";
////    }   // end Decompress
////
////
////    high_resolution_clock::time_point exeFinishTime =
////            high_resolution_clock::now();   // Record end time
////    // Calculate and show duration in seconds
////    std::chrono::duration< double > elapsed = exeFinishTime - startTime;
////
////    cout  // Elapsed time depends on slowest thread
////            << "Elapsed time: "
////            << '\t'
////            << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    return 0;
}