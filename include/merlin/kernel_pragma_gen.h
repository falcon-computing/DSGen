#ifndef _KERNEL_PRAGMA_GEN_H_INCLUDED_
#define _KERNEL_PRAGMA_GEN_H_INCLUDED_
#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

int kernel_pragma_gen(CSageCodeGen &codegen, void *pTopFunc,
                      CInputOptions &options);

#endif
