#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>

#include "rose.h"
#include "bsuNode.h"
#include "codegen.h"
#include "mars_ir.h"
#include "mars_opt.h"
#include "program_analysis.h"

#include "stream_ir.h"

class MemoryPartition {
 protected:
  CSageCodeGen *m_ast;
  void *mTopFunc;
  CInputOptions mOptions;
  CMarsIr *mMars_ir;

  // Mode options
  bool mAltera_flow;
  bool mXilinx_flow;
  int m_reshape_threshold;
  bool m_dual_option;
  std::string m_transform_level;

  // Final decisions
  std::map<void *, std::map<int, int>> m_partitions;
  std::map<CMirNode *, bool> m_actions;  // Xilinx use only
  std::set<void *> m_registers;          // Intel use only
  std::map<void *, std::map<std::string, std::string>>
      m_attributes;  // Intel use only

  // Temp results and caching data
  std::map<CMirNode *, std::map<void *, std::map<int, int>>> tmp_factors;
  std::map<void *, std::map<int, int>> prior_factors;
  std::map<void *, std::set<int>> adjust_factors;
  std::map<void *, int> cache_var_dim;
  std::map<void *, std::vector<size_t>> cache_var_size;
  std::map<void *, void *> cache_var_base_type;

  void dual_port_adjust();

 public:
  MemoryPartition(CSageCodeGen *codegen, void *pTopFunc,
                  const CInputOptions &options, CMarsIr *mars_ir)
      : m_ast(codegen), mTopFunc(pTopFunc), mOptions(options),
        mMars_ir(mars_ir), mAltera_flow(false), mXilinx_flow(false) {
    init();
  }
  void init();
  bool run();

  std::map<CMirNode *, bool> get_node_actions() { return m_actions; }
  std::map<void *, std::map<int, int>> get_partitions() { return m_partitions; }
  void set_registers(std::set<void *> registers) {
    for (auto reg : registers)
      m_registers.insert(reg);
  }

  void partition_analysis();
  bool
  partition_analysis_node(CMirNode *fNode, CMirNode *bNode,
                          std::map<void *, std::map<int, int>> *var2partition);
  void partition_merge();

  // Xilinx flow
  void insert_partition_xilinx();
  void partition_transform_xilinx();

  // Intel flow
  void insert_partition_intel();
  void partition_evaluate_intel();
  void partition_transform_intel();

  void reportPartitionMessage();
  void delete_existing_pragma();

  bool reorganize_factor(
      std::map<void *, std::vector<std::map<int, int>>> *v_factors);
  void index_transform(CMirNode *bNode);
  void array_index_transform();
};

// Xilinx flow
void partition_pragma_gen_xilinx(CSageCodeGen *codegen, void *arr_init, int dim,
                                 int factor);
void enumerate_point_space(const std::vector<int> &dim_size,
                           std::vector<std::vector<int>> *point_set);
int index_switch_transform(CSageCodeGen *codegen, void *input_array,
                           void *input_loop, std::vector<int> switch_factor_in,
                           bool read_only);
int choose_factor(std::vector<size_t> arr_size,
                  const std::vector<std::map<int, int>> &o_factors,
                  std::map<int, int> *m_factors, std::map<int, int> *p_factors);
void copy_factors(const std::vector<std::map<int, int>> &original,
                  std::vector<std::map<int, int>> *target,
                  const std::vector<size_t> &arr_size_org,
                  const std::vector<size_t> &arr_size_target, int partial_dim);
void copy_factors(const std::map<int, int> &original,
                  std::map<int, int> *target);
bool check_action(int f1, int f2);

void print_partition(CSageCodeGen *codegen,
                     const std::map<void *, std::map<int, int>> &partitions);
bool factor_extract(CSageCodeGen *codegen, CMarsIr *mars_ir, void *sg_pragma,
                    map<int, int> *hls_factors);
// User message
void reportInvalidFactor(CSageCodeGen *codegen, CMirNode *node,
                         std::string str_factor);
void reportInvalidFactor1(CSageCodeGen *codegen, CMirNode *node, int old_factor,
                          int range);
void reportSuboptimalFGPIP(CSageCodeGen *codegen, CMirNode *node, void *arr);
void reportSuboptimalMemIntel(CSageCodeGen *codegen, void *arr_init);
