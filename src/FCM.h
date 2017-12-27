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
    
    // getters and setters
    const vector<bool>&   getIR         () const;   // Get inverted repeat(s)
    const vector<u8>&     getCtxDepth   () const;   // Get context depth(es)
    const vector<string>& getTarAddr    () const;   // Get tar. files addresses
    const vector<string>& getRefAddr    () const;   // Get ref. files addresses
    u64**                 getTables     () const;  // Get table(s)
//    u16**                 getTables     () const;   // Get table(s)
    htable_t*             getHashTables () const;   // Get hash table(s)
    bool  getDecompFlag                 () const;   // Get decompress flag
    u16   getN_threads                  () const;   // Get number of threads
    u16   getN_models                   () const;   // Get number of models
//    u32   getN_div                      () const; // Get number of divisions
    void  setStartTime    (const high_resolution_clock::time_point&);
    void  setDecompFlag   (bool);                   // Set decompress flag
    void  setN_threads    (u16);                    // Set number of threads
    void  setCompMode     (char);                   // Set compression mode
    void  setN_models     (u16);                    // Set number of models
    void  setGamma        (double);                 // Set gamma
//    void  setN_div        (u32);                   // Set number of divisions
    void  pushTarAddr     (const string&);          // Push back tars. addr.
    void  pushRefAddr     (const string&);          // Push back refs. addr.
    void  setTable        (u64*, u8);               // Set table(s)
//    void  setTable        (u16*, u8);               // Set table(s)
    void  setHashTable    (const htable_t&, u8);    // Set hash table(s)
    // push back model(s) params (ir, ctx_depth, alpha_denom)
    void  pushParams      (bool, u8, u16);
   
   
//   double getGamma ()    { return gamma; }
//   vector<u16> getAlpha ()    { return alphaDens; }
//
//   char getCompmode ()    { return compMode; }


private:
    std::mutex     mut;                // Mutex
    high_resolution_clock::time_point startTime;   // Start time
    
    u16            n_threads;          // Number of threads
    bool           decompFlag;         // Decompress flag
    
    char           compMode;           // Compression mode (table / hash table)
    u16            n_models;           // Number of models
    double         gamma;              // Gamma (for mixture of FCMs)
    vector<bool>   invRepeats;         // Inverted repeat(s)
    vector<u8>     ctxDepths;          // Context depthe(s) (SIZE <= 255)
    vector<u16>    alphaDens;          // Alpha denominator(s)
//    u32            n_div;              // No. divisions done for no. syms
    vector<string> tarAddr;            // Target files addresses
    vector<string> refAddr;            // Reference files addresses
    u64**          tables;             // Table(s)
//    u16**          tables;             // Table(s)
    htable_t*      hashTables;         // Hash table(s)
//    htable_str_t hashTable_str;      // Hash table (string key)
};

#endif //PHOENIX_FCM_H