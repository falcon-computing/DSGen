#ifndef _MEMORY_BURST_H_INCLUDED_
#define _MEMORY_BURST_H_INCLUDED_
#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

#include "program_analysis.h"
using namespace MarsProgramAnalysis;
class MemoryAccessInfo {
  size_t m_burst_number;
  size_t m_burst_length;

public:
  MemoryAccessInfo(int burst_number, int burst_length)
      : m_burst_number(burst_number), m_burst_length(burst_length) {}
  size_t get_burst_number() const { return m_burst_number; }
  size_t get_burst_length() const { return m_burst_length; }
};
class MemoryBurst {
  CSageCodeGen &m_ast;
  void *mTopFunc;
  CInputOptions &mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;
  size_t mSizeofBRAM;
  size_t m_current_total_usage;
  size_t mSingleBufferSize;
  size_t mLiftingThreshold;
  double mLiftingRatio;
  bool mNaive_tag;
  bool mMemcpy_inline;
  bool mAltera_flow;
  bool m_report_out_of_resource;
  enum effort mEffort;
  size_t m_length_threshold;
  size_t m_wide_bus_length_lower_threshold;
  size_t m_partition_factor_upper_threshold;
  bool m_lift_fine_grained_cache;
  std::set<void *> mInsertedGlobal;
  // key: <var, loop> -> basicblock
  std::map<std::pair<void *, void *>, void *> mMemcpyBlockBefore;
  std::map<std::pair<void *, void *>, void *> mMemcpyBlockAfter;
  std::map<void *, void *> mArray2TopArg;
  std::set<void *> m_assert_header_file;
  std::map<void *, set<void *>> m_cached_scope;
  enum ACCESS_FLAG {
    NO_ACCESS = 0,
    READ_FLAG = 1,
    WRITE_FLAG = 2,
    READ_WRITE_FLAG = 3
  };

  std::map<void *, std::map<void *, std::map<void *, void *>>>
      mParameter2CallAndOffset;
  std::map<void *, int> mParameter2Offset;

public:
  MemoryBurst(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &options,
              bool report)
      : m_ast(codegen), mTopFunc(pTopFunc), mOptions(options),
        mNaive_tag(false), mMemcpy_inline(false), mAltera_flow(false),
        m_report_out_of_resource(!report), mEffort(STANDARD),
        m_length_threshold(1024), m_wide_bus_length_lower_threshold(10240),
        m_partition_factor_upper_threshold(256),
        m_lift_fine_grained_cache(true) {
    init();
  }

  bool run();

  void init();

  void clean_tiling_tag_and_cache();

  bool auto_cache_for_coalescing();

public:
  enum BURST_DIAGNOSIS_INFO {
    NO_NEED_BURST = 0,
    BURST_APPLIED = 1,
    READ_WRITE_SYNC = 2,
    NON_SIMPLE_FORM = 3,
    NON_EXACT_ACCESS = 4,
    NEGATIVE_OFFSET = 5,
    NEGATIVE_BURST_LENGTH = 6,
    UNSUPPORTED_REFERENCE = 7,
    UNDETERMINED_LEN = 8,
    RESOURCE_OUT_OF_BOUND = 9,
    INFINITE_RANGE = 10
  };

  void cg_check_range_availability(void *sg_loop, void *sg_array,
                                   int element_size, CMarsRangeExpr &mr_merged,
                                   CMarsExpression &me_start,
                                   CMarsExpression &me_length, int &is_applied,
                                   int64_t &buff_size, int &read_write,
                                   bool report);

private:
  bool insert_burst(CMirNode *bNode, CMirNode *fNode, int node_num,
                    int parallel_tag, int parent_pipeline_t);
  bool burst_anal(CMirNode *bNode, CMirNode *fNode, void *array,
                  const vector<void *> &bAccess, int64_t &buffer_size,
                  int &element_size, void *&burst_scope, bool &intra_reuse,
                  int parent_pipeline_t, int offset = 0);
  int get_granularity(CMirNode *bNode, const vector<void *> &bAccess,
                      void *array, int &burst_level, int parent_pipeline_t);
  int get_location(CMirNode *bNode, const vector<void *> &bAccess, void *array,
                   int &gran, int burst_level, int element_size, int offset,
                   bool &intra_reuse);
  bool build_memcpy(CMirNode *bNode, CMirNode *fNode,
                    const vector<void *> &bAccess, void *array,
                    int64_t buffer_size, void *burst_scope, int node_num,
                    int buffer_count, int element_size);
  void insert_flatten(CMirNode *bNode, string tag_str);

