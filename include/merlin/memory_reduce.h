#ifndef __MEMORY_REDUCE_H__
#define __MEMORY_REDUCE_H__
#include "codegen.h"
#include "mars_ir_v2.h"
class MemoryReduce {
  CSageCodeGen &mCodegen;
  CMarsIrV2 mMars_ir;
  enum reduce_type {
    ONE_ITERATION = 0,
    NO_INTRA_DEP = 1,
  };

public:
  MemoryReduce(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &option);
  bool run();

  bool process_one_node(CMarsNode *node, enum reduce_type rt);

  bool optimize_local_memory_layout(CMarsNode *node);

  void check_const_indices(
      set<int> &reduced_dims,
      vector<tuple<void *, int64_t, int64_t>> &vec_index_info);
  void check_one_iteration(set<int> &reduced_dims,
                           map<int, int> &dim2outer_loop);

  bool check_data_dependence(set<int> &reduced_dims,
                             map<int, int> &dim2outer_loop,
                             vector<void *> &outer_loops, set<void *> &refs,
                             void *sg_array, void *sg_scope,
                             vector<size_t> &dims);
  bool is_located_before(void *sg_node1, void *sg_node2);
};
#endif
