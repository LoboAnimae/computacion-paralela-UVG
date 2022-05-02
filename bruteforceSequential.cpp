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

// Taken from GDawg's implementation
string decrypt(CBC_Mode< DES >::Decryption decryptor, string cipher, CryptoPP::byte key[DES::KEYLENGTH],
               CryptoPP::byte iv[DES::BLOCKSIZE]) {
    string recovered;
    decryptor.SetKeyWithIV(key, 8, iv);
    StringSource s(cipher, true,
                   new StreamTransformationFilter(decryptor,
                                                  new StringSink(recovered), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING
                   ) // StreamTransformationFilter
    ); // StringSource
    return recovered;
}

// Checks if the decoded string contains the keyword, from GDawg's implementation
bool check_key(CBC_Mode< DES >::Decryption decryptor, string cipher, CryptoPP::byte key[DES::KEYLENGTH], CryptoPP::byte iv[DES::BLOCKSIZE]){
    return decrypt(decryptor, cipher, key, iv).find("des_crypto") != std::string::npos;
}

int main(int argc, char* argv[])
{
    AutoSeededRandomPool prng;

    SecByteBlock key(8);
    prng.GenerateBlock(key, 8);

    CryptoPP::byte iv[DES::BLOCKSIZE] = {0};
    CryptoPP::byte key2[DES::KEYLENGTH] = {0, 0, 0, 0, 255, 255, 255, 255};

    string plain = "Tried: des_crypto, crypto++ apt install, crypto++ local install, libtirpc, VMs... maybe its my PC lol";
    string cipher, encoded, recovered;

    // Pretty print key
    encoded.clear();
    StringSource(key2, 8, true,
                 new HexEncoder(
            new StringSink(encoded)
    ) // HexEncoder
    ); // StringSource
    cout << "key: " << encoded << endl;

    /*********************************\
    \*********************************/

    try
    {
        cout << "plain text: " << plain << endl;

        CBC_Mode< DES >::Encryption e;
        e.SetKeyWithIV(key2, 8, iv);

        // The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource(plain, true,
                     new StreamTransformationFilter(e,
                                                    new StringSink(cipher)
        ) // StreamTransformationFilter
        ); // StringSource
    }
    catch(const CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

    // Pretty print
    encoded.clear();
    StringSource(cipher, true,
                 new HexEncoder(
            new StringSink(encoded)
    ) // HexEncoder
    ); // StringSource
    cout << "cipher text: " << encoded << endl;

    /*********************************\
    \*********************************/

    // 8 Cycles due to the key size (8) GDawg implementation & Gus guidance
    try
    {
        key2[0] = (CryptoPP::byte)0;
        key2[1] = (CryptoPP::byte)0;
        key2[2] = (CryptoPP::byte)0;
        key2[3] = (CryptoPP::byte)0;
        key2[4] = (CryptoPP::byte)0;
        key2[5] = (CryptoPP::byte)0;
        key2[6] = (CryptoPP::byte)0;
        key2[7] = (CryptoPP::byte)0;
        CBC_Mode< DES >::Decryption d;
        for(int i0 = 0; i0 < 255; i0++){
            key2[0] = (CryptoPP::byte)i0;
            for(int i1 = 0; i1 < 255; i1++){
                key2[1] = (CryptoPP::byte)i1;
                for(int i2 = 0; i2 < 255; i2++){
                    key2[2] = (CryptoPP::byte)i2;
                    for(int i3 = 0; i3 < 255; i3++){
                        key2[3] = (CryptoPP::byte)i3;
                        for(int i4 = 0; i4 < 255; i4++){
                            key2[4] = (CryptoPP::byte)i4;
                            for(int i5 = 0; i5 < 255; i5++){
                                key2[5] = (CryptoPP::byte)i5;
                                for(int i6 = 0; i6 < 255; i6++){
                                    key2[6] = (CryptoPP::byte)i6;
                                    for(int i7 = 0; i7 < 255; i7++){
                                        key2[7] = (CryptoPP::byte)i7;
                                        // Checks if the decrypted text contains the keyword
                                        if (check_key(d, cipher, key2, iv)){
                                            cout << "Keyword has been found! \n";
                                            return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch(const CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

    return 0;
}