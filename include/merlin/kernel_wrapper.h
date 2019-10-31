#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

// APIs for middle-end:
int merlin_wrapper_add_sub_kernel(CMarsAST_IF *ast, void *parent_kernel,
                                  void *sub_kernel, bool is_before = true);
