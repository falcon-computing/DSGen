#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_KERNEL_SEPARATE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_KERNEL_SEPARATE_H_
#include "cmdline_parser.h"
#include "file_parser.h"
#include "xml_parser.h"

#include "PolyModel.h"
#include "codegen.h"
#include "mars_message.h"
#include "mars_opt.h"
#include "tldm_annotate.h"

#include "mars_ir_v2.h"

#include "postwrap_process.h"

int kernel_separate_top(CSageCodeGen *codegen, void *pTopFunc,
                        const CInputOptions &options);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_KERNEL_SEPARATE_H_
