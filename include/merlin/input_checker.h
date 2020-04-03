#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_INPUT_CHECKER_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_INPUT_CHECKER_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include "codegen.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "program_analysis.h"
#include "rose.h"

using MarsProgramAnalysis::CMarsExpression;
using MarsProgramAnalysis::CMarsRangeExpr;
using std::ifstream;
using std::istringstream;
using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

extern void GetTLDMInfo_withPointer4(void *sg_node, void *pArg);
int constant_propagation_top(CSageCodeGen *codegen, void *pTopFunc,
                             const CInputOptions &option);

class InputChecker {
 public:
  InputChecker(CSageCodeGen *codegen_, void *pTopFunc_,
               const CInputOptions &options_)
      : codegen(codegen_), pTopFunc(pTopFunc_), options(options_),
        mAltera_flow(false), mXilinx_flow(false), mValidAltera(true),
        mValid(true) {
    mPure_kernel = false;
    init();
  }
  InputChecker(CSageCodeGen *codegen_, void *pTopFunc_)
      : codegen(codegen_), pTopFunc(pTopFunc_), mAltera_flow(false),
        mXilinx_flow(false), mValidAltera(true), mValid(true) {
    mPure_kernel = false;
  }

  void build_mars_ir(bool check_pragma, bool pragma_in_loop, bool build_node);
  void build_mars_ir_v2(bool build_node);
  void CheckAssertHeader();
  void CheckOldStyle();
  int CheckInterfacePragma();
  void CheckUserDefinedTypeOutsideHeaderFile();
  void CheckFunctionPointerTop();
  int CheckAddressOfPortArray();
  int CheckValidTop();
  bool CheckKernelName();
  int CheckKernelArgumentNumber(vector<void *> vec_kernels, string tool);
  int CheckKernelArgumentNumberTop();
  void CheckKernelArgument(void *pKernelTop);
  int CheckKernelArgumentTop();
  void CheckMixedCCPPDesign();
  void CheckMemberFunction();
  void CheckOverloadInKernelFunction();
  void PreCheckLocalVariableWithNonConstantDimension();
  void PostCheckLocalVariableWithNonConstantDimension();
  bool CheckOpenclConflictName();
  void CheckRangeAnalysis();
  void ExpandDefineDirectiveInPragma();
  int IsInterfaceRelated(vector<string> depths, void *kernel);
  int CheckMaxDepths(vector<string> depths);
  int CheckDepths(vector<string> depths, void *kernel, void *var_init);
  void CheckInvalidVariableInPragma();
  void check_result();
  set<void *> GetPortAlias(void *var, void *pos, set<void *> *p_visited);
  int CheckAddressOfOnePort(void *array, void *pos,
                            const set<void *> ports_alias, bool top);
  void PreCheck();
  void PostCheck();

  bool mPure_kernel;

 private:
  CSageCodeGen *codegen;
  void *pTopFunc;
  CInputOptions options;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;
  bool mAltera_flow;
  bool mXilinx_flow;
  bool mValidAltera;
  // bool mValidXilinx;
  bool mValid;
  unordered_map<string, string> def_directive_map_;

 private:
  void init();
  void CheckLocalVariableWithNonConstantDimension(void *func);
  void CheckValidTop(void *func, set<void *> *p_visited, int top);
  int CheckAddressOfOnePortImpl(void *array, void *pos,
                                const set<void *> ports_alias, bool top,
                                set<void *> *p_visited);
  vector<void *> GetTopKernels();
  void CheckFunctionPointer(void *func, set<void *> *p_visited);
  void ScanDefineDirectives();
  void SetDefineDirectives(string);
  void EmbedRecursiveDefines();
  int EvaluatePragmaAttributes();
  void ReplaceTokens(vector<string> &, string);
  bool HasDefNameToken(vector<string> &, unordered_set<string> &);
  bool HasInvalidCalcChar(string &);
  int64_t Calculate(string &);
  int64_t ParseExpr(string &, int &);
  int64_t ParseNum(string &, int &);
  void StripComments(string &);
};

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_INPUT_CHECKER_H_
