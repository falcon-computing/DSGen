
#ifndef _MARS_IR_V2_H_INCLUDED_
#define _MARS_IR_V2_H_INCLUDED_
#include "bsuNode_v2.h"

enum effort { LOW = 0, STANDARD = 1, MEDIUM = 2, HIGH = 3 };
enum tool_type {
  XILINX_SDX = 0,
  INTEL_SDK = 1,
};
class CMarsIrV2 {
  set<void *> mKernelTops; // top level user kernel function declaration
  map<void *, void *>
      mKernel2Pragma; // the task pragma associated with the kernel if any
  set<void *>
      mAllKernelFuncs; // all the kernel functions and kernel subfunctions
  set<void *> mAlteraChannels;
  set<void *> mSharedPorts;
  vector<CMarsNode *> mIrNodes; // MarsIR nodes

  vector<CMarsEdge *> mIrEdgesList; // MarsIR nodes
  map<CMarsNode *, map<CMarsNode *, map<void *, CMarsEdge *>>>
      mIrEdges; // MarsIR edges

  map<void *, pair<int, int>>
      mKernel2IrNodeRange; // MarsIR nodes range for each kernel
  map<void *, vector<vector<int>>> mLoop2Orders; // map from loop to its order
  map<void *, map<vector<int>, void *>>
      mOrder2Loop; // map from order to its loop

  map<void *, void *> mCall2Func;        // intermediate variable
  map<void *, set<void *>> mFunc2Caller; // intermediate variable
  map<void *, set<void *>> mFunc2Callee; // intermediate variable
  map<void *, bool> mFunc2Loop;          // intermediate variable
  map<void *, bool> mFlattenLoop;        // intermediate variable
  map<void *, CMarsLoop *> mLoopInfo;    // map from loop to its info
  set<void *> mPragmas;
  CMarsAST_IF *m_ast;

  bool mValid;      // check whether the progam can be distributed
                    // according to for-loop
                    // structures
  bool mValidCheck; // whether to do the above check

  bool mErrorOut;

  map<void *, vector<size_t>> mDimensionPragma; // the allocated size, constant
  map<void *, vector<void *>>
      mDepthPragma; // the size for date transfer, 0: equal to dimension,
                    // sg_init: the variable
  map<void *, string> mContinueFlag; // the size for date transfer, 0: equal to
                                     // dimension, sg_init: the variable
  map<void *, bool> mBurstFlag;     // true: infer burst; false: not infer burst
  map<void *, bool> mWriteOnlyFlag; // true: do not infer read burst from host
                                    // even if the write is non-exact
  map<void *, bool> mCoalescingFlag; // true: not apply delinearization; false:
                                     // apply delinearization
  map<void *, bool>
      mFalseDepFlag; // true: no data dependence; false: data dependence
  map<void *, size_t> mBurstMaxSize; // max threshold for each port
  set<void *> mNoSynchronizedPorts;

public:
  CMarsIrV2(CSageCodeGen *ast = nullptr)
      : m_ast(ast), mValid(true), mValidCheck(false), mErrorOut(false) {}
  void build_mars_ir(CSageCodeGen &codegen, void *pTopFunc,
                     bool check_valid = false, bool build_node = false);
  bool is_kernel(void *func_decl);

  bool is_valid() { return mValid; }

public:
  bool is_kernel_port(void *sg_init_name);

  bool is_tiled_loop(void *scope);

  bool is_shared_port(void *sg_init_name);

  bool is_altera_channel(void *port);

  set<void *> get_all_shared_ports() { return mSharedPorts; }

  bool iswithininnermostloop(void *stmt);

  bool skip_synchronization(void *port) {
    return mNoSynchronizedPorts.count(port) > 0;
  }

  bool is_undistributed_statement(void *stmt);

  bool get_burst_flag(void *arg);

  bool get_write_only_flag(void *arg);

  bool get_coalescing_flag(void *arg);

  bool get_false_dep_flag(void *arg);

  size_t get_burst_max_size(void *arg);

protected:
  bool trace_to_bus(void *target_arr, map<void *, bool> &res, void *pos);

public:
  bool any_trace_is_bus(void *sg_init_name, void *pos);

protected:
  bool every_trace_is_bus(void *sg_init_name, void *pos);

public:
  CMarsNode *get_node(size_t i);
  CMarsAST_IF *get_ast() { return m_ast; }
  vector<CMarsNode *> &get_all_nodes() { return mIrNodes; }
  vector<CMarsEdge *> &get_all_edges() { return mIrEdgesList; }
  set<void *> get_shared_ports(CMarsNode *node);
  ~CMarsIrV2();

  size_t size() const { return mIrNodes.size(); }

  bool isErrorOut() const { return mErrorOut; }

  void clear();

  void update_ports_for_all_nodes();

  void update_all_edges();
  void update_dependence_for_one_edge();
  void update_dependence_for_all_edges();
  void update_ssst_flag_for_all_edges();
  int get_number_of_all_sub_nodes(void *kernel_top,
                                  const vector<int> &order_prefix);
  void get_order_of_sub_nodes(void *kernel_top, const vector<int> &order_prefix,
                              vector<int> &orders);

  vector<vector<int>> get_order_for_loop(void *loop);

  void *get_loop_for_order(void *kernel_top, const vector<int> &order);

  pair<int, int> get_node_range(void *kernel_top) {
    pair<int, int> res(numeric_limits<int>::max(), numeric_limits<int>::max());
    map<void *, pair<int, int>>::iterator ret =
        mKernel2IrNodeRange.find(kernel_top);
    if (ret == mKernel2IrNodeRange.end())
      return res;
    return ret->second;
  }

