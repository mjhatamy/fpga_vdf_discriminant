############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project fpga_vdf_discriminant
set_top fpga_vdf_discriminant
add_files fpga_vdf_discriminant/fpga_vdf_discriminant.cpp
add_files fpga_vdf_discriminant/fpga_vdf_discriminant.h
add_files -tb fpga_vdf_discriminant/fpga_vdf_discriminant-tb.cpp
open_solution "solution1" -flow_target vivado
set_part {xc7k410tfbg676-2}
create_clock -period 10 -name default
#source "./fpga_vdf_discriminant/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -format ip_catalog