  bool check_declaration(void *kernel_decl, string arr_name);

  void add_header(void *sg_node);

  bool is_partial_access(void *access, void *scope);

  bool checkDataDependence(CMirNode *scope, CMirNode *lnode, bool group,
                           void *array);
  void *create_local_buffer(CMirNode *lnode, CMirNode *fNode, void *arr,
                            int node_num, void *burst_scope, int buffer_count,
                            int64_t buffer_size);

  bool check_memory_resource(void *array, void *scope, int64_t buffer_size,
                             int element_size);

  bool check_resource_limit(void *array, int64_t orig_buffer_size,
                            int element_size);

  bool check_single_resource_limit(void *array, int64_t buffer_size,
                                   int element_size);

  bool check_whole_resource_limit(int64_t buffer_size, int element_size);

  bool check_lifting_resource_limit(void *array, void *scope,
                                    int64_t last_buffer_size,
                                    int64_t new_buffer_size, int element_size);

  bool check_lifting_efficiency(MemoryAccessInfo last_burst_info,
                                MemoryAccessInfo new_burst_info,
                                void *last_scope, void *scope);

  void *get_insert_loop(CMirNode *bNode, int burst_location);

  void reportUnboundAccess(void *array, CMarsRangeExpr &range);

  void reportPossibleNegativeAccessLength(void *scope, void *array,
                                          CMarsExpression &len);

  void reportDataDependence(CMirNode *bNode, void *array);

  void reportOutofResourceLimit(void *array, void *scope,
                                int64_t size_in_bytes);

  void reportOutofSingleResourceLimit(void *array, void *scope,
                                      int64_t size_in_bytes);

  void reportMessage(CMirNode *bNode, void *array, int64_t buffer_size,
                     int element_bytes);

  void cg_reportMessage(void *sg_arry, void *sg_loop, int64_t buffer_size,
                        int element_bytes, string kind);

  void reportPossibleNegativeAccessStart(CMarsExpression &me_start,
                                         void *sg_array, void *sg_scope);

  bool isUnsupported(void *ref);
  bool isUnsupportedAndReport(void *ref);
  bool is_cg_trace_up(int burst_diagnosis_res);

protected:
  void cg_get_loops_in_topology_order(vector<void *> &cg_scopes);
  int cg_check_reference_availability(void *sg_loop, void *sg_array,
                                      CMarsExpression &me_start);
  int cg_check_single_reference_availability(void *sg_loop, void *sg_array);
  bool cg_transform_burst(void *sg_loop, void *sg_array,
                          CMarsExpression &me_merged_start,
                          CMarsExpression &me_merged_length,
                          CMarsExpression &me_write_start,
                          CMarsExpression &me_write_length, int read_only,
                          void *&buf_decl, void *&memcpy_r, void *&memcpy_w,
                          int64_t &buffer_size, bool check_constant_burst_len);
  int cg_transform_refs_in_scope(void *sg_array, void *offset, void *insert_pos,
                                 void *sg_array_replace, void *scope,
                                 int check_only, vector<void *> &vec_offset);
  void cg_lifting_burst(void *sg_loop, CMarsExpression &me_start,
                        CMarsExpression &me_length, void *&buf_decl,
                        void *&memcpy_r, void *&memcpy_w, int read_only);
  void cg_get_merged_access_range(void *sg_loop, void *sg_array,
                                  CMarsRangeExpr &mr_merged,
                                  CMarsRangeExpr &mr_write, int &read_only,
                                  int64_t *access_size);

