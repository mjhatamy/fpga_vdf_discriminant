//
// Created by mjhatamy on 10/10/21.
//

#include "fpga_vdf_discriminant.h"

using namespace FpgaVdf;

void fpga_vdf_discriminant(FpgaVdfIntArray *inData, FpgaVdfIntArray &outData) {

    Form f;
    f.d = -2; //*inData;

    //std::cout << "D: " << f.d.to_string(AP_HEX) << std::endl;
    //FpgaVdfIntArray::add(f.a, f.d, *inData);
    //FpgaVdfIntArray::mul(f.a, f.d, *inData);
    FpgaVdfIntArray::div(f.a, *inData, f.d);

    //std::cout << "A: " << f.a.to_string(AP_HEX) << std::endl;
    //DATA_TYPE sum[DATA_ARRAY_LEN];
    //Utils::sum(sum, inData, inData);
    //Utils::assign(sum, outData);
    outData = f.a;
}
