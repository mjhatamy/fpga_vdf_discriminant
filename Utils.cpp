//
// Created by mjhatamy on 10/10/21.
//

#include "Utils.h"

#include <regex>

#ifndef __SYNTHESIS__
#include <gmp.h>
#endif

namespace FpgaVdf {
    void Utils::assign(DATA_TYPE src[DATA_ARRAY_LEN], DATA_TYPE dst[DATA_ARRAY_LEN]) {
        for(int i=0; i < DATA_ARRAY_LEN; i++) {
            dst[i] = src[i];
        }
    }

#ifndef __SYNTHESIS__
    void Utils::parse(const char *hashCharArray, DATA_TYPE *outData) {
        std::string hashString(hashCharArray);
        std::regex isMinusRegex("^(-0x).*$");
        std::regex hexPrefixRegex("^(0x|-0x)");

        bool isMinus = regex_match(hashString, isMinusRegex);
        std::string hexPrefix = isMinus ? "-0x" : "0x";

        hashString = std::regex_replace(hashString, hexPrefixRegex, "");
        //std::cout << "Processed Hash String: " << isMinus << " - " << hashString << std::endl;

        for(int i= (int)hashString.length() -1, offset=0; i > 0; ) {
            if(i-255 >= 0) {
                outData[offset] = DATA_TYPE(hashString.substr(i-255, 256).insert(0, hexPrefix).c_str(), 0);
            } else {
                outData[offset] = DATA_TYPE(hashString.substr(0, i ).insert(0, hexPrefix).c_str(), 0);
            }
            i-=256;
            offset++;
        }
    }

    void sumValidateByGMP(DATA_TYPE *sumResult, DATA_TYPE *lVal, DATA_TYPE *rVal) {
        mpz_t glVal, grVal, gSumVal, gSumFromBinStr;
        mpz_inits(glVal, grVal, gSumVal, gSumFromBinStr, NULL);

        for(int offset = 0; offset < DATA_ARRAY_LEN; offset++) {
            mpz_set_str(glVal, lVal[offset].to_string(AP_HEX, true).c_str(), 0);
            mpz_set_str(grVal, rVal[offset].to_string(AP_HEX, true).c_str(), 0);

            std::string binVal = sumResult[offset].to_string(AP_HEX);
            //binVal.replace(2,5, "");
            //std::cout << binVal.length() << " - " << binVal << std::endl << sumResult[offset].to_string(AP_HEX) << std::endl;
            mpz_set_str(gSumFromBinStr, binVal.c_str(), 0);
            //mpz_mul_ui(gSumFromBinStr, gSumFromBinStr, -0x1);
            mpz_add(gSumVal, glVal, grVal);

            if(mpz_cmp(gSumVal, gSumFromBinStr) != 0) {
                std::cout << "FPGA_VDF SUM function ERROR detected by GMP for index '" << offset << "'" << std::endl;
                gmp_printf("GMP:> lVal: %#Zx\n", glVal);
                gmp_printf("GMP:> rVal: %#Zx\n", grVal);
                gmp_printf("GMP:> gSumVal: %#Zx\n", gSumVal);
                gmp_printf("GMP:> gSumFromBinStr: %#Zx\n", gSumFromBinStr);

                throw std::invalid_argument("Sum calculation is incorrect");
            }
        }
        std::cout << "FPGA_VDF SUM function validated by GMP "<< std::endl;
        mpz_clears(glVal, grVal, gSumVal, gSumFromBinStr, NULL);
    }

#endif
    void Utils::sum(DATA_TYPE *sumResult, DATA_TYPE *lVal, DATA_TYPE *rVal) {

        ap_int<2> carry = 0;
        for(int i=0; i < DATA_ARRAY_LEN; i++) {
            DATA_TYPE_SUM sum = lVal[i] + rVal[i] + carry;
            carry[0] = sum[DATA_LEN-1] ^ sum.sign();
            if(carry[0] != 0) {
                carry.set(1, sum.sign());
            }

            if(carry != 0) {
                std::cout << "carry[" << i << "]: " << carry.to_string(AP_HEX) << " - bin: " << carry.to_string(AP_BIN) <<
                std::endl;
            }

            sumResult[i] = sum.range(DATA_LEN - 2, 0);
            sumResult[i].set(DATA_LEN - 1, sum.sign());
//
//            const std::string lVarBinString = lVal[i].to_string(AP_BIN);
//            const std::string lVarHexString = lVal[i].to_string(AP_HEX);
//
//            std::string sumBinString = sum.to_string(AP_BIN);
//            std::string sumHexString = sum.to_string(AP_HEX);

//            std::cout << "Sum " << i << std::endl << "Hex: " << sum.to_string(AP_HEX) << std::endl
//                      << "BIN: " << sum.to_string(AP_BIN) << std::endl;
//            std::cout << "Carry[" << i << "]: Hex: " << carry.to_string(AP_HEX) << " - BIN: " << carry.to_string(AP_BIN) << std::endl;
        }

#ifndef __SYNTHESIS__
        sumValidateByGMP(sumResult, lVal, rVal);
#endif
    }


}