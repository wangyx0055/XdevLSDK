/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#include <XdevLPlatform.h>
#include <XdevLTypes.h>
#include <XdevLUtils.h>
#include <XdevLXstring.h>
#include <XdevLXfstring.h>
#include <XdevLError.h>

#if defined (__GNUC__) && !defined (WIN32)
#  include <netinet/in.h>
#  include <sys/time.h>
#elif defined (_MSC_VER)
#  include <time.h>
#endif

#if !defined (WIN32)
#  include <sys/time.h>
#  include <unistd.h>
#  include <pthread.h>
#else
#  include <direct.h>
#endif

namespace xdl {

	xdl_int getPluginInformationFromFilename(
	  std::string& path,
	  std::string& pluginName,
	  XdevLVersion& version,
	  const std::string& filename) {

		//
		// Skip if it is the local or parrent path.
		//
		if(filename == "." || filename == "..") {
			return ERR_ERROR;
		}

		path 						= xstd::get_path(filename);
		std::string tmp = xstd::get_pathless<std::string>(filename);
		pluginName			= tmp.substr(0, tmp.find_last_of("-"));

		//
		// Check if the file has a version number.
		//
		if(pluginName.length() == tmp.length()) {
			return ERR_ERROR;
		}

		std::vector<std::string> values;
		std::string versionstr(tmp.substr(pluginName.length() + 1, tmp.length()));
		xstd::tokenize(versionstr, values, ".");

		//
		// Check if the number of version number is correct.
		//
		if(values.size() < 3) {
			return ERR_ERROR;
		}

		version.setVersion(xstd::from_string<xdl_int>(values[0]),
		                   xstd::from_string<xdl_int>(values[1]),
		                   xstd::from_string<xdl_int>(values[2]));


		return ERR_OK;
	}


	void sleep(xdl_double pSec) {
#if defined (__GNUC__) && !defined (WIN32)
		//usleep ( static_cast<xdl_int>(pSec * 1000000.0) ) ; // no way!!
		// since the unistd functions sleep/usleep suspend
		// the entire process, not a single thread, chrono::sleep uses
		// the posix thread interface to put only the current thread asleep.

		struct timeval  currenttime;

		struct timespec wait;
		pthread_mutex_t mutex;
		pthread_cond_t  cond;
		xdl_int32 dt_sec, dt_usec;

		// set timeout time, relative to current time:
		gettimeofday( &currenttime, NULL );
		dt_sec  = (xdl_int32) pSec;
		dt_usec = (xdl_int32) ((pSec - (xdl_double)dt_sec) * 1000000.0);
		wait.tv_nsec = (currenttime.tv_usec + dt_usec) * 1000L;

		if ( wait.tv_nsec > 1000000000L ) {
			wait.tv_nsec -= 1000000000L;
			dt_sec ++;
		}

		wait.tv_sec  = currenttime.tv_sec + dt_sec;

		// initialize condition and mutex objects:
		pthread_mutex_init( &mutex, NULL );
		pthread_cond_init( &cond, NULL );
		// do a timed wait:
		pthread_mutex_lock( &mutex );
		pthread_cond_timedwait( &cond, &mutex, &wait );
		pthread_mutex_unlock( &mutex );
		// destroy condition and mutex objects:
		pthread_mutex_destroy( &mutex );
		pthread_cond_destroy( &cond );
#else
		::Sleep ( static_cast<unsigned long>(pSec*1000.0) ) ;
#endif
	}

	xdl_uint32 adler32(const xdl_char* buf, size_t buflength) {
		const xdl_uint8 *buffer = (const xdl_uint8*)buf;
		xdl_uint32 s1 = 1;
		xdl_uint32 s2 = 0;

		for (size_t n = 0; n < buflength; n++) {
			s1 = (s1 + buffer[n]) % 65521;
			s2 = (s2 + s1) % 65521;
		}

		return (s2 << 16) | s1;
	}


	/*
	 *  sha1.c
	 *
	 *  Description:
	 *      This file implements the Secure Hashing Algorithm 1 as
	 *      defined in FIPS PUB 180-1 published April 17, 1995.
	 *
	 *      The SHA-1, produces a 160-bit message digest for a given
	 *      data stream.  It should take about 2**n steps to find a
	 *      message with the same digest as a given message and
	 *      2**(n/2) to find any two messages with the same digest,
	 *      when n is the digest size in bits.  Therefore, this
	 *      algorithm can serve as a means of providing a
	 *      "fingerprint" for a message.
	 *
	 *  Portability Issues:
	 *      SHA-1 is defined in terms of 32-bit "words".  This code
	 *      uses <stdint.h> (included via "sha1.h" to define 32 and 8
	 *      bit unsigned integer types.  If your C compiler does not
	 *      support 32 bit unsigned integers, this code is not
	 *      appropriate.
	 *
	 *  Caveats:
	 *      SHA-1 is designed to work with messages less than 2^64 bits
	 *      long.  Although SHA-1 allows a message digest to be generated
	 *      for messages of any number of bits less than 2^64, this
	 *      implementation only works with messages with a length that is
	 *      a multiple of the size of an 8-bit character.
	 *
	 */

