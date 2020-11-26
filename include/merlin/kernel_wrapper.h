/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

//  APIs for middle-end:
int merlin_wrapper_add_sub_kernel(CMarsAST_IF *ast, void *parent_kernel,
                                  void *sub_kernel, bool is_before = true);
