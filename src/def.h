#ifndef PHOENIX_DEF_H
#define PHOENIX_DEF_H

#include <unordered_map>    // Hash table

using std::cout;
using std::string;
using std::unordered_map;
using std::array;

/*******************************************************************************
    Phoenix version
*******************************************************************************/
const string MONTH     = "01";
const string YEAR      = "18";
const string VERSION   = YEAR + "." + MONTH;
const string DEV_YEARS = "2016-2018";


/*******************************************************************************
    Typedefs
*******************************************************************************/
typedef unsigned char       u8;
typedef unsigned short int  u16;
typedef unsigned int        u32;
typedef unsigned long int   u64;
typedef signed char         i8;
typedef short int           i16;
typedef int                 i32;
typedef long int            i64;

// vaghti bishtar az 65,535 (uint16_t max) shod, hameye adad ha nesf mishan.
// ye variable ro ham tanzim kon ke maloom she chand bar nesf kardim
//typedef unordered_map< string, array< u64, ALPH_SIZE > > htable_str_t;
//typedef unordered_map< u64 , array< u16, ALPH_SIZE > > htable_t;

constexpr u8 ALPH_SIZE = 5;           // Alphabet {A, C, N, G, T} size
typedef unordered_map<u64, array<u64, ALPH_SIZE>> htable_t;


/*******************************************************************************
    Constants
*******************************************************************************/
constexpr double LOG2_ALPH_SIZE  = 2.3219281;   // log2 of 5 (ALPHABET_SIZE)
constexpr u8     ALPH_SUM_SIZE   = 6;           // ALPHABET_SIZE+1: 1 more col. for 'sum'
constexpr u8     TABLE_MAX_CTX   = 12;         // Max context depth for building table
constexpr u8     IR_MAGIC_NUM    = 4;          // For IR calculation
constexpr u8     DEFAULT_N_THR   = 2;           // Default number of threads
constexpr u8     N_FREE_THREADS  = 1;
constexpr double DEFAULT_GAMMA   = 0.95;       // Default gamma (mixture of FCMs)
const     string COMP_FILETYPE   = ".co";       // Compressed file type
const     string DECOMP_FILETYPE = ".de" ;      // Decompressed file type
constexpr u64    WATERMARK       = 20180101;   // Used in compression/decompression
constexpr u64    BUFFER_SIZE     = 262144;      // Used in decompression
constexpr u64    DOUBLE_TO_INT   = 65535;       // For converting double to integer
//constexpr u64 MAX_INT            = 2147483647; // Maximum possible integer = 2^31-1
constexpr u64    MAX_INT         = 1073741823;  // Maximum possible integer = 2^30-1
// Max no. of bases allowed for sum col. of table = (MAX_INT-1)/65535 = 2^15
constexpr u64    MAX_N_BASE_SUM  = 16384;
//constexpr u64 MAX_N_BASE_SUM     = 32768;
// Max no. of bases allowed for each cell = MAX_N_BASE_SUM / 5
constexpr u64    MAX_N_BASE      = 3276;
//constexpr u64    MAX_N_BASE      = 6553;


/*******************************************************************************
    Command line input arguments
*******************************************************************************/
struct InArgs
{
    static bool   VERBOSE;            // Verbose mode
    static bool   DECOMP_FLAG;        // Decompression flag
    static u8     N_THREADS;          // # threads
    static u8     N_MODELS;           // # models
    static double   GAMMA;        // Gamma (for mixture of FCMs)
//    static string IN_FILE_NAME;       /**< @brief Input file name */
//    static string KEY_FILE_NAME;      /**< @brief Password file name */
};


/*******************************************************************************
    Compile-time functions
*******************************************************************************/
// Power: Base^Exponent
template <typename T>
constexpr T POWER(T b, u64 e, T result=1)
{
    return (e<1 ? result : POWER(b*b, e>>1, e&1 ? result*b : result));
}

//template <typename T>
//constexpr T POWER(T b, u64 e) { return (e==0) ? 1 : b*POWER(b, e-1); }


/*******************************************************************************
    Lookup tables
*******************************************************************************/
static const u64 POWER5[28] =    // 5^0 to 5^27, which needs less than 64 bits
    {
                           1,                  5,                  25,
                         125,                625,                3125,
                       15625,              78125,              390625,
                     1953125,            9765625,            48828125,
                   244140625,         1220703125,          6103515625,
                 30517578125,       152587890625,        762939453125,
               3814697265625,     19073486328125,      95367431640625,
             476837158203125,   2384185791015625,   11920928955078100,
           59604644775390600, 298023223876953000, 1490116119384770000,
         7450580596923830000
    };


/*******************************************************************************
    Usage guide
*******************************************************************************/
static void help ()
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
        << "      Copyright (C) " << DEV_YEARS
                                  << ", IEETA, University of Aveiro."   << '\n'
        << "      This is a Free software, under GPLv3. You may redistribute\n"
        << "      copies of it under the terms of the GNU - General Public  \n"
        << "      License v3 <http://www.gnu.org/licenses/gpl.html>. There  \n"
        << "      is NOT ANY WARRANTY, to the extent permitted by law." << '\n';
    
    exit(1);
}

#endif //PHOENIX_DEF_H