#include "fpga_vdf_discriminant.h"

#include <regex>
using namespace FpgaVdf;

int main() {
	int status = 0;

    const char* hash = "-0xdc2a335cd2b355c99d3d8d92850122b3d8fe20d0f5360e7aaaecb448960d57bcddfee12a229bbd8d370feda5a17466fc725158ebb78a2a7d37d0a226d89b54434db9c3be9a9bb6ba2c2cd079221d873a17933ceb81a37b0665b9b7e247e8df66bdd45eb15ada12326db01e26c861adf0233666c01dec92bbb547df7369aed3b1fbdff867cfc670511cc270964fbd98e5c55fbe0947ac2b9803acbfd935f3abb8d9be6f938aa4b4cc6203f53c928a979a2f18a1ff501b2587a93e95a428a107545e451f0ac6c7f520a7e99bf77336b1659a2cb3dd1b60e0c6fcfffc05f74cfa763a1d0af7de9994b6e35a9682c4543ae991b3a39839230ef84dae63e88d90f457";
    std::cout << "base Hash length: " << strlen(hash) << std::endl;
    DATA_TYPE inData[DATA_ARRAY_LEN] = {0,0,0,0};

    mpz_t hashMz, hashMz2;
    mpz_init_set_str(hashMz, hash, 0);
    mpz_init_set_str(hashMz2, hash, 0);

    char outMZStr[8192];
    for(int i = 0; i < 90000; i++) {
        gmp_sprintf(outMZStr, "%#Zx", hashMz);

        std::cout << outMZStr << std::endl;
        //std::cout << "Extracted Data Array " << std::endl;

        FpgaVdf::Utils::parse(outMZStr, inData);

//        for(int i=0; i < DATA_ARRAY_LEN; i++) {
//            std::string item = inData[i].to_string(AP_HEX);
//            std::cout << ":[" << i << "-" << item.length() << "]: " << item << std::endl;
//        }
//        std::cout << std::endl;

        DATA_TYPE outData[DATA_ARRAY_LEN] = { 0 , 0 , 0 ,0};
        fpga_vdf_discriminant(inData, outData);

        mpz_add(hashMz, hashMz, hashMz2);// -0xFFFFFFFFFFFFFFFF );
    }



//    std::cout << "[TB] outData Address" << outData << std::endl;
//
//    std::cout << std::endl;
//    for(int i=0; i < 2; i++) {
//        //ap_int<2048> a = outData[i];
//        //std::cout << a.to_string(AP_HEX) << std::endl;
//        std::cout << "outData[" << i << "-" << outData[i].to_string(AP_HEX).length() - 3 <<"]: " << outData[i].to_string(AP_HEX) << std::endl;
//        //std::cout << "outData[" << i << "-" << outData[i].to_string(AP_BIN).length() - 2 << "]: " << outData[i].to_string(AP_BIN) << std::endl;
//
//    }
//    std::cout << std::endl;

	return status;
}
