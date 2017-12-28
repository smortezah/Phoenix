#ifndef PHOENIX_ARITHENCDEC_H
#define PHOENIX_ARITHENCDEC_H

/*******************************************************************************
  Armando J. Pinho	(ap@ua.pt)
  Morteza Hosseini	(seyedmorteza@ua.pt)

  University of Aveiro, DETI/IEETA, 3810-193 Aveiro, Portugal
  December 1999, March 2017

Adapted based on:

Authors:  John Carpinelli   (johnfc@ecr.mu.oz.au)
          Wayne Salamonsen  (wbs@mundil.cs.mu.oz.au)
          Lang Stuiver      (langs@cs.mu.oz.au)
          Radford Neal      (radford@ai.toronto.edu)

Purpose:  Data compression using a revised arithmetic coding method.

Based on: A. Moffat, R. Neal, I.H. Witten, "Arithmetic Coding Revisted",
          Proc. IEEE Data Compression Conference, Snowbird, Utah, March 1995.

          Low-Precision Arithmetic Coding Implementation by Radford M. Neal

Copyright 1995 John Carpinelli and Wayne Salamonsen, All Rights Reserved.
Copyright 1996 Lang Stuiver, All Rights Reserved.

These programs are supplied free of charge for research purposes only,
and may not sold or incorporated into any commercial product. There is
ABSOLUTELY NO WARRANTY of any sort, nor any undertaking that they are
fit for ANY PURPOSE WHATSOEVER.  Use them at your own risk. If you do
happen to find a bug, or have modifications to suggest, please report
the same to Alistair Moffat, alistair@cs.mu.oz.au. The copyright
notice above and this statement of conditions must remain an integral
part of each and every copy made of these files.

  $Log: arith.c,v $
  Revision 1.2  2008-01-23 15:59:09  ap
  No real changes...

  Revision 1.1  2005/07/01 09:22:27  ap

  New files.

  Revision 1.1  1996/08/07 01:34:11  langs
  Initial revision
*******************************************************************************/


#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstdint>

#include "def.h"

using std::cerr;


/*******************************************************************************
    Bit and byte input output functions.
    Input/Output to stdin/stdout 1 bit at a time.
    Also byte i/o and fread/fwrite, so can keep a count of bytes read/written

    Once bit functions are used for either the input or output stream, byte
    based functions are NOT safe to use, unless a flush_{input,output}stream is
    first called. (Since bits are buffered a char at a time, while bytes are
    output immediately)
*******************************************************************************/
#define BYTE_SIZE 8

extern unsigned int	_bytes_input, _bytes_output;

extern int _in_buffer;              // Input buffer
extern unsigned char _in_bit_ptr;   // Input bit pointer
extern int _in_garbage;		        // # of bytes read past EOF

extern int _out_buffer;             // Output buffer
extern int _out_bits_to_go;         // Output bits in buffer

#ifndef FAST_BITIO
extern int _bitio_tmp;              // Used by i/o macros to
#endif								// Keep function ret values


/*******************************************************************************
    #define FAST_BITIO
    Normally count input and output bytes so program can report stats With
    FAST_BITIO set, no counting is maintained, which means file sizes reported
    with the '-v' option will be meaningless, but should improve speed slightly
*******************************************************************************/
#ifdef FAST_BITIO
    #define OUTPUT_BYTE(x, s) 				putc(x, s)
	#define INPUT_BYTE(s)	  				getc(s)
	#define BITIO_FREAD(ptr, size, nitems)	fread(ptr, size, nitems, stdin)
	#define BITIO_FWRITE(ptr, size, nitems) fwrite(ptr, size, nitems, stdout)
#else
#define OUTPUT_BYTE(x, s)   ( _bytes_output++, putc(x, s) )

#define INPUT_BYTE(s)       ( _bitio_tmp = getc(s), 			             \
			                    _bytes_input += (_bitio_tmp == EOF) ? 0 : 1, \
			                    _bitio_tmp )

