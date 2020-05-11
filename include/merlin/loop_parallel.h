#pragma once

#include <map>
#include <set>
#include <string>

#include "codegen.h"
#include "mars_ir.h"
#include "mars_opt.h"
#include "mars_ir_v2.h"

class LoopParallel {
 protected:
  CSageCodeGen *m_ast;
  void *mTopFunc;
  CInputOptions mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;

  // Mode options
  bool mAltera_flow;
  bool mXilinx_flow;
  bool mNaive;
  bool mAutoReg;
  bool mIvdep;
  bool mDepResolve;
  bool mLoopFlatten;
  bool mAutoPartition;

  set<void *> warned_loops;

 public:
  LoopParallel(CSageCodeGen *codegen, void *pTopFunc,
               const CInputOptions &options)
      : m_ast(codegen), mTopFunc(pTopFunc), mOptions(options),
        mAltera_flow(false), mXilinx_flow(false), mNaive(false), mAutoReg(true),
        mIvdep(false), mDepResolve(true), mLoopFlatten(true),
        mAutoPartition(true) {
    init();
  }
  void init();
  bool run();
  bool preprocess();
  void check_out_of_bound_access();
  void remove_loop_pragmas();
  void removePragmaStatement(CMirNode *new_node);

  // Xilinx flow
  int loop_parallel_xilinx_top();
  void
  parse_pragma_xilinx(map<CMirNode *, bool> node_actions,
                      std::map<void *, std::map<int, int>> array_partitions);
  void insert_pipeline(CMirNode *bNode, bool action,
                       std::map<void *, std::map<int, int>> array_partitions);
  void insert_unroll(CMirNode *bNode);

  // Intel flow
  int loop_parallel_intel_top();
  int parse_pragma_intel();

 protected:
  void check_dependency_msg(
    CMirNode *bNode, string *msg_dep,
    std::map<void *, std::map<int, int>> array_partitions);
};

bool check_legal_complete_unroll(CSageCodeGen *codegen, void *sg_loop,
                                 int64_t *l_range);

// Dependency checker
int dependency_resolve_top(CSageCodeGen *ast, void *pTopFunc,
                           const CInputOptions &options);
void *test_false_dependency(CSageCodeGen *codegen, void *sg_loop,
                            void *sg_array, int *has_write);
void check_false_dependency_xilinx(CSageCodeGen *codegen, void *sg_pragma,
                                   void *target_var);
void check_false_dependency_xilinx(CSageCodeGen *codegen, void *pTopFunc,
                                   const CInputOptions &options);
void check_false_dependency_intel(CSageCodeGen *codegen, void *pTopFunc,
                                  const CInputOptions &options);
bool check_false_dependency_intel(CSageCodeGen *codegen, void *sg_pragma,
                                  void *target_var, bool check_all = false);

// Loop tripcount process
void add_loop_tripcount_pragma(CSageCodeGen *codegen, void *pTopFunc,
                               CMarsIr *mars_ir);

// auto register and auto ivdep
int intel_auto_ivdep_top(CSageCodeGen *codegen, void *pTopFunc,
                         const CInputOptions &options);
int intel_auto_register_top(CSageCodeGen *codegen, void *pTopFunc,
                            const CInputOptions &options,
                            set<void *> *registers);
int convert_attribute_pragma_to_type(CSageCodeGen *codegen, void *pTopFunc,
                                     const CInputOptions &options);

// Message report
void reportCoarseGrainedParallel(CSageCodeGen *codegen, CMirNode *new_node);
void reportCompleteUnroll(CSageCodeGen *codegen, void *sg_loop, int range);
void reportUnrollFactor(CSageCodeGen *codegen, void *sg_loop, int factor);
void reportPipeline(CSageCodeGen *codegen, void *sg_loop, string II_factor,
                    string msg_dep);
void reportIgnorePragmaDependence(CSageCodeGen *codegen, void *sg_pragma,
                                  void *var);
void reportApplyPragmaDependence(CSageCodeGen *codegen, void *var,
                                 void *sg_loop, bool check_all = false);
void reportRiskyFalseDependence(CSageCodeGen *codegen, void *sg_loop,
                                void *var);
