#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_MEMORY_COALESCING_XILINX_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_MEMORY_COALESCING_XILINX_H_
#include <map>
#include <set>
#include <vector>
#include <string>
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

class MemoryCoalescingXilinx {
  CSageCodeGen *m_ast;
  void *mTopFunc;
  CInputOptions mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;
  int mBitWidth;
  bool mBitWidthSpecified;
  int mMaxDimension;
  bool mNaiveHLS;
  bool mEnableCDesign;
  bool mEnableSubOptimal;
  std::set<int> mBitWidthSet;
  size_t m_length_threshold;
  bool mCpp_design;
  bool mWideBusFlag;
  std::set<string> mCoalescingHeader;

 public:
  MemoryCoalescingXilinx(CSageCodeGen *Codegen, void *pTopFunc,
                         const CInputOptions &Options)
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

  void init(CSageCodeGen *Codegen);

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

  bool updateMemcpyCallWithLargeBitWidth(const MemcpyInfo &info,
                                         void *orig_memcpy_call);
  bool isSupportedType(void *base_type);

  std::string StripSignnessInType(const std::string &org_type,
                                  bool replace_space);

  void *AddCastToStripSignness(void *address_exp, void *base_type,
                               const MemcpyInfo &info);

  void AddMemCpyHeader(void *scope);

  void AddDataTypeHeader(void *scope);

  void AddPartitionPragma(const std::string &buf_name, void *stmt,
                          const MemcpyInfo &info);

  bool checkInvalidBitWidth(int opt_bitwidth, int orig_bitwidth);

  bool updateSingleAssignWithLargeBitWidth(const SingleAssignInfo &info,
                                           void *base_type, void *orig_access,
                                           void *kernel_decl);

  bool isUnsupportedSingleAssign(void *single_assign);

  void *createBuffer(void *base_type, void *sg_scope, void *func_decl);

  void detect_access_in_paralleled_loop(void *ref, ArgumentInfo *info);

  void auto_cache_for_coalescing();

  void parse_memcpy_info(void *func_call,
                         const std::map<void *, int> &paralists,
                         std::vector<ArgumentInfo> *arg_info_);

 private:
  std::set<void *> mGlobals_with_memcpy_header;
  std::set<void *> mGlobals_with_data_type_header;
  std::vector<void *> mFunc_defs;
  std::map<void *, std::set<std::string>> mMacroCall;
  //  key is the first declaration with no definition
  std::map<void *, std::vector<ArgumentInfo>> mFunc2Info;
  std::set<void *> mDead_func_calls;

  const std::vector<std::string> m_supported_types{"int",
                                                   "unsigned int",
                                                   "signed int",
                                                   "char",
                                                   "signed char",
                                                   "unsigned char",
                                                   "short",
                                                   "signed short",
                                                   "unsigned short",
                                                   "long",
                                                   "signed long",
                                                   "unsigned long",
                                                   "long long",
                                                   "signed long long",
                                                   "unsigned long long",
                                                   "float",
                                                   "double"};
};

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_MEMORY_COALESCING_XILINX_H_
