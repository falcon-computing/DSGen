#ifndef _MEMORY_COALESCING_XILINX_H_INCLUDED_
#define _MEMORY_COALESCING_XILINX_H_INCLUDED_

#include "rose.h"

#include "PolyModel.h"
#include "bitwidth_opt.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

using namespace std;
// using namespace SageInterface;
// using namespace SageBuilder;
using namespace MarsProgramAnalysis;

class MemoryCoalescingXilinx {
  CSageCodeGen &m_ast;
  void *mTopFunc;
  CInputOptions &mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;
  int mBitWidth;
  bool mBitWidthSpecified;
  int mMaxDimension;
  bool mNaiveHLS;
  bool mEnableCDesign;
  bool mEnableSubOptimal;
  set<int> mBitWidthSet;
  size_t m_length_threshold;
  bool mCpp_design;
  bool mWideBusFlag;

public:
  MemoryCoalescingXilinx(CSageCodeGen &Codegen, void *pTopFunc,
                         CInputOptions &Options)
      : m_ast(Codegen), mTopFunc(pTopFunc), mOptions(Options), mBitWidth(512),
        mBitWidthSpecified(false), mMaxDimension(5), mNaiveHLS(false),
        mEnableCDesign(false), mEnableSubOptimal(false),
        m_length_threshold(1024), mCpp_design(false), mWideBusFlag(false) {
    init(Codegen);
  }
  bool run();

  ~MemoryCoalescingXilinx();

private:
  bool update_interface(CMirNode *fNode);

  void init(CSageCodeGen &Codegen);

  void collectInterfaceInfo();

  void InitInterfaceInfo(void *func_def);

  void backwardInterfaceInfo(void *func_def);

  void forwardInterfaceInfo(void *func_def);

  void AdjustInterfaceInfo();
  bool updateKernelFunction();

  bool updateFunctionInterface();

  bool updateFunctionInterface(void *func_def);

  void *getLargeBitWidthType(void *orig_type, int opt_bitwidth);

  void *getLargeBitWidthTypeWithoutModifier(void *orig_type, int opt_bitwidth);

  bool isMemcpy(void *func_call);

  bool updateMemcpyCallWithLargeBitWidth(const MemcpyInfo &info, void *scope);
  bool isSupportedType(void *base_type);

  string StripSignnessInType(const string &org_type, bool replace_space);

  void *AddCastToStripSignness(void *address_exp, void *base_type,
                               const MemcpyInfo &info);

  void AddMemCpyHeader(void *scope);

  void AddDataTypeHeader(void *scope);

  void AddPartitionPragma(const string &buf_name, void *stmt,
                          const MemcpyInfo &info);

  bool checkInvalidBitWidth(int opt_bitwidth, int orig_bitwidth);

  bool updateSingleAssignWithLargeBitWidth(const SingleAssignInfo &info,
                                           void *base_type, void *orig_assign,
                                           void *func_decl);

  bool isUnsupportedSingleAssign(void *single_assign);

  void *createBuffer(void *base_type, void *sg_scope, void *func_decl);

  bool check_declaration(void *func_decl, string var_name);

  bool detect_access_in_paralleled_loop(void *ref);

private:
  set<void *> mGlobals_with_memcpy_header;
  set<void *> mGlobals_with_data_type_header;
  vector<void *> mFunc_defs;
  map<void *, set<string>> mMacroCall;
  // key is the first declaration with no definition
  map<void *, vector<ArgumentInfo>> mFunc2Info;
  set<void *> mDead_func_calls;

  const vector<string> m_supported_types{
      "int",   "unsigned int", "signed int",
      "char",  "signed char",  "unsigned char",
      "short", "signed short", "unsigned short",
      "long",  "signed long",  "unsigned long",
      "float", "double"};
};

#endif