  vector<CMarsNode *> get_nodes_with_common_loop(/*void *kernel_top, */
                                                 void *loop);

  CMarsEdge *get_edge_by_node(CMarsNode *node0, CMarsNode *node1, void *port) {
    return GetEdgeByNode(node0, node1, port);
  }

  void get_top_loop_info(map<void *, map<void *, bool>> &top_loop_2_ports,
                         map<void *, vector<void *>> &top_loops);

  vector<void *> get_top_kernels() {
    return vector<void *>(mKernelTops.begin(), mKernelTops.end());
  }
  void *get_kernel_task_pragma(void *kernel) {
    if (mKernel2Pragma.find(kernel) == mKernel2Pragma.end())
      return nullptr;
    else
      return mKernel2Pragma[kernel];
  }
  void *TraceUpToOneKernelByPos(void *pos);

  vector<void *> get_common_loops(vector<CMarsNode *> &nodes);

  CMarsLoop *get_loop_info(void *loop);

  int get_node_idx(CMarsNode *node);

  void remove_all_pragmas();

  void convert_false_dep_pragmas(tool_type tt);

  bool remove_writeonly_shared_local_ports();

  // ZP: 20160910
  vector<size_t> get_array_dimension(void *sg_name, int &cont_flag,
                                     void *pos = nullptr);
  vector<size_t> get_array_depth(void *sg_name, void *pos = nullptr);
  string get_depth_string(vector<size_t> size, int cont_flag);

  void build_node_info(const set<void *> &unrolled_loops,
                       const vector<void *> &topo_funcs);

private:
  bool isEmptyStmtSequence(vector<void *> &stmts);

  void createNode(vector<void *> &stmts, vector<void *> &loops,
                  map<void *, vector<pair<void *, bool>>> &loop2conds,
                  vector<pair<void *, bool>> &conds, vector<int> &orders,
                  t_func_call_path &call_path, void *kernel);

  bool checkSubLoops(void *sg_scope, const set<void *> &unrolled_loops);

  bool get_topological_order_funcs(vector<void *> &res);

  bool dfs_traverse(void *curr, set<void *> &visited, set<void *> &updated,
                    vector<void *> &res);

  void buildNode(void *body, vector<void *> &loops,
                 map<void *, vector<pair<void *, bool>>> &loop2cond,
                 vector<pair<void *, bool>> &conds, vector<int> &orders,
                 t_func_call_path &call_path, void *kernel,
                 const set<void *> &unrolled_loops);

  CMarsEdge *GetEdgeByNode(CMarsNode *node0, CMarsNode *node1, void *port) {
    if (mIrEdges.end() == mIrEdges.find(node0)) {
      map<CMarsNode *, map<void *, CMarsEdge *>> empty;
      mIrEdges.insert(
          pair<CMarsNode *, map<CMarsNode *, map<void *, CMarsEdge *>>>(node0,
                                                                        empty));
    }
    if (mIrEdges[node0].end() == mIrEdges[node0].find(node1)) {
      map<void *, CMarsEdge *> empty;
      mIrEdges[node0].insert(
          pair<CMarsNode *, map<void *, CMarsEdge *>>(node1, empty));
    }
    if (mIrEdges[node0][node1].end() == mIrEdges[node0][node1].find(port)) {
      CMarsEdge *edge = new CMarsEdge(m_ast, node0, node1, port);
      mIrEdges[node0][node1].insert(pair<void *, CMarsEdge *>(port, edge));
    }

    return mIrEdges[node0][node1][port];
  }

  void collect_altera_channels(const vector<void *> &all_funcs);

  bool check_identical_indices(set<void *> &refs);

  void init_variable_info_pragma();
  size_t get_dimension_by_pragma(void *sg_init, int i, int &cont_flag);
  size_t get_depth_by_pragma(void *sg_init, int i, void *pos);

  bool is_flatten_loop(void *loop, const set<void *> &unroll_loops);

public:
  void *get_unique_wrapper_pragma(void *sg_init);
  void *get_unique_interface_pragma(void *sg_init);

  string get_pragma_attribute(void *sg_pragma, string key,
                              bool allow_hls = false);
  string get_all_pragma_attribute(void *sg_pragma, string key, bool &is_cmost);
  string get_pragma_command(void *sg_pragma, bool allow_hls = false);
  void set_pragma_attribute(void *&sg_pragma, string key, string value,
                            bool third_party = false);

  // s_fileter: the field after ACCEL, e.g. "interface", "wrapper", etc.
  void *get_pragma_by_variable(string s_var, void *scope, string s_filter = "");

  void get_function_bound(void *func, set<void *> &bound_set);

  bool check_identical_indices_without_simple_type(set<void *> &refs);

  vector<tuple<CMarsNode *, bool, int>> get_coarse_grained_def(CMarsNode *node,
                                                               void *port);

  bool has_definite_coarse_grained_def(CMarsNode *node, void *poart);

  void reset_kernel_node_analysis() { m_valid_kernel_node_analysis = false; }

  bool is_used_by_kernel(void *sg_node);

private:
  bool m_valid_kernel_node_analysis;
  set<void *> m_var_used_by_kernel;
  set<void *> m_func_used_by_kernel;
  set<void *> m_type_used_by_kernel;
  void collect_all_kernel_nodes();
  void collect_all_kernel_nodes(void *scope, set<void *> &visited);
};

#endif
