//
// Created by mjhatamy on 10/10/21.
//

#ifndef FPGA_VDF_DISCRIMINANT_VDFTYPES_H
#define FPGA_VDF_DISCRIMINANT_VDFTYPES_H

#define AP_INT_MAX_W 4096

#include <ap_int.h>
#include <regex>

namespace FpgaVdf {
#define DATA_ARRAY_LEN 4
#define DATA_LEN 1025
    typedef ap_int<DATA_LEN> DATA_TYPE;
    typedef ap_int<DATA_LEN + 1> DATA_TYPE_SUM;
    typedef ap_int<(DATA_LEN * 2) - 1> DATA_TYPE_MUL;


    template<int LEN, int SIZE>
    struct FpgaVdfIntType {
        typedef ap_int<DATA_LEN> VALUE_TYPE;
        typedef ap_int<LEN + 1> SUM_VALUE_TYPE;
        typedef ap_int<(LEN * 2) - 1> MUL_VALUE_TYPE;
        typedef ap_int<LEN> BaseApInt;
        ap_int<LEN> val[SIZE];

        FpgaVdfIntType() {
            for (int i = 0; i < SIZE; i++) {
                this->val[i] = 0;
            }
        }

        FpgaVdfIntType(int i[SIZE]) {

        }

        FpgaVdfIntType(int arg...) {
            va_list args;
            va_start(args, arg);
            int value = arg;
            int count = 0;
            while (count < SIZE) {
                this->val[count] = value;
                value = va_arg(args, int);
                count++;
            }
            va_end(args);
        }

        FpgaVdfIntType &operator=(FpgaVdfIntType value) {
            for (int i = 0; i < SIZE; i++) {
                this->val[i] = value[i];
            }
            return (*this);
        }

        FpgaVdfIntType &operator+=(FpgaVdfIntType rVal) {
            ap_int<2> carry = 0;
            for (int i = 0; i < SIZE; i++) {
                DATA_TYPE_SUM sum = this[i] + rVal[i] + carry;

                carry[0] = sum[LEN - 1] ^ sum.sign();
                if (carry[0] != 0) {
                    carry.set(1, sum.sign());
                } else {
                    carry[1] = 0;
                }
                this[i] = sum.range(LEN - 2, 0);
                this[i].set(LEN - 1, sum.sign());
            }
            return (*this);
        }

        ap_int<LEN> &operator[](int index) {
            return this->val[index];
        }

        static void
        mul(FpgaVdfIntType<LEN, SIZE> &dst, FpgaVdfIntType<LEN, SIZE> &lVal, FpgaVdfIntType<LEN, SIZE> &rVal) {
            VALUE_TYPE carry[SIZE][SIZE] = { 0, 0, 0, 0 };
            SUM_VALUE_TYPE carryX[SIZE] = { 0, 0, 0, 0 };
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    MUL_VALUE_TYPE res = lVal.val[j] * rVal.val[i];
                    if(j > 0) {
                        res += carryX[i];
                    }
                    if(i > 0 & (j+1) < SIZE) {
                        res += carry[i-1][j+1];
                    }
                    carry[i][j] = res.range(LEN-2, 0);
                    carry[i][j].set(LEN - 1, res.sign());

                    carryX[i] = res.range((LEN * 2) - 3, LEN - 1);
                    carryX[i].set(LEN - 1, res.sign());
                }
            }
            for (int i = 0; i < SIZE; i++) {
                dst.val[i] = carry[i][0];
            }
#ifndef __SYNTHESIS__
            validateMul(lVal, rVal, dst);
#endif
        }

#ifndef __SYNTHESIS__

        static void validateMul(FpgaVdfIntType<LEN, SIZE> &lVal, FpgaVdfIntType<LEN, SIZE> &rVal,
                                FpgaVdfIntType<LEN, SIZE> &fpgaResult) {
            mpz_t glVal, grVal, gSumVal, gFpgaSumResult;
            mpz_inits(glVal, grVal, gSumVal, gFpgaSumResult, NULL);

            mpz_set_str(gFpgaSumResult, fpgaResult.to_string(AP_HEX).c_str(), 0);

            // Calculate by GMP
            mpz_set_str(glVal, lVal.to_string(AP_HEX, true).c_str(), 0);
            mpz_set_str(grVal, rVal.to_string(AP_HEX, true).c_str(), 0);
            mpz_mul(gSumVal, glVal, grVal);

            if (mpz_cmp(gSumVal, gFpgaSumResult) != 0) {
                gmp_printf("Expected Result==>\nHEX: %#Zx\nFPGAResult=>\nHex: %#Zx\n\n", gSumVal, gFpgaSumResult);
                gmp_printf("glVal: %#Zx\ngrVal: %#Zx\n\n", glVal, grVal);
                //std::cout << "FPGA Result Printed" << std::endl << fpgaResult.to_string(AP_HEX) << std::endl;
                mpz_clears(glVal, grVal, gSumVal, gFpgaSumResult, NULL);
                throw std::out_of_range("Sum result is not equal results from GMP Library.");
            } else {
                std::cout << "MUL Validated by GMP" << std::endl;
            }
            mpz_clears(glVal, grVal, gSumVal, gFpgaSumResult, NULL);
        }

