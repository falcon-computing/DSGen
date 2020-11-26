/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

void readInterReport(string path, map<string, map<string, string>> *content);

void writeInterReport(string path,
                      const map<string, map<string, string>> &content);
