#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H

#include <iostream>
#include <vector>
#include <mutex>

#include "def.h"

using std::string;
using std::vector;
using std::chrono::high_resolution_clock;


class FCM : public InArgs
{
public:
    //todo. after defining a compress and decompress class, "gamma" should
    //todo. be defined in decompress class and not compress class
    double         gamma;                    // Gamma (for mixture of FCMs)
    vector<string> tarAddr;                  // Target files addresses
    vector<string> refAddr;                  // Reference files addresses
    
    
    
    high_resolution_clock::time_point startTime;     // Start time
    char      compMode;                      // Comp mode (table / hash table)
    u64**     tables;                        // Table(s)
    htable_t* hashTables;                    // Hash table(s)
//    u32            n_div;                  // No. divisions done for no. syms
    
    FCM                    ();               // Constructor
    void   initTables      ();               // Initialize vector of tables
    void   initHashTables  ();               // Initialize vector of hash tables
    void   buildModel      (vector<string> const&, bool, u8, u16);
                                             // Build reference(s) model
    void   compress        (string const&);  // Compress target file
    void   extractHeader   (string const&);  // Header inf. for decomp.
    void   decompress      (string const&);  // Decompress target file
    inline u64    fileSize (string const&);  // Size of file
//    void   buildHashTable_str ();            // Build hash table (string key)

private:
    std::mutex    mut;                               // Mutex
    
    inline char   symIntToChar   (u8)       const;   // 01234 -> ACNGT
    inline u8     symCharToInt   (char)     const;   // ACNGT -> 01234
    inline void   printHashTable (u8)       const;   // Print hash table
    inline double fastPow        (double, double);   // Fast power
    inline u64    countSymbols   (string const&);    // Count no. syms in a file
};

#endif //PHOENIX_FCM_H