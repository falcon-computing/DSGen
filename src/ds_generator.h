//===----------------------------------------------------------------------===//
//
// The Merlin(TM) Compiler Design Space Explorer
//
// Copyright (C) Falcon Computing Solutions, Inc - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
//
//===----------------------------------------------------------------------===//
//
// Owner: Min Gao, Cody Yu
///
/// \file ds_generator.h
/// \brief Automatic design space pragma generation for user code.
/// The design space generator automatically generate/append pipeline, tiling
/// and parallel pragmas for each loop scope. The dependency order of the three
/// sorts are pipleline <-- tiling <-- parallel.
//
//===----------------------------------------------------------------------===//
#ifndef DS_GENERATOR_H_
#define DS_GENERATOR_H_
#include <time.h>
#include <sys/time.h>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <json/json.h> 
#include <boost/any.hpp>

#include "codegen.h"
#include "input_checker.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "dse_utils.h"


using namespace MarsProgramAnalysis;
using namespace std;

#define MIN_TILE_SIZE 4
#define MAX_TILE_SIZE 16384

enum PragmaType {
  PIPELINE,
  TILING,
  PARALLEL,
  INTERFACE
};


static string GetKeyWord(PragmaType pragma_ty) {
  string ret;
  switch (pragma_ty) {
    case PIPELINE: {
      ret = "pipeline";
      break;
    } 
    case TILING: {
      ret = "til";
      break;
    } 
    case PARALLEL: {
      ret = "parallel";
      break;
    }
    case INTERFACE: {
      ret = "interface";
      break;
    }
    default: {
      ret = "UNKNOWN";
      assert(0 && "unknown keyword.\n");
    }
  }
  return ret;
}

static string GetTypeName(PragmaType pragma_ty) {
  string ret = GetKeyWord(pragma_ty);
  if (ret == "til") {
    return "TILING";
  }
  else {
    transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return ret;
  }
}

template <typename T> 
struct PragmaInfo {
  string id;
  PragmaType type;
  string order;
  string cond;
  vector<T> options;
  T default_value;
  /// void * -> SgPragmaStatement
  void *pragma;
  void *pos;

  void SetBasicValue(void *stmt, string pragma_id, PragmaType pragma_type, void *ins_pos=nullptr) {
    pragma = stmt; 
    id = pragma_id;
    type = pragma_type;
    order = "";
    pos = ins_pos;
  }
  void AppendOption(T option) {
    options.push_back(option);
  }
  void AppendOption(vector<T> vec) {
    for (auto &option: vec)
      options.push_back(option);
  }
  void SetCond(string condition) {
    cond = condition;
  }
  void SetOrder(string ord) {
    order = ord;
  }
  void SetDefault(T val) {
    default_value = val;
  }
  void PrintToJson(ofstream &fout, Json::Value &item) {
    Json::Value entry;
    stringstream ss;
    string delim = ",";
    for (auto &option : options) {
      ss << option;
      ss << delim;
    }
    string all_options = ss.str(); 
    if (!all_options.empty())
      all_options.pop_back();
    string DS = "[x for x in [" + all_options + "]" + cond + "]";
    entry["options"] = DS;
    if (!order.empty())
      entry["order"] = order;
    entry["default"] = default_value;
    entry["ds_type"] = GetTypeName(type);
    item[id] = entry;
  }
};

class ScopePragmas {
 public:
  ScopePragmas() {}
  ~ScopePragmas() {
    //for (auto item: map_category_pragmas_) {
    //  delete item.second;
    //}
  }
  inline void set(PragmaType key, boost::any val) {
    map_category_pragmas_[key] = val;
  }
  inline boost::any get(PragmaType key) {
    return map_category_pragmas_[key];
  }
  inline bool exist(PragmaType key) {
    return map_category_pragmas_.count(key);
  }
 private:
  /// void * -> SgPragmaStatement
  map<PragmaType, boost::any> map_category_pragmas_;
};


/// void * -> SgScope
typedef map<void *, ScopePragmas *> PragmaMap; 

class DsGenerator;
typedef void (DsGenerator::*FnHandler) (void *, string);

class DsGenerator {
 public:
  DsGenerator(CSageCodeGen &ast, void *pTopFunc, CInputOptions &options) 
    : m_ast(ast), 
      p_top_func_(pTopFunc),
      options_(options) {
    mars_ir_.get_mars_ir(&m_ast, p_top_func_, options, true);
    mars_ir_v2_.build_mars_ir(&m_ast, p_top_func_);
    loop_index_ = 0;
    fout.open("ds_info.json");
    is_fine_grain_ = options_.get_option("-fgrain") == "on";
    // cout << "=====\n";
    // cout << options_.get_option("-fgrain") << endl;
    // cout << "=====\n";
  }
  virtual ~DsGenerator() {
    for (auto item: map_scope_pragmas_) {
      delete item.second;
    }
    fout.close();
  }
  int InitScopePragmas();
  void InsertCreatedPragmasAndOutputJson();
  inline PragmaMap &GetMapScopePragmas() {
    return map_scope_pragmas_;
  }
  inline CMarsIr &GetCMarsIr() {
    return mars_ir_;
  }
  inline CMarsIrV2 &GetCMarsIrV2() {
    return mars_ir_v2_;
  }
 public:
  CSageCodeGen &m_ast;
 // public for unit testing purpose.
 // should be private when release.
 public:
  inline void build_mars_ir(bool check_pragma, bool pragma_in_loop,
                               bool build_node) {
    //  build Mars IR
    mars_ir_.clear();
    mars_ir_.get_mars_ir(&m_ast, p_top_func_, options_, build_node, false,
                       pragma_in_loop);
  }
  inline void clear_mars_ir() {
    mars_ir_.clear();
  }
  void CanonicalizeIR();
  bool CanonicalizeLoop();
  bool StandardizeLoop();
  void TraverseLoopDesignSpace(FnHandler fn, void *stmt, string loop_id); 
  bool UpdateUserSpecifiedPragma(void *scope_stmt, 
                                 PragmaType pragma_ty,
                                 string pragma_id);
  void BuildPipeline(void *stmt, string loop_code);
  void BuildTiling(void *stmt, string loop_code);
  void BuildParallel(void *stmt, string loop_code);
  void BuildBitWidthOpt(void *kernel);
  void SetUserPragmaInfo(void *scope_stmt, void *pragma_stmt, 
                         string pragma_id, PragmaType pragma_ty);
  void GetShadowedParams(void *stmt,
                         PragmaType pragma_ty,
                         vector<string> &free_ids,
                         vector<string> &fixed_ids);
  vector<int> GetAllDivisors(int num, int limit);
  inline bool IsTilable(void *loop, 
                        int tc, bool isInnermost) {
    if (tc < MIN_TILE_SIZE)
      return false;
    if (isInnermost)
      return false;
    return true;
  }
  long GetTotalDepth(string depth);
 private:  
  PragmaMap map_scope_pragmas_;
  CMarsIr mars_ir_;
  CMarsIrV2 mars_ir_v2_;
  void *p_top_func_;
  CInputOptions &options_;
  int loop_index_;
  bool is_fine_grain_;
  std::ofstream fout;
  Json::Value item;
};

int ds_generator_top(CSageCodeGen &m_ast, void *pTopFunc,
                    CInputOptions &options);
#endif