	/*
	 *  Define the SHA1 circular left shift macro
	 */
#define SHA1CircularShift(bits,word) \
	(((word) << (bits)) | ((word) >> (32-(bits))))

	/*
	 *  SHA1Reset
	 *
	 *  Description:
	 *      This function will initialize the SHA1Context in preparation
	 *      for computing a new SHA1 message digest.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The context to reset.
	 *
	 *  Returns:
	 *      sha Error Code.
	 *
	 */
	 
	int SHA::reset() {
		
		Length_Low             = 0;
		Length_High            = 0;
		Message_Block_Index    = 0;
		Intermediate_Hash[0]   = 0x67452301;
		Intermediate_Hash[1]   = 0xEFCDAB89;
		Intermediate_Hash[2]   = 0x98BADCFE;
		Intermediate_Hash[3]   = 0x10325476;
		Intermediate_Hash[4]   = 0xC3D2E1F0;

		Computed   = 0;
		Corrupted  = 0;

		return shaSuccess;
	}

	/*
	 *  SHA1Result
	 *
	 *  Description:
	 *      This function will return the 160-bit message digest into the
	 *      Message_Digest array  provided by the caller.
	 *      NOTE: The first octet of hash is stored in the 0th element,
	 *            the last octet of hash in the 19th element.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The context to use to calculate the SHA-1 hash.
	 *      Message_Digest: [out]
	 *          Where the digest is returned.
	 *
	 *  Returns:
	 *      sha Error Code.
	 *
	 */
	int SHA::getResult(const uint8_t* data, xdl_uint length, uint8_t* Message_Digest) {
		
		reset();
		
		setInput( data, length);
		
		int i;

		if (Corrupted) {
			return Corrupted;
		}

		if (!Computed) {
			padMessage();

			for (i=0; i<64; ++i) {
				/* message may be sensitive, clear it out */
				Message_Block[i] = 0;
			}

			Length_Low = 0;    /* and clear length */

			Length_High = 0;
			Computed = 1;
		}

		for (i = 0; i < SHA1HashSize; ++i) {
			Message_Digest[i] = Intermediate_Hash[i>>2] >> 8 * ( 3 - ( i & 0x03 ) );
		}

		return shaSuccess;
	}

	/*
	 *  SHA1Input
	 *
	 *  Description:
	 *      This function accepts an array of octets as the next portion
	 *      of the message.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The SHA context to update
	 *      message_array: [in]
	 *          An array of characters representing the next portion of
	 *          the message.
	 *      length: [in]
	 *          The length of the message in message_array
	 *
	 *  Returns:
	 *      sha Error Code.
	 *
	 */
	int SHA::setInput(const uint8_t* message_array, xdl_uint length) {
		if (!length) {
			return shaSuccess;
		}

		if (!message_array) {
			return shaNull;
		}

		if (Computed) {
			Corrupted = shaStateError;
			return shaStateError;
		}

		if (Corrupted) {
			return Corrupted;
		}

		while (length-- && !Corrupted) {
			Message_Block[Message_Block_Index++] =
			  (*message_array & 0xFF);

			Length_Low += 8;

			if (Length_Low == 0) {
				Length_High++;

				if (Length_High == 0) {
					/* Message is too long */
					Corrupted = 1;
				}
			}

			if (Message_Block_Index == 64) {
				processMessageBlock();
			}

			message_array++;
		}

		return shaSuccess;
	}

