#pragma once

#include "cmdline_parser.h"
#include "mars_ir.h"

int loop_flatten_for_pipeline(CSageCodeGen &codegen, void *pTopFunc,
                              CInputOptions &options, CMarsIr &mars_ir);