  bool process_coarse_grained_top();
  bool process_fine_grained_top();

  void cg_get_options_for_lifting(void *curr_scope, void *curr_array,
                                  vector<void *> &vec_upper_scope,
                                  vector<void *> &vec_array_in_upper_scope,
                                  bool mars_ir_v2 = false);

  void cg_get_cached_options(void *scope);

  int is_cg_burst_available(void *lift_array, void *lift_scope,
                            int element_size, int64_t &buff_size,
                            int64_t *access_size, bool exact_check = false,
                            bool report = false);

  int cg_check_delinearize_availability(void *sg_scope, void *sg_array,
                                        CMarsExpression &me_start);

  bool cg_memory_burst_transform(void *scope_to_lift, void *array_to_lift,
                                 bool check_constant_burst_len);

  bool process_coarse_grained_top_v2();

  bool process_all_scopes(vector<void *> &scopes);

  bool process_one_node(CMarsNode *node);

  bool check_identical_range_in_multiple_nodes(void *port, void *scope,
                                               bool report);

  bool check_identical_indices_within_single_node(void *port, void *scope,
                                                  bool report);

  bool check_coarse_grained_write_only(void *port, void *scope);

  void update_mars_ir(void *port, void *scope);

  void cg_transform_burst_for_parallel(void *sg_scope, void *sg_array,
                                       CMarsExpression &me_start,
                                       CMarsExpression &me_length,
                                       int read_write, void *&buf_decl,
                                       void *&memcpy_r, void *&memcpy_w,
                                       int64_t &buff_size, int para_size,
                                       int64_t lb, bool continuous);

  bool cg_memory_burst_transform_for_parallel(void *scope, void *array);

  bool cg_memory_burst_transform_for_data_reuse(void *scope, void *array);

  int is_cg_parallel_available(void *array, void *scope, std::string &msg);

  int cg_get_access_size(void *array, void *scope);

  void cg_transform_burst_for_data_reuse(
      void *sg_scope, void *sg_array, CMarsExpression &me_start,
      CMarsExpression &me_length, int read_write, void *&buf_decl,
      void *&memcpy_r, void *&memcpy_w, int64_t &buff_size,
      vector<void *> &vec_loops, vector<size_t> &vec_lens,
      vector<void *> &vec_lbs, void *insert_pos);

  bool is_burst_candidate(void *var_init, void *func, void *pos, bool force_on);

  void *get_top_arg(void *var_init);

  void *update_position(void *insert_pos);

  void *create_offset_expr(CMarsExpression &start_index, void *&burst_len,
                           bool &is_non_negative_len,
                           bool &is_non_negative_start, int64_t &buffer_size,
                           bool refine_len, bool final_expr, void *pos);

  size_t get_single_threshold(void *array, int element_size);

  void detect_user_burst();

  bool detect_user_for_loop_burst(void *ref, void *pntr);

  void refine_burst_start_and_length(
      CMarsExpression &me_length, CMarsExpression &me_start, void *insert_pos,
      void *sg_scope, void *sg_array, bool refine_len, int64_t &buff_size,
      void *&burst_len, void *&burst_start_expr, bool &is_non_negative_len,
      bool &is_non_negative_start);

  void *createTempVariableWithBoundAssertion(void *func_decl, void *sg_scope,
                                             void *initializer, string var_name,
                                             int64_t up_bound,
                                             bool is_non_negative_len,
                                             void *&assert_stmt);

  // YX: handle negative offset during burst generation
  void add_offset_later(void *decl, void *param, void *call, void *offset_exp);

  void add_offset();

  void update_array_index(void *param, void *scope, void *offset);

  bool check_auto_caching_for_coalescing(void *curr_array, void *curr_scope);

  int get_access_number(void *array, void *sg_scope, set<void *> &visited);

  void fg_get_loops_in_topology_order(vector<void *> &fg_scopes);
};
static const std::string VIVADO_str("HLS");
#endif