	/*
	 *  SHA1ProcessMessageBlock
	 *
	 *  Description:
	 *      This function will process the next 512 bits of the message
	 *      stored in the Message_Block array.
	 *
	 *  Parameters:
	 *      None.
	 *
	 *  Returns:
	 *      Nothing.
	 *
	 *  Comments:

	 *      Many of the variable names in this code, especially the
	 *      single character names, were used because those were the
	 *      names used in the publication.
	 *
	 *
	 */
	void SHA::processMessageBlock() {
		const uint32_t K[] =    {       /* Constants defined in SHA-1   */
			0x5A827999,
			0x6ED9EBA1,
			0x8F1BBCDC,
			0xCA62C1D6
		};
		int           t;                 /* Loop counter                */
		uint32_t      temp;              /* Temporary word value        */
		uint32_t      W[80];             /* Word sequence               */
		uint32_t      A, B, C, D, E;     /* Word buffers                */

		/*
		 *  Initialize the first 16 words in the array W
		 */

		for (t = 0; t < 16; t++) {
			W[t] = Message_Block[t * 4] << 24;
			W[t] |= Message_Block[t * 4 + 1] << 16;
			W[t] |= Message_Block[t * 4 + 2] << 8;
			W[t] |= Message_Block[t * 4 + 3];
		}

		for (t = 16; t < 80; t++) {
			W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
		}

		A = Intermediate_Hash[0];

		B = Intermediate_Hash[1];
		C = Intermediate_Hash[2];
		D = Intermediate_Hash[3];
		E = Intermediate_Hash[4];

		for (t = 0; t < 20; t++) {
			temp =  SHA1CircularShift(5,A) +
			        ((B & C) | ((~B) & D)) + E + W[t] + K[0];
			E = D;
			D = C;
			C = SHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		for (t = 20; t < 40; t++) {
			temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
			E = D;
			D = C;
			C = SHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		for (t = 40; t < 60; t++) {
			temp = SHA1CircularShift(5,A) +
			       ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
			E = D;
			D = C;
			C = SHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		for (t = 60; t < 80; t++) {
			temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
			E = D;
			D = C;
			C = SHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		Intermediate_Hash[0] += A;
		Intermediate_Hash[1] += B;
		Intermediate_Hash[2] += C;
		Intermediate_Hash[3] += D;
		Intermediate_Hash[4] += E;
		Message_Block_Index = 0;
	}


	/*
	 *  SHA1PadMessage
	 *

	*  Description:
	 *      According to the standard, the message must be padded to an even
	 *      512 bits.  The first padding bit must be a '1'.  The last 64
	 *      bits represent the length of the original message.  All bits in
	 *      between should be 0.  This function will pad the message
	 *      according to those rules by filling the Message_Block array
	 *      accordingly.  It will also call the ProcessMessageBlock function
	 *      provided appropriately.  When it returns, it can be assumed that
	 *      the message digest has been computed.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The context to pad
	 *      ProcessMessageBlock: [in]
	 *          The appropriate SHA*ProcessMessageBlock function
	 *  Returns:
	 *      Nothing.
	 *
	 */

	void SHA::padMessage() {
		/*
		 *  Check to see if the current message block is too small to hold
		 *  the initial padding bits and length.  If so, we will pad the
		 *  block, process it, and then continue padding into a second
		 *  block.
		 */
		if (Message_Block_Index > 55) {
			Message_Block[Message_Block_Index++] = 0x80;

			while (Message_Block_Index < 64) {
				Message_Block[Message_Block_Index++] = 0;
			}

			processMessageBlock();

			while (Message_Block_Index < 56) {
				Message_Block[Message_Block_Index++] = 0;
			}

		} else {
			Message_Block[Message_Block_Index++] = 0x80;

			while (Message_Block_Index < 56) {

				Message_Block[Message_Block_Index++] = 0;
			}
		}

		/*
		 *  Store the message length as the last 8 octets
		 */
		Message_Block[56] = (xdl_uint8)(Length_High >> 24);
		Message_Block[57] = (xdl_uint8)(Length_High >> 16);
		Message_Block[58] = (xdl_uint8)(Length_High >> 8);
		Message_Block[59] = (xdl_uint8)(Length_High);
		Message_Block[60] = (xdl_uint8)(Length_Low >> 24);
		Message_Block[61] = (xdl_uint8)(Length_Low >> 16);
		Message_Block[62] = (xdl_uint8)(Length_Low >> 8);
		Message_Block[63] = Length_Low;

		processMessageBlock();
	}

	TiXmlElement* TiXmlFindElement(TiXmlDocument* doc, const xdl_char* elements) {
		std::vector<std::string> tokens;
		size_t number = xstd::tokenize(elements, tokens, "/");
		TiXmlNode* child = doc;
		size_t count = 0;
		while(count < number) {
			child = child->FirstChild(tokens[count].c_str());
			count++;;
		}
		if(child != NULL)
			return child->ToElement();

		return NULL;
	}

	std::ifstream::pos_type getSizeOfFile(const xdl::XdevLFileName& file) {
		std::ifstream in(file, std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
	}

}