#endif

        static void
        add(FpgaVdfIntType<LEN, SIZE> &dst, FpgaVdfIntType<LEN, SIZE> &lVal, FpgaVdfIntType<LEN, SIZE> &rVal) {
            ap_int<2> carry = 0;
            for (int i = 0; i < SIZE; i++) {
                DATA_TYPE_SUM sum = lVal[i] + rVal[i] + carry;

                carry[0] = sum[LEN - 1] ^ sum.sign();
                if (carry[0] != 0) {
                    carry.set(1, sum.sign());
                } else {
                    carry[1] = 0;
                }
                dst.val[i] = sum.range(LEN - 2, 0);
                dst.val[i].set(LEN - 1, sum.sign());
            }
#ifndef __SYNTHESIS__
            validateAdd(lVal, rVal, dst);
#endif
        }

#ifndef __SYNTHESIS__

        static void validateAdd(FpgaVdfIntType<LEN, SIZE> &lVal, FpgaVdfIntType<LEN, SIZE> &rVal,
                                FpgaVdfIntType<LEN, SIZE> &fpgaResult) {
            mpz_t glVal, grVal, gSumVal, gFpgaSumResult;
            mpz_inits(glVal, grVal, gSumVal, gFpgaSumResult, NULL);

            mpz_set_str(gFpgaSumResult, fpgaResult.to_string(AP_HEX).c_str(), 0);

            // Calculate by GMP
            mpz_set_str(glVal, lVal.to_string(AP_HEX).c_str(), 0);
            mpz_set_str(grVal, rVal.to_string(AP_HEX).c_str(), 0);
            mpz_add(gSumVal, glVal, grVal);

            if (mpz_cmp(gSumVal, gFpgaSumResult) != 0) {
                gmp_printf("Expected Result==>\nHEX: %#Zx\nFPGAResult=>\nHex: %#Zx", gSumVal, gFpgaSumResult);
                mpz_clears(glVal, grVal, gSumVal, gFpgaSumResult, NULL);
                throw std::out_of_range("Sum result is not equal results from GMP Library.");
            } else {
                std::cout << "SUM Validated by GMP" << std::endl;
            }
            mpz_clears(glVal, grVal, gSumVal, gFpgaSumResult, NULL);
        }

#endif

#ifndef __SYNTHESIS__

        FpgaVdfIntType(std::string value) {
            std::string hashString(value);
            std::regex isMinusRegex("^(-0x).*$");
            std::regex hexPrefixRegex("^(0x|-0x)");

            bool isMinus = regex_match(hashString, isMinusRegex);
            std::string hexPrefix = isMinus ? "-0x" : "0x";

            hashString = std::regex_replace(hashString, hexPrefixRegex, "");

            int offset = 0;
            for (int i = (int) hashString.length(); i > 0;) {
                if (i - 257 >= 0) {
                    val[offset] = BaseApInt(hashString.substr(i - 256, 256).insert(0, hexPrefix).c_str(), 0);
                } else {
                    val[offset] = BaseApInt(hashString.substr(0, i).insert(0, hexPrefix).c_str(), 0);
                }
                std::cout << "i:" << i << " " << std::endl;
                i -= 256;
                offset++;
                if (offset > SIZE) {
                    throw std::out_of_range("out of range");
                }
            }
            for (; offset < SIZE; offset++) {
                val[offset] = 0;
            }
        }

#endif
#ifndef __SYNTHESIS__

        std::string to_string(signed char radix = AP_BIN, bool sign = true) {
            bool isMinus = this->val[0] < 0;
            std::string fpgaResultStr;
            std::regex replaceHex("^(-0x|0x)");
            std::regex replaceInitialZeroHex("^(-0x0|0x0)");

            for (int i = 0; i < SIZE; i++) {
                if (this->val[i] == 0 && i > 0) continue;
                std::string res = std::regex_replace(this->val[i].to_string(radix, sign), replaceInitialZeroHex, "");
                res = std::regex_replace(res, replaceHex, "");
                fpgaResultStr.insert(0, res);
            }
            fpgaResultStr.insert(0, radixString(radix, isMinus));
            return fpgaResultStr;
        }

        int to_mpz_t(mpz_t val) {
            std::string str = this->to_string(AP_HEX);
            mpz_init_set_str(val, str.c_str(), 0);
        }

#endif
#ifndef __SYNTHESIS__
    private:
        std::string radixString(signed char radix, bool isMinus = false) {
            std::string result;
            switch (radix) {
                case 2:
                    result += isMinus ? "-0b" : "0b";
                    break;
                case 8:
                    result += isMinus ? "-0o" : "0o";
                    break;
                case 16:
                    result += isMinus ? "-0x" : "0x";
                    break;
                default:
                    assert("invalid radix" && 0);
            }
            return result;
        }

#endif
    };

    typedef FpgaVdfIntType<1025, 5> FpgaVdfIntArray;

}


#endif //FPGA_VDF_DISCRIMINANT_VDFTYPES_H
