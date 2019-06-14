#ifndef _MEMORY_COALESCING_INTEL_H_INCLUDED_
#define _MEMORY_COALESCING_INTEL_H_INCLUDED_

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

class MemoryCoalescingIntel {
  CSageCodeGen &m_ast;
  void *mTopFunc;
  CInputOptions &mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;
  int mBitWidth;
  bool m_valid;
  bool mNaiveHLS;
  bool mBitWidthSpecified;
  const unsigned int m_unroll_threshold = 64;
  const int m_max_bitwidth = 512;

public:
  MemoryCoalescingIntel(CSageCodeGen &Codegen, void *pTopFunc,
                        CInputOptions &Options)
      : m_ast(Codegen), mTopFunc(pTopFunc), mOptions(Options), mBitWidth(512),
        m_valid(true), mNaiveHLS(false), mBitWidthSpecified(false),
        m_count_memcpy(0) {
    init(Codegen);
  }
  bool run();

  ~MemoryCoalescingIntel();

private:
  bool update_interface(CMirNode *fNode);

  void init(CSageCodeGen &Codegen);

  void collectInterfaceInfo();

  void collectInterfaceInfo(void *func_def);

  bool updateFunctionInterface();

  bool updateFunctionInterface(void *func_def);

  bool isMemcpy(void *func_call);

  bool updateMemcpyCallWithLargeBitWidth(const MemcpyInfo &info, void *scope);
  bool isSupportedType(void *base_type);

  bool checkInvalidBitWidth(int opt_bitwidth, int orig_bitwidth);

  bool check_declaration(void *func_decl, string var_name);

  int memcpy_replace(const MemcpyInfo &info, void *func_call);

  int handle_single_assignment(void *sg_array1, vector<void *> &sg_idx1,
                               vector<size_t> &dim1, void *sg_array2,
                               vector<void *> &sg_idx2, vector<size_t> &dim2,
                               void *sg_length, int type_size, void *func_call);

  int generate_nested_loops(void *sg_array1, vector<void *> &sg_idx1,
                            vector<size_t> &dim1, void *sg_array2,
                            vector<void *> &sg_idx2, vector<size_t> &dim2,
                            void *sg_length, int type_size, void *func_call,
                            const MemcpyInfo &info);

private:
  set<void *> mGlobals_with_memcpy_header;
  set<void *> mGlobals_with_ap_int_header;
  vector<void *> mFunc_defs;
  map<void *, set<string>> mMacroCall;
  // key is the first declaration with no definition
  map<void *, vector<ArgumentInfo>> mFunc2Info;
  set<void *> mDead_func_calls;

  const vector<string> m_supported_types{"int",
                                         "unsigned int",
                                         "signed int",
                                         "short",
                                         "signed short",
                                         "unsigned short",
                                         "char",
                                         "signed char",
                                         "unsigned char",
                                         "long",
                                         "signed long",
                                         "unsigned long",
                                         "float",
                                         "double"};
  int m_count_memcpy;
};

#endif