/// Return result of fread
#define BITIO_FREAD(ptr, size, nitems)			        \
		( _bitio_tmp = fread(ptr, size, nitems, stdin), \
		  _bytes_input += _bitio_tmp * size,			\
	  	  _bitio_tmp )

/// Return result of fwrite
#define BITIO_FWRITE(ptr, size, nitems)				    \
	( _bitio_tmp = fwrite(ptr, size, nitems, stdout),	\
	  _bytes_output += _bitio_tmp * size,			    \
	  _bitio_tmp )
#endif


// The following variables are supposedly local, but actually global so they can
// be referenced by macro
unsigned int _bytes_input  = 0;
unsigned int _bytes_output = 0;

int _in_buffer;					// I/O buffer
unsigned char _in_bit_ptr = 0;	// Bits left in buffer
int _in_garbage;				// Bytes read beyond eof

int _out_buffer;				// I/O buffer
int	_out_bits_to_go;			// Bits to fill buffer

#ifndef FAST_BITIO
int _bitio_tmp;					// Used by some of the
#endif


/* ================= USER ADJUSTABLE PARAMETERS =================== */
/*******************************************************************************
    Default B_bits and F_bits
*******************************************************************************/
#ifndef B_BITS
#define B_BITS 32
#endif

#ifndef F_BITS
#define F_BITS 27
#endif


/*******************************************************************************
    Change these types for different precision calculations. They may affect
    the speed of the arithmetic operations (multiplcation, division, shift,
    etc). The way the stats module is implemented, the type of freq_value must
    be able to accomodate f_bits+1 bits, instead of f_bits, to avoid overflows.
    Ie: For an f_bits of up to 31, type freq_value must be 32 bits.
*******************************************************************************/
typedef unsigned long	code_value;		// B_BITS of precision
typedef unsigned long	freq_value;		// F_BITS+1 of precision
typedef unsigned long	div_value;		// B_BITS-F_BITS of precision


/*******************************************************************************
    MAX_BITS_OUTSTANDING is a bound on bits_outstanding
    If bits_outstanding ever gets above this number (extremely unlikely) the
    program will abort with an error message.
*******************************************************************************/
#define MAX_BITS_OUTSTANDING ((unsigned long)1<<31)
/* ================= END USER ADJUSTABLE PARAMETERS =================== */


/*******************************************************************************
    Determine maximum bits allowed, based on size of the types used to store
    them. Also, that MAX_F_BITS <= MAX_B_BITS-2
*******************************************************************************/
#define MAX_B_BITS (int) (sizeof(code_value) * 8)
#define MAX_F_BITS (int)((sizeof(freq_value)*8)-1 < MAX_B_BITS - 2\
					  ?  (sizeof(freq_value)*8)-1 : MAX_B_BITS - 2)


/*******************************************************************************
    Determine maximum bits allowed, based on size of the types used. If varying
    bits, variables are B_bits, F_bits, Half and Quarter,otherwise #define them.
    These variables will be read (and possibly changed) by main.c and stats.c
*******************************************************************************/
#define B_bits B_BITS
#define	F_bits F_BITS

extern char *coder_desc;


#define Half	((code_value) 1 << (B_bits-1))
#define Quarter ((code_value) 1 << (B_bits-2))


/*******************************************************************************
    Separate input and output
*******************************************************************************/
// Input decoding state
static code_value in_R;						// Code range
static code_value in_D;						// = V-L (V offset)
static div_value  in_r;						// Normalized range

// Output encoding state
static code_value out_L;					// Lower bound
static code_value out_R;					// Code range
static unsigned long out_bits_outstanding;	// Follow bit count


/////---------------------------------------------------------------------------


class ArithEncDec
{
public:
    ArithEncDec ();                                      // Constructor

