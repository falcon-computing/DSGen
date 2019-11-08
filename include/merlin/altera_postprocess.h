#pragma once

#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "xml_parser.h"

#include "bsuNode_v2.h"
#include "codegen.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "program_analysis.h"
#include "rose.h"

int remove_redundancy(CSageCodeGen *codegen, void *pTopFunc);
