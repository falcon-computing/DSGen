/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOWER_CPP_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOWER_CPP_H_
#include "cmdline_parser.h"
#include "codegen.h"
int lower_cpp_top(CSageCodeGen *cg, void *pTopFunc,
                  const CInputOptions &options);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOWER_CPP_H_
