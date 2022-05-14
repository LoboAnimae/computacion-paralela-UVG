/**
 * bruteforceSequential.cpp
 * References: GDawg4 (Rodrigo Garoz) & https://cryptopp.com/wiki/TripleDES, https://github.com/tebinraouf/CryptoPP/blob/master/tdes-ecb.cpp
 * Run with g++
 * **/

#include "cryptopp860/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp860/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp860/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp860/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp860/des.h"
using CryptoPP::DES;

#include "cryptopp860/modes.h"
using CryptoPP::CBC_Mode;

#include "cryptopp860/secblock.h"
using CryptoPP::SecByteBlock;

#include <vector>

#include <exception>

#include "cryptopp860/integer.h"

#include <mpi.h>

// Taken from GDawg's implementation
string decrypt(CBC_Mode<DES>::Decryption decryptor, string cipher, CryptoPP::byte key[DES::KEYLENGTH],
	CryptoPP::byte iv[DES::BLOCKSIZE])
{
	string recovered;
	decryptor.SetKeyWithIV(key, 8, iv);
	StringSource
		s(cipher, true,
		new
			StreamTransformationFilter(decryptor,
			new
				StringSink(recovered), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING
		) // StreamTransformationFilter
	); // StringSource
	return recovered;
}

// Checks if the decoded string contains the keyword, from GDawg's implementation
bool
check_key(CBC_Mode<DES>::Decryption decryptor,
	string cipher,
	CryptoPP::byte key[DES::KEYLENGTH],
	CryptoPP::byte iv[DES::BLOCKSIZE])
{
	return decrypt(decryptor, cipher, key, iv).find("des_crypto") != std::string::npos;
}
#define START_INTERVAL 0
#define END_INTERVAL 255
#define INTERVAL_NUMBER 15
void fill_intervals(std::vector<int>* interval)
{
	if (END_INTERVAL % INTERVAL_NUMBER != 0)
	{
		throw;
	}
	int current_start = START_INTERVAL;
	for (int i = 0; i < INTERVAL_NUMBER; ++i)
	{
		interval->push_back(current_start);
		current_start += INTERVAL_NUMBER;
	}
}

bool find_word(int from,
	int to,
	int level,
	CBC_Mode<DES>::Decryption d,
	string cipher,
	CryptoPP::byte* key2,
	CryptoPP::byte* iv)
{
	if (level > 8)
	{
		throw;
	}
	if (level == 8)
	{
		for (int i = from; i <= to; ++i)
		{
			key2[level - 1] = (CryptoPP::byte)
				i;
			if (check_key(d, cipher, key2, iv))
			{
				return true;
			}
		}
		return false;
		// Do something to stop the recursion
	}
	else if (level == 0)
	{ // Tree root
		// We are on the first level of the recursion
		std::vector<int> intervals;
		fill_intervals(&intervals);
		// Sequential
		for (int i = 0; i <= 255; ++i)
		{
			key2[0] = (CryptoPP::byte)i;
//			int local_from = intervals[i];
//
//
//			int local_to =  i == intervals.size()
//				? intervals[i+1] - 1
//				: END_INTERVAL;

			

			bool result = find_word(0, 0, level + 1, d, cipher, key2, iv);
			if (result)
			{
				return true;
			}
		}
	}
	else
	{ // From 1 to 7
		std::vector<int> intervals;
		fill_intervals(&intervals);
		for (int i = 0; i <= 255; ++i)
		{
			key2[level] = (CryptoPP::byte)
				i;
//			int local_from = intervals[i];
//			int local_to = i + 1 > INTERVAL_NUMBER
//						   ? intervals[i + 1] - 1
//						   : END_INTERVAL;
			if (find_word(0, 255, level + 1, d, cipher, key2, iv))
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

int main(int argc, char* argv[])
{

	MPI_Init (&argc, &argv);
	int rank, num;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  	MPI_Comm_size (MPI_COMM_WORLD, &num);

	AutoSeededRandomPool prng;

	SecByteBlock
		key(8);
	prng.GenerateBlock(key, 8);

	CryptoPP::byte iv[DES::BLOCKSIZE] = { 0 },
		key2[DES::KEYLENGTH] = { 0, 0, 0, 0, 0, 0, 0, 10 };

	// string plain = "a";
	string plain = "Este es la cadena de prueba, esperemos encontrar un resultado apropiado!";
	// string plain = "Tried: des_crypt";
	if (plain.find("des_crypto") == std::string::npos)
	{
		throw;
	}
	string cipher, encoded, recovered;

	// Pretty print key
	encoded.clear();
	StringSource(key2, 8, true,
		new
			HexEncoder(
			new
				StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "key: " << encoded << endl;

	/*********************************\
	\*********************************/

	try
	{
		cout << "plain text: " << plain << endl;

		CBC_Mode<DES>::Encryption e;
		e.SetKeyWithIV(key2, 8, iv);

		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plain, true,
			new
				StreamTransformationFilter(e,
				new
					StringSink(cipher)
			) // StreamTransformationFilter
		); // StringSource
	}
	catch (
		const CryptoPP::Exception
		& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/

	// Pretty print
	encoded.clear();
	StringSource(cipher, true,
		new
			HexEncoder(
			new
				StringSink(encoded)
		) // HexEncoder
	); // StringSource
	// printf(encoded);
	cout << "cipher text: " << encoded << endl;

	/*********************************\
	\*********************************/

	// 8 Cycles due to the key size (8) GDawg implementation & Gus guidance
	try
	{
		key2[0] = (CryptoPP::byte)
			0;
		key2[1] = (CryptoPP::byte)
			0;
		key2[2] = (CryptoPP::byte)
			0;
		key2[3] = (CryptoPP::byte)
			0;
		key2[4] = (CryptoPP::byte)
			0;
		key2[5] = (CryptoPP::byte)
			0;
		key2[6] = (CryptoPP::byte)
			0;
		key2[7] = (CryptoPP::byte)
			0;
		CBC_Mode<DES>::Decryption d;


		bool result = find_word(0, 0, 0, d, cipher, key2, iv);

		if (result) {
			for (const auto &ans : key2) {
				cout << (int)ans << " ";
			}
			cout << endl;
		} else {
			cout << "Not found!" << endl;
		}
	}
	catch (
		const CryptoPP::Exception
		& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/

	return 0;
}


