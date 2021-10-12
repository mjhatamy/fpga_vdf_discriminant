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
        for (int i = 0; i < DATA_ARRAY_LEN; i++) {
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

        for (int i = (int) hashString.length(), offset = 0; i > 0;) {
            if (i - 257 >= 0) {
                outData[offset] = DATA_TYPE(hashString.substr(i - 256, 256).insert(0, hexPrefix).c_str(), 0);
            } else {
                outData[offset] = DATA_TYPE(hashString.substr(0, i).insert(0, hexPrefix).c_str(), 0);
            }
            std::cout << "i:" << i << " " << std::endl;
            i -= 256;
            offset++;
        }
    }

    /**
     *
     * @param lValStr
     * @param rValStr
     * @param fpgaResult
     * @param lVal
     * @param rVal
     */
    void Utils::sumValidateByGMP(const std::string& lValStr, std::string rValStr, DATA_TYPE *fpgaResult ) {
        mpz_t glVal, grVal, gSumVal, gSumFromBinStr;
        mpz_inits(glVal, grVal, gSumVal, gSumFromBinStr, NULL);
        mpz_set_str(glVal, lValStr.c_str(), 0);
        mpz_set_str(grVal, rValStr.c_str(), 0);
        mpz_add(gSumVal, glVal, grVal);

        char gmpResultCharArray[9192];
        gmp_sprintf(gmpResultCharArray, "%#Zx", gSumVal);
        std::string gmpResultStr(gmpResultCharArray);

        std::string fpgaResultStr;
        std::regex replaceHex("^(-0x|0x)");
        std::regex replaceInitialZeroHex("^(-0x0|0x0)");
        for(int i=0; i < DATA_ARRAY_LEN; i++) {
            std::string res = std::regex_replace(fpgaResult[i].to_string(AP_HEX), replaceInitialZeroHex, "");
            res = std::regex_replace(res, replaceHex, "");
            fpgaResultStr.insert(0, res);
            std::cout << "FPGA: " << res << std::endl;
        }
        std::cout << "FPGA        : 0x" << fpgaResultStr << std::endl;
        std::cout << "gmpResultStr: " << gmpResultStr << std::endl;

        mpz_clears(glVal, grVal, gSumVal, gSumFromBinStr, NULL);
    }
    void Utils::sumValidateByGMP(int offset, DATA_TYPE *fpgaResult, DATA_TYPE *lVal, DATA_TYPE *rVal, const ap_int<2>& carry) {
        mpz_t glVal, grVal, gSumVal, gSumFromBinStr;
        mpz_inits(glVal, grVal, gSumVal, gSumFromBinStr, NULL);

        mpz_set_str(glVal, lVal->to_string(AP_HEX, true).c_str(), 0);
        mpz_set_str(grVal, rVal->to_string(AP_HEX, true).c_str(), 0);

        std::string binVal = fpgaResult->to_string(AP_HEX);
        mpz_set_str(gSumFromBinStr, binVal.c_str(), 0);
        mpz_add(gSumVal, glVal, grVal);
        if (mpz_cmp(gSumVal, gSumFromBinStr) != 0) {
            std::cout << "FPGA_VDF SUM function ERROR detected by GMP for index" << std::endl;
            gmp_printf("GMP:> lVal: %#Zx\n", glVal);
            gmp_printf("GMP:> rVal: %#Zx\n", grVal);
            gmp_printf("GMP:> gSumVal: %#Zx\n", gSumVal);
            gmp_printf("GMP:> gSumFromBinStr: %#Zx\n", gSumFromBinStr);

            throw std::invalid_argument("Sum calculation is incorrect");
        }
        std::cout << "FPGA_VDF SUM function validated by GMP in offset [" << offset << "] and carry: " <<
        carry.to_string(AP_HEX) << "." << std::endl;
        mpz_clears(glVal, grVal, gSumVal, gSumFromBinStr, NULL);
    }

#endif

    void Utils::sum(DATA_TYPE *sumResult, DATA_TYPE *lVal, DATA_TYPE *rVal) {

        ap_int<2> carry = 0;
        for (int i = 0; i < DATA_ARRAY_LEN; i++) {
            DATA_TYPE_SUM sum = lVal[i] + rVal[i] + carry;

            carry[0] = sum[DATA_LEN - 1] ^ sum.sign();
            if (carry[0] != 0) {
                carry.set(1, sum.sign());
            } else {
                carry[1] = 0;
            }
            sumResult[i] = sum.range(DATA_LEN - 2, 0);
            sumResult[i].set(DATA_LEN - 1, sum.sign());

#ifndef __SYNTHESIS__
            if (carry != 0) {
                std::cout << "carry[" << i << "]: " << carry.to_string(AP_HEX) << " - bin: "
                          << carry.to_string(AP_BIN) << std::endl;
            }
           // sumValidateByGMP(i, &sumResult[i], &lVal[i], &rVal[i], carry);
#endif
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


    }


}