    void GetInterval         (int*, int*, int*, u8);
    u8   GetSymbol           (int*, int*, int*, int, u8);
    void WriteNBits          (u64,  int,  FILE*);    // Write N bits to a file
    int  ReadNBits           (u32,  FILE*);          // Read N bits from a file
    void AESym               (u8,   int*, int,  FILE*);  // Arithmetic encoding
    u8   ADSym               (u8,   int*, int,  FILE*);  // Arithmetic decoding
    void start_encode        ();
    void finish_encode       (FILE*);
    void start_decode        (FILE*);
    void finish_decode       ();
    void arithmetic_encode   (freq_value, freq_value, freq_value, FILE*);
    void arithmetic_decode   (freq_value, freq_value, freq_value, FILE*);
    freq_value arithmetic_decode_target (freq_value);
    void startoutputtingbits ();
    void startinputtingbits  ();
    void doneoutputtingbits  (FILE*);
    void doneinputtingbits   ();

//    void binary_arithmetic_encode(freq_value, freq_value, int, FILE*);
//    int  binary_arithmetic_decode(freq_value, freq_value, FILE*);

private:
    inline void OUTPUT_BIT           (int, FILE*);
    inline void ADD_NEXT_INPUT_BIT   (code_value&, int, FILE*);
    inline void ORIG_BIT_PLUS_FOLLOW (int, FILE*);
    inline void BIT_PLUS_FOLLOW      (int, FILE*);
    inline void ENCODE_RENORMALISE   (FILE*);
    inline void DECODE_RENORMALISE   (FILE*);
};

/*******************************************************************************
    Constructor
*******************************************************************************/
ArithEncDec::ArithEncDec () {}


/*******************************************************************************

*******************************************************************************/
void ArithEncDec::GetInterval (int *low, int *high, int *count, u8 symbol)
{
	*low = 0;	for (u8 n = 0; n < symbol; n++) *low += count[ n ];
	*high = *low + count[ symbol ];
}


/*******************************************************************************

*******************************************************************************/
u8 ArithEncDec::GetSymbol (int *low, int *high, int *count,
						   int target, u8 nSymbols)
{
	u8 n;
	
	*low = 0;
	for (n = 0; n < nSymbols; n++)
	{
		if (*low + count[ n ] > target)	break;
		*low += count[ n ];
	}
	*high = *low + count[ n ];

	return n;
}


/*******************************************************************************

*******************************************************************************/
void ArithEncDec::WriteNBits (u64 bits, int nBits, FILE *oFp)
{
	while (nBits--)
	{
		if ((bits >> nBits) & 0x1)  arithmetic_encode(1, 2, 2, oFp);
		else                        arithmetic_encode(0, 1, 2, oFp);
	}
}


/*******************************************************************************

*******************************************************************************/
int ArithEncDec::ReadNBits (u32 nBits, FILE *iFp)
{
	int bits = 0;
	int target, low, high, count[2] = {1, 1};

	while (nBits--)
	{
		bits <<= 1;
		target = arithmetic_decode_target(2);
		bits |= GetSymbol(&low, &high, count, target, 2);
		arithmetic_decode(low, high, 2, iFp);
	}

	return bits;
}


/*******************************************************************************

*******************************************************************************/
void ArithEncDec::AESym (u8 symbol, int *counters, int totalCount, FILE *oFp)
{
	int low, high;

	GetInterval(&low, &high, counters, symbol);
	arithmetic_encode(low, high, totalCount, oFp);
}


/*******************************************************************************

*******************************************************************************/
u8 ArithEncDec::ADSym (u8 nSymbols, int *counters, int totalCount, FILE *iFp)
{
	int low, high;

	u8 symbol = GetSymbol(&low, &high, counters,
						  arithmetic_decode_target(totalCount), nSymbols);
	arithmetic_decode(low, high, totalCount, iFp);

	return symbol;
}


