//
// Created by mjhatamy on 10/10/21.
//

#include "fpga_vdf_discriminant.h"

using namespace FpgaVdf;

void fpga_vdf_discriminant(DATA_TYPE inData[DATA_ARRAY_LEN], DATA_TYPE outData[DATA_ARRAY_LEN]) {

    DATA_TYPE sum[DATA_ARRAY_LEN];
    Utils::sum(sum, inData, inData);
    Utils::assign(sum, outData);
}
