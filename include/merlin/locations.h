/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

#include "ir_types.h"
#include "mars_opt.h"

extern bool g_muteHISError;

#if 0  //  control module log
#define LOC_INFO(x)                                                            \
  cout << "[LOC INFO][" << __func__ << ", " << __LINE__ << "]" << x << endl;
#define LOC_WARNING(x)                                                         \
  cout << "[LOC WARNING][" << __func__ << ", " << __LINE__ << "]" << x << endl;
#else
#define LOC_INFO(x)
#define LOC_WARNING(x)
#endif

#if 0
#define LOC_ERROR(x)                                                           \
  if (!g_muteHISError) {                                                       \
    cerr << "[LOC ERROR][" << __func__ << ", " << __LINE__ << "]" << x         \
         << endl;                                                              \
  } else {                                                                     \
    cerr << "[LOC WARNING][" << __func__ << ", " << __LINE__ << "]" << x       \
         << endl;                                                              \
  }
#else
#define LOC_ERROR(x)
#endif

/******************************************************************************/
/* Utility functions **********************************************************/
/******************************************************************************/

bool isStartWith(string str, string substr);
bool isKernelFile(string fileName);
bool isWrapperFile(string fileName);

/******************************************************************************/
/* Location functions *********************************************************/
/******************************************************************************/

class CSageCodeGen;

class FileLocation {
  string file_name;
  int line_num;
  int col_num = -1;

 public:
  FileLocation(const string &file_name, const int line_num,
               const int col_num = -1)
      : file_name(file_name), line_num(line_num), col_num(col_num) {}

  explicit FileLocation(const string &fileLocation);

  const string &getFileName() const { return this->file_name; }

  int getLineNum() const { return this->line_num; }

  int getColNum() const { return this->col_num; }

  bool operator==(const FileLocation &rhs) const;

  string toString() const;
};

class TopoLocation {
  int type;
  vector<int> location;

 public:
  TopoLocation() : type(-1), location({}) {}
  TopoLocation(const int type, vector<int> location)
      : type(type), location(location) {}
  explicit TopoLocation(const string &topoLocation);

  void setType(int type) { this->type = type; }

  vector<int> &getTopoVector() { return this->location; }

  string getTopoString() const;

  char getTypeString() const;

  string toString() const { return getTypeString() + getTopoString(); }
};

/******************************************************************************/
/* Locations converting APIs **************************************************/
/******************************************************************************/

SgNode *getSgNode(CSageCodeGen *ast, const string &topoString,
                  SgNode *root = nullptr);
SgNode *getSgNode(CSageCodeGen *ast, const string &fileLocation,
                  const int type);
string getFileLocation(CSageCodeGen *ast, SgNode *, int simple = 0);
string getFileLocation(CSageCodeGen *ast, const string &topoLocation,
                       int simple = 0);
string getTopoLocation(CSageCodeGen *ast, SgNode *, SgNode *root = nullptr);
string getTopoLocation(CSageCodeGen *ast, const string &fileLocation,
                       const int type);

/******************************************************************************/
/* Utility API ****************************************************************/
/******************************************************************************/

bool isSkippedIncludeFile(string filename);
bool isSkipped(CSageCodeGen *ast, SgNode *node);

string getSimpleFilename(string filename);
vector<SgNode *> orderedByFile(CSageCodeGen *ast,
                               const vector<SgNode *> &children);

//  vector<TopoLocation> getTopoLocationBytype(SgNode *node, const int);
int getChildrenNumWithoutInclude(CSageCodeGen *ast, SgNode *node);
int getTopoIndex(CSageCodeGen *ast, SgNode *child);
SgNode *getNodeByTopoIndex(CSageCodeGen *ast, SgNode *node, size_t index);

bool isCompilerGenerated(CSageCodeGen *ast, SgNode *node);
