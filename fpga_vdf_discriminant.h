//
// Created by mjhatamy on 10/10/21.
//

#ifndef FPGA_VDF_DISCRIMINANT_H
#define FPGA_VDF_DISCRIMINANT_H

#include "VDFTypes.h"
#include "Utils.h"

typedef struct  {
    FpgaVdf::FpgaVdfIntArray a;
    FpgaVdf::FpgaVdfIntArray b;
    FpgaVdf::FpgaVdfIntArray c;
    FpgaVdf::FpgaVdfIntArray d;

    FpgaVdf::FpgaVdfIntArray denom;
} Form;

void fpga_vdf_discriminant(FpgaVdf::FpgaVdfIntArray *inData, FpgaVdf::FpgaVdfIntArray &outData);

#endif //FPGA_VDF_DISCRIMINANT_FPGA_VDF_DISCRIMINANT_H
