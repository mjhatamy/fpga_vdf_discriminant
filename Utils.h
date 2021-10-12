//
// Created by mjhatamy on 10/10/21.
//

#ifndef FPGA_VDF_DISCRIMINANT_UTILS_H
#define FPGA_VDF_DISCRIMINANT_UTILS_H

#include "VDFTypes.h"

namespace FpgaVdf {

    class Utils {
    public:
        static void assign(DATA_TYPE src[DATA_ARRAY_LEN], DATA_TYPE dst[DATA_ARRAY_LEN]);

        static void sum(DATA_TYPE sumResult[DATA_ARRAY_LEN], DATA_TYPE lVal[DATA_ARRAY_LEN],
                             DATA_TYPE rVal[DATA_ARRAY_LEN]);

#ifndef __SYNTHESIS__
        static void parse(const char * hashCharArray, DATA_TYPE outData[DATA_ARRAY_LEN]);
        static void sumValidateByGMP(int offset, DATA_TYPE *fpgaResult, DATA_TYPE *lVal, DATA_TYPE *rVal, const ap_int<2>& carry);
        static void sumValidateByGMP(const std::string& lValStr, std::string rValStr, DATA_TYPE *fpgaResult);
#endif
    };
}

#endif //FPGA_VDF_DISCRIMINANT_UTILS_H
