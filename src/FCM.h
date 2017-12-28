#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H

#include <iostream>
#include <vector>
#include <mutex>

#include "def.h"

using std::string;
using std::vector;
using std::chrono::high_resolution_clock;


class FCM
{
public:
    high_resolution_clock::time_point startTime;    // Start time
    u16            n_threads;          // # threads
    u8             n_models;           // # models
    bool           decompFlag;         // Decompress flag
    char           compMode;           // Compression mode (table / hash table)
    double         gamma;              // Gamma (for mixture of FCMs)
    vector<bool>   invRepeats;         // Inverted repeat(s)
    vector<u8>     ctxDepths;          // Context depthe(s) (SIZE <= 255)
    vector<u16>    alphaDens;          // Alpha denominator(s)
    vector<string> tarAddr;            // Target files addresses
    vector<string> refAddr;            // Reference files addresses
    u64**          tables;             // Table(s)
    htable_t*      hashTables;         // Hash table(s)
//    u32            n_div;              // No. divisions done for no. syms
    
    FCM ();                                // Constructor
    void   initTables          ();         // Initialize vector of tables
    void   initHashTables      ();         // Initialize vector of hash tables
                                                       
    void   buildModel          (const vector<string>&,
                                bool, u8, u16);     // Build reference(s) model
    void   compress            (const string&);     // Compress target file
    void   extractHeader       (const string&);     // Header inf. for decomp.
    void   decompress          (const string&);     // Decompress target file
    
    inline char   symIntToChar (u8)        const;   // 01234 -> ACNGT
    inline u8     symCharToInt (char)      const;   // ACNGT -> 01234
    inline double fastPow      (double, double);    // Fast power
    inline u64    fileSize     (const string&);     // Size of file
    inline u64    countSymbols (const string&);     // Count no. syms in a file
           
//    void buildHashTable_str ();             // Build hash table (string key)
    inline void   printHashTable (u8)      const;   // Print hash table

private:
    std::mutex     mut;                // Mutex
};

#endif //PHOENIX_FCM_H