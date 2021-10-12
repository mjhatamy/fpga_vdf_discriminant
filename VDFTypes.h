//
// Created by mjhatamy on 10/10/21.
//

#ifndef FPGA_VDF_DISCRIMINANT_VDFTYPES_H
#define FPGA_VDF_DISCRIMINANT_VDFTYPES_H

#define AP_INT_MAX_W 4096
#include <ap_int.h>

namespace FpgaVdf {
#define DATA_ARRAY_LEN 4
#define DATA_LEN 1025
typedef ap_int<DATA_LEN> DATA_TYPE;
typedef ap_int<DATA_LEN + 1> DATA_TYPE_SUM;

}


#endif //FPGA_VDF_DISCRIMINANT_VDFTYPES_H
