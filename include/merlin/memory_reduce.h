#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_MEMORY_REDUCE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_MEMORY_REDUCE_H_
#include <tuple>
#include <map>
#include <set>
#include <vector>
#include "codegen.h"
#include "mars_ir_v2.h"
class MemoryReduce {
  CSageCodeGen *mCodegen;
  CMarsIrV2 mMars_ir;
  enum reduce_type {
    ONE_ITERATION = 0,
    NO_INTRA_DEP = 1,
  };

 public:
  MemoryReduce(CSageCodeGen *codegen, void *pTopFunc,
               const CInputOptions &option);
  bool run();

  bool process_one_node(CMarsNode *node, enum reduce_type rt);

  bool optimize_local_memory_layout(CMarsNode *node);

  void check_const_indices(
      std::set<int> *reduced_dims,
      const std::vector<std::tuple<void *, int64_t, int64_t>> &dim2vars);
  void check_one_iteration(std::set<int> *reduced_dims,
                           const std::map<int, int> &dim2outer_loop);

  bool check_data_dependence(std::set<int> *reduced_dims,
                             const std::map<int, int> &dim2outer_loop,
                             const std::vector<void *> &outer_loops,
                             const std::set<void *> &refs, void *sg_array,
                             void *sg_scope, const std::vector<size_t> &dims);
  bool is_located_before(void *one_node, void *other_node);
};
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_MEMORY_REDUCE_H_
