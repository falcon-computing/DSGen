#ifndef LOOP_NODE_H
#define LOOP_NODE_H

#include <list>
#include <map>

#include "analPragmas.h"
#include "codegen.h"
#include "ir_types.h"
#include "program_analysis.h"
#include "rose.h"

class CMirNode;

typedef std::list<SgNode *> sgnode_list;
typedef std::map<string, int> array_table_list;

/* YUXIN:
 * When a CMirNode is a function body, the array_table contains all the array
 * information declared in the function body or as function parameters.
 * When a CMirNode is a loop body, the array_table only contains the array
 * access information within the loop body scope.
 */
class CMirNode {
  // std::set<string> kernel;
  std::set<void *> kernel_decl;
  std::map<void *, vector<string>> kernel2interface;
  std::set<void *> reduction_decl;
  void init();
  std::set<void *> arr_vars;

public:
  CMirNode();
  CMirNode(SgBasicBlock *ref, bool is_function);
  ~CMirNode();

  // main structure
  SgBasicBlock *ref; // A pointer to the nodea, default as SgBasicBlock*
  bool is_affine; // FIXME: specify whether the iteration domain is affine, not
                  // implemented yet
  bool is_curr_affine; // Current loop level is affine or not
  string iterator_string;
  string iterator_range_string;
  string order_vec_string;
  // access_table_list access_table;
  //  access_table_list full_access_table;
  array_table_list full_array_list;
  map<void *, vector<void *>> full_access_table_v2;

  sgnode_list child_table;
  sgnode_list pragma_table;

  // additional structure
  bool is_fine_grain; // consider the impact of pragma, e.g.
                      // there are sub loops which all have been unrolled
  bool is_innermost;  // do not consider the impact of pragma
  bool is_pipeline;
  bool is_parallel;
  bool is_function;
  bool is_kernel;
  bool is_task;
  bool is_while;
  bool has_loop;
  bool has_loop_without_pragma;
  int arraycount;
  int iterators_num;
  vector<void *> loops;
  string funcname_str;
  string funcname_without_args;
  symbol_table_map
      iterator_table; // stores all names of loop iterator variables
  range_table_map range_lower_table; // the lower range of the iterators
  range_table_map range_upper_table; // the upper range of the iterators
  range_table_map upper_err_table;   // the upper range of the iterators
  set<SgInitializedName *> liveIns;
  set<SgInitializedName *> liveOuts;

  vector<CAnalPragma> vec_pragma;

  int has_opt_pragmas();
  int has_pipeline();
  int has_parallel();

  int has_tiling();
  int has_spawn();
  int has_flatten_off();
  int has_reduction();
  int has_line_buffer();

  int is_partial_unroll(int &factor);

  string get_attribute(string attr);

  void remove_attribute(string attr);

  CAnalPragma get_pipeline_pragma();

  CAnalPragma get_parallel_pragma();

  CAnalPragma get_tiling_pragma();

  vector<CAnalPragma> get_all_pragmas() { return vec_pragma; }
  // functionality
  bool order_vector_gen(CSageCodeGen &codegen, void *sg_node_scope,
                        void *sg_node);
  bool analyze_iteration_domain_order(CSageCodeGen &codegen, void *sg_scope_,
                                      void *sg_stmt_, vector<string> &iterators,
                                      vector<int> &loop_indices,
                                      vector<SgExpression *> &lower,
                                      vector<SgExpression *> &upper,
                                      vector<int> &ub_limit_vec);
  bool get_child_task_idx(CSageCodeGen &codegen, void *sg_scope_,
                          void *sg_child_, int &index);
  bool isAffineRange();
  //	bool parse_full_accesses(CSageCodeGen &codegen, int dim, void* arr_init,
  //				vector<map<void*, int>> &index_expr_full, int &mod_size);

  //	bool check_relation(CSageCodeGen &codegen, int dim,
  //				void *arr_init,  void * curr_loop, bool &recur_tag);
  void node_liveness_analysis();

  // interface
  SgBasicBlock *get_ref() { return ref; }
  SgForStatement *get_parent_for();
  int get_acc_num() { return arraycount; };
  bool isFineGrain() { return is_fine_grain; };
  bool isFunction() { return is_function; };
  string unparseToString() { return ref->unparseToString(); }
  void print_access();
  void print_child();
  void print_parent();

  // Interface functions
  string get_fun_name(bool with_arg = true) const {
    return with_arg ? funcname_str : funcname_without_args;
  };
#if 0
    void add_one_kernel(string name) {
        kernel.insert(name);
    }
    set<string> get_all_kernel() const {
        return kernel;
    }
#endif
  void add_one_kernel(void *func_decl, const vector<string> &vec_interface) {
    kernel_decl.insert(func_decl);
    kernel2interface[func_decl] = vec_interface;
  }

  set<void *> get_all_kernel_decl() const { return kernel_decl; }
  map<void *, vector<string>> get_all_kernel_interface() const {
    return kernel2interface;
  }
  void add_one_reduction(void *func_decl) { reduction_decl.insert(func_decl); }
  set<void *> get_all_reduction_decl() const { return reduction_decl; }
  bool check_kernel() const { return is_function && is_kernel; }

  void set_kernel() { is_kernel = true; }
  void set_task() { is_task = true; }

  void add_one_child(CMirNode *child);
  void set_parent(CMirNode *parent) {
    parent_node = parent;
    return;
  }
  void add_one_predecessor(CMirNode *caller);
  vector<CMirNode *> get_all_predecessor() { return predecessors; }
  CMirNode *get_child(size_t index = 0);
  vector<CMirNode *> get_all_child(bool recursive = false);
  CMirNode *get_parent() { return parent_node; }

  CMirNode *get_func() { return func_node; }

  void set_func(CMirNode *node) { func_node = node; }

  bool is_complete_unroll(bool check_skip_attribute = false);

  bool is_autoFG();

  void set_full_access_table_v2(CSageCodeGen &codegen, bool recursively = true);
  void parse_sub_func_access(CSageCodeGen &codegen, void *sg_decl, void *sg_arr,
                             void *sg_arr_local);

  void add_pragma(CAnalPragma pragma) {
    vec_pragma.push_back(pragma);
    return;
  }
  void add_pragma(SgNode *pragma) {
    pragma_table.push_back(pragma);
    return;
  }
  vector<CAnalPragma> get_pragma() { return vec_pragma; }
  void clear_pragma() {
    vec_pragma.clear();
    return;
  }

  void get_opt_vars(CSageCodeGen &codegen, string opt_str,
                    vector<void *> &vars);

protected:
  CMirNode *parent_node;
  CMirNode *func_node;
  set<CMirNode *> child_node_set;
  vector<CMirNode *> child_node;
  vector<CMirNode *> predecessors;
  set<CMirNode *> predecessor_set;
  bool ParseLoopBound(SgExpression *bound);
  void getLoopLiveVariables(LivenessAnalysis *liv, SgForStatement *loop,
                            std::set<SgInitializedName *> &liveIns,
                            std::set<SgInitializedName *> &liveOuts);
};

#endif
