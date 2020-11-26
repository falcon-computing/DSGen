/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights
 *reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

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
