#ifndef PHOENIX_DEF_H
#define PHOENIX_DEF_H


#include <chrono>           // time
#include <unordered_map>    // hash table

using std::string;
using std::unordered_map;
using std::array;


/***********************************************************
    About PHOENIX
************************************************************/
#define VERSION_PHOENIX 1
#define RELEASE_PHOENIX 0


/***********************************************************
    constant
************************************************************/
#define ALPHABET_SIZE   5     // alphabet {A, C, N, G, T}


/***********************************************************
    typedef
************************************************************/
// TODO: vaghti bishtar az 65,535 (uint16_t max) shod, hameye adad ha nesf mishan.
// ye variable ro ham tanzim kon ke maloom she chand bar nesf kardim
typedef unordered_map< string, array<uint16_t, ALPHABET_SIZE> > htable_t;


#endif //PHOENIX_DEF_H