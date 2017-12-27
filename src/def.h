#ifndef PHOENIX_DEF_H
#define PHOENIX_DEF_H

#include <unordered_map>    // hash table

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

constexpr u8 ALPH_SIZE = 5;           // alphabet {A, C, N, G, T} size
typedef unordered_map<u64, array<u64, ALPH_SIZE>> htable_t;


/*******************************************************************************
    Constants
*******************************************************************************/
#define ALPH_SUM_SIZE     6           // ALPHABET_SIZE+1: 1 more col. for 'sum'
#define LOG2_ALPH_SIZE    2.3219281   // log2 of 5 (ALPHABET_SIZE)
#define TABLE_MAX_CTX     12          // max context depth for building table
//#define TABLE_MAX_CTX     0          // max context depth for building table
#define IR_MAGIC_NUM      4           // for IR calculation

#define DEFAULT_N_THREADS 2           // default number of threads
#define N_FREE_THREADS    1
#define DEFAULT_GAMMA     0.95        // default gamma (mixture of FCMs)

#define COMP_FILETYPE     ".co"       // compressed file type
#define DECOMP_FILETYPE   ".de"       // decompressed file type
#define WATERMARK         20170324    // used in compression/decompression
#define BUFFER_SIZE       262144      // used in decompression

#define DOUBLE_TO_INT     65535       // for converting double to integer
//#define MAX_INT           2147483647 // maximum possible integer = 2^31-1
#define MAX_INT           1073741823  // maximum possible integer = 2^30-1
// max no. of bases allowed for sum col. of table = (MAX_INT-1)/65535 = 2^15
#define MAX_N_BASE_SUM    16384
//#define MAX_N_BASE_SUM    32768
// max no. of bases allowed for each cell = MAX_N_BASE_SUM / 5
#define MAX_N_BASE        3276
//#define MAX_N_BASE        6553


/*******************************************************************************
    Compile-time functions
*******************************************************************************/
// Power: Base^Exponent
template <typename T>
constexpr T cPower(T b, u64 e, T result=1)
{
    return (e<1 ? result : cPower(b*b, e/2, (e%2) ? result*b : result));
}

//template <typename T>
//constexpr T cPower(T b, u64 e) { return (e==0) ? 1 : b*cPower(b, e-1); }


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


#endif //PHOENIX_DEF_H