/*******************************************************************************
    Encode a symbol given its low, high and total frequencies
*******************************************************************************/
void ArithEncDec::arithmetic_encode (freq_value low, freq_value high,
									 freq_value total, FILE *s)
{
	/*
    The following pseudocode is a concise (but slow due to arithmetic
    calculations) description of what is calculated in this function. Note that
    the division is done before the multiplication. This is one of the key
    points of this version of arithmetic coding. This means that much larger
    frequency values can be accomodated, but that the integer ratio R/total
    (code range / frequency range) becomes a worse approximation as the total
    frequency count is increased. Therefore less than the whole code range will
    be allocated to the frequency range. The whole code range is used by
    assigning the symbol at the end of the frequency range (where high==total)
    this excess code range above and beyond its normal code range. This
    of course distorts the probabilities slightly, see the paper connected with
    this program for details and compression results.

    Restricting the total frequency range means that the division and
    multiplications can be done to low precision with shifts and adds.

    The following code describes this function:
    (The actual code is only written less legibly to improve speed)

       L += low*(R/total);		// Adjust low bound

	   // Restrict range. If symbol at end of range.
	   // Restrict & allocate excess codelength to it.
       if (high < total) R = (high-low) * (R/total);
       else				 R = R - low * (R/total);

       ENCODE_RENORMALISE;		// Expand code range and output bits

	   // EXTREMELY improbable
       if (bits_outstanding > MAX_BITS_OUTSTANDING)	abort();
    */

	code_value temp;

	{
		div_value out_r;
		out_r = out_R/total;		// Calc range:freq ratio
		temp = out_r*low;			// Calc low increment
		out_L += temp;				// Increase L
		if (high < total)	out_R = out_r*(high-low);  // Restrict R
		else				out_R -= temp;      // If at end of freq range
		/* Give symbol excess code range  */
	}

    ENCODE_RENORMALISE(s);

	if (out_bits_outstanding > MAX_BITS_OUTSTANDING)
	{
		/*
        For MAX_BITS_OUTSTANDING to be exceeded is extremely improbable, but it
        is possible. For this to occur the COMPRESSED file would need to
        contain a sequence MAX_BITS_OUTSTANDING bits long (eg: 2^31 bits, or 256
        megabytes) of all 1 bits or all 0 bits. This possibility is extremely
        remote (especially with an adaptive model), and can only occur if the
        compressed file is greater than MAX_BITS_OUTSTANDING long. Assuming the
        compressed file is effectively random, the probability for any 256
        megabyte section causing an overflow would be 1 in 2^(2^31). This is a
        number approximately 600 million digits long (decimal).
        */

		cerr << "Bits_outstanding limit reached - File too large\n";
		exit(1);
	}
}


/*******************************************************************************
    Decode the target value using the current total frequency and the coder's
    state variables

    The following code describes this function:
    The actual code is only written less legibly to improve speed, including
    storing the ratio in_r = R/total for use by arithmetic_decode()

    target = D / (R/total);	* D = V-L.  (Old terminology)
       * D is the location within the range R that the code value is located
       * Dividing by (R/total) translates it to its correspoding frequency value

    The approximate calculations mean the encoder may have coded outside the
    valid frequency range (in the excess code range). This indicates that the
    symbol at the end of the frequency range was coded. Hence return end of
    range (total-1)

    if (target < total) return target;
    else                return total-1;
*******************************************************************************/
freq_value ArithEncDec::arithmetic_decode_target (freq_value total)
{
	freq_value target;

	in_r = in_R/total;
	target = (in_D)/in_r;

	return (target >= total ? total-1 : target);
}


/*******************************************************************************
    Decode the next input symbol

    The following code describes this function:
    The actual code is only written less legibly to improve speed, See the
    comments for arithmetic_encode() which is essentially the same process.

    D -= low * (R/total);    // Adjust current code value

    if (high < total) R = (high-low) * (R/total);   // Adjust range
    else			  R -= low * (R/total);    // End of range is a special case
 
    DECODE_RENORMALISE;      // Expand code range and input bits
*******************************************************************************/
void ArithEncDec::arithmetic_decode (freq_value low, freq_value high,
									 freq_value total, FILE *s)
{
	code_value temp;

	/// Assume r has been set by decode_target
	temp = in_r*low;
	in_D -= temp;
	if (high < total)   in_R = in_r*(high-low);
	else                in_R -= temp;

    DECODE_RENORMALISE(s);
}


