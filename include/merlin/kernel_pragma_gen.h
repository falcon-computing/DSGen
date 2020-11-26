/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_KERNEL_PRAGMA_GEN_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_KERNEL_PRAGMA_GEN_H_
#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

int kernel_pragma_gen(CSageCodeGen *codegen, void *pTopFunc,
                      const CInputOptions &options);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_KERNEL_PRAGMA_GEN_H_
