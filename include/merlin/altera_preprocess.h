#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

int kernel_file_list_gen(CSageCodeGen &codegen, void *pTopFunc, int tool);