/*******************************************************************************
    Start the encoder
*******************************************************************************/
void ArithEncDec::start_encode ()
{
	/// Set initial coding range to [0,Half)
	out_L = 0;
	out_R = Half;
	out_bits_outstanding = 0;
}


/*******************************************************************************
    Finish encoding by outputting follow bits and all the bits in
    L (B_bits long) to make the last symbol unambiguous. This also means the
    decoder can read them harmlessly as it reads beyond end of what would have
    been valid input.
*******************************************************************************/
void ArithEncDec::finish_encode (FILE *s)
{
	int nbits;
	code_value bits;

	nbits = B_bits;
	bits  = out_L;

	/// Output the nbits integer bits
	for (int i = 1; i <= nbits; i++)
		BIT_PLUS_FOLLOW(((bits >> (nbits - i)) & 1), s);
}


/*******************************************************************************
    Start the decoder Fills the decode value in_D from the bitstream.
    If FRUGAL_BITS is defined only the first call reads in_D from the bitstream.
    Subsequent calls will assume the excess bits that had been read but not used
    (sitting in in_V) are the start of the next coding message, and it will put
    these into in_D. It also initialises in_V to the start of the bitstream.

    FRUGAL_BITS also means that the first bit (0) was not output, so only take
    B_bits-1 from the input stream. Since there are B_bits "read-ahead" in the
    buffer, on second and subsequent calls retrieve_excess_input_bits() is used
    and the last bit must be placed back into the input stream.
*******************************************************************************/
void ArithEncDec::start_decode (FILE *s)
{
    in_D = 0;		/// Initial offset in range is 0
	in_R = Half;	/// Range = Half

	for (int i = 0; i<B_bits; i++)  ADD_NEXT_INPUT_BIT(in_D, 0, s);   /// Fill D

	if (in_D >= Half) {cerr<<"Corrupt input file (start_decode())\n"; exit(1);}
}


/*******************************************************************************
    Throw away B_bits in buffer by doing nothing (encoder wrote these for us to
    consume.) (They were mangled anyway as we only kept V-L, and cannot get back
    to V)
*******************************************************************************/
void ArithEncDec::finish_decode ()
{
	/// No action
}


/*******************************************************************************
    Initialize the bit output function
*******************************************************************************/
void ArithEncDec::startoutputtingbits ()
{
	_out_buffer = 0;
	_out_bits_to_go = BYTE_SIZE;
}


/*******************************************************************************
    Start the bit input function
*******************************************************************************/
void ArithEncDec::startinputtingbits ()
{
	_in_garbage = 0;	/// Number of bytes read past end of file
	_in_bit_ptr = 0;	/// No valid bits yet in input buffer
}


/*******************************************************************************
    Complete outputting bits
*******************************************************************************/
void ArithEncDec::doneoutputtingbits (FILE *s)
{
	if (_out_bits_to_go != BYTE_SIZE)
        OUTPUT_BYTE(_out_buffer << _out_bits_to_go, s);
	_out_bits_to_go = BYTE_SIZE;
}


/*******************************************************************************
    Complete inputting bits
*******************************************************************************/
void ArithEncDec::doneinputtingbits ()
{
	_in_bit_ptr = 0;	/// "Wipe" buffer (in case more input follows)
}


