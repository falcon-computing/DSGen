#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

#include "codegen.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "rose.h"

int func_inline(CSageCodeGen &codegen, void *func_call, enum effort curr_flow,
                bool transform, bool report);
