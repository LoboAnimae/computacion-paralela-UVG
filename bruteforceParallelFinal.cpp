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

#include <stack>

#include <vector>

#include <exception>
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

bool recursive_find_word(
	int from,
	int to,
	int level,
	CBC_Mode<DES>::Decryption d,
	string cipher,
	CryptoPP::byte* key2,
	CryptoPP::byte* iv,
	std::stack<int>* result_stack,
	int id)

{
	
	if (level == 8)
	{
		for (int i = from; i <= to; ++i)
		{
			key2[level - 1] = (CryptoPP::byte)
				i;
			if (check_key(d, cipher, key2, iv))
			{
				cout << "ID recursive: " << id << endl;
				result_stack->push(i);
				return true;
			}
		}
		return false;
		// Do something to stop the recursion
	}
	else
	{ // From 1 to 7
		std::vector<int> intervals;
		fill_intervals(&intervals);
		for (int i = 0; i <= 255; ++i)
		{
			key2[level] = (CryptoPP::byte)
				i;
			if (recursive_find_word(0, 255, level + 1, d, cipher, key2, iv, result_stack, id))
			{
				result_stack->push(i);
				return true;
			}
		}
	}
	return false;
}

void find_word(
	int from,
	int to,
	int level,
	CBC_Mode<DES>::Decryption d,
	string cipher,
	CryptoPP::byte* key2,
	CryptoPP::byte* iv,
	std::stack<int>* result_stack,
	int id,
	int size)
{

	MPI_Status status;

	// If the id is 0, then assign the work
	if (id == 0)
	{
		// Root of the tree
		// We are on the first level of the recursion
		int expected_from = 1;

		// Wait for all others to awake
		while (true)
		{
			int buff;
			MPI_Recv(&buff, 1, MPI_INT, expected_from, expected_from, MPI_COMM_WORLD, &status);
			expected_from++;
			if (expected_from >= size) break;
		}

		std::vector<int> intervals;
		fill_intervals(&intervals);
		intervals.push_back(-1);
		for (int i = 1; i < size; ++i)
		{
			int j;
			for (j = 0; j < (int)intervals.size(); ++j)
			{
				MPI_Send(&intervals[j], 1, MPI_INT, i, j, MPI_COMM_WORLD);
			}
		}
		// Sequential
		for (int i = 0; i <= 255; ++i)
		{
			key2[level] = (CryptoPP::byte)i;
			int local_from = intervals[0];

			int local_to = intervals[1];
			std::stack<int> result_stack;

			if (recursive_find_word(local_from, local_to, 1, d, cipher, key2, iv, &result_stack, id))
			{
				cout << "Found in " << id << " the key ";

				while (!result_stack.empty())
				{
					cout << result_stack.top() << " ";
					result_stack.pop();
				}
				throw;
			}
		}
	}
	else
	{
		int acknowledge = 1;
		MPI_Send(&acknowledge, 1, MPI_INT, 0, id, MPI_COMM_WORLD);

		// Some node of the tree
		std::vector<int> intervals;

		int j = 0;
		int buff = 0;
		while (buff != -1)
		{
			MPI_Recv(&buff, 1, MPI_INT, 0, j++, MPI_COMM_WORLD, &status);
			intervals.push_back(buff);
			// cout << "Buff: " << buff << " ID: " << id << endl;

		}
		// cout << "ID: " << id << " broken" << endl;

		int from_interval = intervals[id];
		int to_interval = id + 1 > (int)intervals.size()
						  ? END_INTERVAL
						  : intervals[id + 1];

		// cout << "ID: " << id << " from: " << from_interval << " to: " << to_interval << endl;


		for (int i = from_interval; i < to_interval; ++i)
		{
			cout << "ID: " << id << " i: " << i << endl;
			std::stack<int> result_stack;

			if (recursive_find_word(from_interval, to_interval, 1, d, cipher, key2, iv, &result_stack, id))
			{
				cout << "Found in " << id << " the key ";

				while (!result_stack.empty())
				{
					cout << result_stack.top() << " ";
					result_stack.pop();
				}
				throw;
			}
		}
	}
}

int main(int argc, char* argv[])
{
//	MPI_Status st;
	//MPI_Request req;
	int
	//flag,
	N,
		id;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(comm, &N);
	MPI_Comm_rank(comm, &id);

	AutoSeededRandomPool prng;

	SecByteBlock
		key(8);
	prng.GenerateBlock(key, 8);

	CryptoPP::byte iv[DES::BLOCKSIZE] = { 0 },
		key2[DES::KEYLENGTH] = { 0, 0, 0, 0, 0, 0, 0, 10};

	// string plain = "a";
	// string plain = "des_crypto: Hello World!";
	string plain = "Este es la cadena de prueba, esperemos encontrar un resultado apropiado";
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

		std::stack<int> result_stack;
		find_word(0, 0, 0, d, cipher, key2, iv, &result_stack, id, N);
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

	MPI_Finalize();

	return 0;
}