/*******************************************************************************
    Outputs bit 'b' to stdout. Builds up a buffer, writing a byte at a time.
*******************************************************************************/
inline void ArithEncDec::OUTPUT_BIT (int b, FILE *s)
{
    do
    {
        _out_buffer <<= 1;
        if (b)  _out_buffer |= 1;
        
        _out_bits_to_go--;
        if (_out_bits_to_go == 0)
        {
            OUTPUT_BYTE(_out_buffer, s);
            _out_bits_to_go = BYTE_SIZE;
            _out_buffer = 0;
        }
    } while (0);
}


/*******************************************************************************
    Returns a bit from stdin, by shifting 'v' left one bit, and adding next bit
    as lsb (possibly reading upto garbage_bits extra bits beyond valid input)

    garbage_bits: Number of bits (to nearest byte) past end of file to be
    allowed to 'read' before printing an error message and halting. This is
    needed by our arithmetic coding module when the FRUGAL_BITS option is
    defined, as upto B_bits extra bits may be needed to keep the code buffer
    full (although the actual bitvalue is not important) at the end of decoding

    The buffer is not shifted, instead a bit flag (_in_bit_ptr) is moved to
    point to the next bit that is to be read. When it is zero, the next byte
    is read, and it is reset to point to the msb.
*******************************************************************************/
inline void ArithEncDec::ADD_NEXT_INPUT_BIT (code_value &v, int garbage_bits,
                                             FILE *s)
{
    do
    {
        if (_in_bit_ptr == 0)
        {
            _in_buffer = getc(s);
            if (_in_buffer == EOF)
            {
                _in_garbage++;
                if ((_in_garbage - 1) * 8 >= garbage_bits)
                {
                    cerr << "Bad input file"
                            " - attempted read past end of file.\n";
                    exit(1);
                }
            }
            else    _bytes_input++;
            
            _in_bit_ptr = ( 1<<(BYTE_SIZE - 1) );
        }
        v = (v<<1);
        if (_in_buffer & _in_bit_ptr)   v++;
        _in_bit_ptr >>= 1;
    } while (0);
}


/*******************************************************************************
    Responsible for outputting the bit passed to it and an opposite number of
    bit equal to the value stored in bits_outstanding
*******************************************************************************/
inline void ArithEncDec::ORIG_BIT_PLUS_FOLLOW (int b, FILE *s)
{
    do
    {
        OUTPUT_BIT((b), s);
        while (out_bits_outstanding > 0)
        {
            OUTPUT_BIT(!(b), s);
            out_bits_outstanding--;
        }
    } while (0);
}
//============================================================
inline void ArithEncDec::BIT_PLUS_FOLLOW (int b, FILE *s)
{
    ORIG_BIT_PLUS_FOLLOW(b, s);
}


/*******************************************************************************
    Output code bits until the range has been expanded to above QUARTER With
    FRUGAL_BITS option, ignore first zero bit output (a redundant zero will
    otherwise be emitted every time the encoder is started)
*******************************************************************************/
inline void ArithEncDec::ENCODE_RENORMALISE (FILE *s)
{
    do
    {
        while (out_R <= Quarter)
        {
            if (out_L >= Half)
            {
                BIT_PLUS_FOLLOW(1, s);
                out_L -= Half;
            }
            else if (out_L + out_R <= Half)
            {
                BIT_PLUS_FOLLOW(0, s);
            }
            else
            {
                out_bits_outstanding++;
                out_L -= Quarter;
            }
            out_L <<= 1;
            out_R <<= 1;
        }
    } while (0);
}


/*******************************************************************************
    Input code bits until range has been expanded to more than QUARTER. Mimics
    encoder. FRUGAL_BITS option also keeps track of bitstream input so it can
    work out exactly how many disambiguating bits the encoder put out (1,2,3).
*******************************************************************************/
inline void ArithEncDec::DECODE_RENORMALISE (FILE *s)
{
    do
    {
        while (in_R <= Quarter)
        {
            in_R <<= 1;
            ADD_NEXT_INPUT_BIT(in_D, 0, s);
        }
    } while (0);
}

#endif //PHOENIX_ARITHENCDEC_H