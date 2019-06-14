#pragma once

#include "rose.h"

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "mars_ir_v2.h"
#include "mars_ir.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

using namespace std;
using namespace MarsProgramAnalysis;

typedef std::vector<SgStatement *> stmt_table_vec;

struct arr_var {
  SgInitializedName *var;
  int appear;
  int first_appear;
  int last_appear;
  bool is_port;
  bool is_pntr;
  bool is_local;
  bool do_init[2]; // 2 tags:do_init[0]-pre initialization;do_init[1]-post
                   // processing
  int is_orig;
  int w_type;
  int self_w_type;
  int buf_depth;
  map<int, int> node_appear;
  map<int, int> stage_appear;
  arr_var() {
    var = nullptr;
    appear = 0;
    first_appear = 0;
    last_appear = 0;
    is_port = false;
    is_pntr = false;
    is_local = false;
    do_init[0] = do_init[1] = false;
    is_orig = false;
    w_type = 0;
    self_w_type = 0;
    buf_depth = 0;
  }
};

typedef std::map<string, arr_var> var_table_map;
enum wr_type { NA = 0, RaW = 1, WaW = 2, WaR = 3, RaR = 4 };
class CGraphNode : public stmt_table_vec {
public:
  void *ref;
  int stage;
  int type; // 0-for statement; 1-function call; 2-basicblock; 3-if; 4-while;
            // 5-varible declare; 6-others
  map<int, enum wr_type> dep_map; // 1- read after write; 2-write after write;
                                  // 3-write after read; 4-read after read
  map<int, map<void *, enum wr_type>> port_dep_map;
  vector<SgExpression *> write_ref;
  vector<SgExpression *> read_ref;
  var_table_map port;
  bool cg_bus;
  CGraphNode() {
    ref = nullptr;
    stage = 0;
    type = 0;
    cg_bus = false;
  }
};

typedef std::map<int, CGraphNode> node_table_map; // Table of iterative
                                                  // variables

void CoarsePipeline(CSageCodeGen &codegen, void *pTopFunc,
                    CInputOptions &options);
bool generate_pipeline(CSageCodeGen &codegen, void *pTopFunc, CMarsIr *mars_ir,
                       CMarsIrV2 *mars_ir_v2, CMirNode *bNode, CMirNode *fNode,
                       string node_num_str, bool &fine_function);
bool build_graph(CSageCodeGen &codgen, CMirNode *bNode,
                 node_table_map &node_table, bool &fine_function,
                 vector<SgVariableDeclaration *> &var_declare_vec);

bool dependency_analysis(CSageCodeGen &codegen, void *pTopFunc,
                         CMarsIr *mars_ir, CMirNode *bNode, CMirNode *fNode,
                         set<void *> var_init_new_set,
                         node_table_map &node_table, var_table_map &var_table);

int GetStmtDependence(CSageCodeGen &codegen, CMirNode *bNode, SgStatement *st1,
                      SgStatement *st2, CMarsDepDistSet &dist_set);
void print_table(node_table_map node_table);

bool GetNodeDependence(CSageCodeGen &codegen, CMirNode *lnode,
                       node_table_map &node_table, CGraphNode &write_node,
                       CGraphNode &read_node, set<void *> var_init_new_set,
                       int num_w, int num_r);

bool stage_assignment(CSageCodeGen &codegen, CMirNode *node,
                      node_table_map &node_table, int &stage_sum,
                      var_table_map &var_table, set<void *> var_init_new_set);

bool channel_analysis(CSageCodeGen &codegen, CMirNode *bNode, CMirNode *fNode,
                      node_table_map &node_table, var_table_map &var_table,
                      int stage_sum, set<void *> var_init_new_set,
                      bool &cg_level_2);

bool code_generation(CSageCodeGen &codegen, CMarsIr *mars_ir,
                     CMarsIrV2 *mars_ir_v2, CMirNode *bNode, CMirNode *fNode,
                     node_table_map &node_table, int stage_sum,
                     string node_num_str, var_table_map &var_table,
                     vector<SgVariableDeclaration *> &var_declare_vec,
                     set<void *> var_init_new_set, bool &cg_level_2);

bool parse_ref(CSageCodeGen &codegen, void *pTopFunc, CMarsIr *mars_ir,
               SgStatement *stmt, CGraphNode &graph_node, CMirNode *bNode,
               CMirNode *fNode, var_table_map &var_table, int node_i);
void parse_read_ref(CSageCodeGen &codegen, CGraphNode &graph_node,
                    vector<SgExpression *> &read_ref, CMirNode *fNode);
void parse_write_ref(CSageCodeGen &codegen, CGraphNode &graph_node,
                     vector<SgExpression *> &write_ref, CMirNode *fNode);
bool parse_ref_normal(CSageCodeGen &codegen, void *pTopFunc, CMarsIr *mars_ir,
                      SgNode *stmt, CGraphNode &graph_node, CMirNode *bNode,
                      CMirNode *fNode, var_table_map &var_table, int node_i);
bool parse_ref_expr(CSageCodeGen &codegen, void *pTopFunc, CMarsIr *mars_ir,
                    SgNode *stmt, CGraphNode &graph_node, CMirNode *fNode,
                    var_table_map &var_table, int node_i);
bool parse_ref_func(CSageCodeGen &codegen, void *pTopFunc, CMarsIr *mars_ir,
                    SgFunctionCallExp *sg_call, CGraphNode &graph_node,
                    CMirNode *bNode, CMirNode *fNode, var_table_map &var_table,
                    int node_i);
int trace_ref_in_func(CSageCodeGen &codegen, SgFunctionCallExp *sg_call,
                      int arg_i);
int trace_ref_in_stmt(CSageCodeGen &codegen, SgStatement *stmt,
                      SgInitializedName *sg_name);

bool isCurrentLoopIterator(CSageCodeGen &codegen, SgInitializedName *arr_var,
                           SgExpression *ref,
                           SgFunctionDeclaration *kernel_decl);

////////// Range analysis

bool test_range_legality(CSageCodeGen &codegen, CMirNode *bNode,
                         node_table_map node_table, void *sg_scope_w,
                         void *sg_scope_r, void *sg_array, int num_w, int num_r,
                         std::string &msg, set<void *> var_init_new_set);

bool check_range_flatten(CSageCodeGen &codegen, void *sg_loop,
                         node_table_map node_table,
                         CMarsRangeExpr &flatten_mr_r,
                         CMarsRangeExpr &flatten_mr_w, void *sg_array,
                         int num_w, int num_r, std::string &msg);

bool check_range_multiple(CSageCodeGen &codegen, void *sg_loop,
                          node_table_map node_table,
                          vector<CMarsRangeExpr> &vec_mr_r,
                          vector<CMarsRangeExpr> &vec_mr_w, void *sg_array,
                          int num_w, int num_r, std::string &msg);

bool union_access_range(CSageCodeGen &codegen, node_table_map node_table,
                        CMarsRangeExpr &mr_merged, bool &merge_tag,
                        void *sg_array, int num_w, int num_r, std::string &msg);

bool union_access_range_multiple(CSageCodeGen &codegen,
                                 node_table_map node_table,
                                 CMarsRangeExpr &mr_merged, bool &merge_tag,
                                 void *sg_array, int num_w, int num_r,
                                 std::string &msg, int dim);

///////////////

bool preprocessing(CSageCodeGen &codegen, CMarsIr *mars_ir, CMirNode *bNode,
                   vector<SgVariableDeclaration *> &var_declare_vec,
                   vector<void *> &var_decl_old, vector<void *> &var_decl_new,
                   set<void *> &var_init_new_set, bool &changed);

bool frame_nodes(CSageCodeGen &codegen, node_table_map node_table,
                 CMirNode *bNode, bool &changed);

bool test_feedback_dependency(CSageCodeGen &codegen, void *sg_array,
                              void *sg_loop, int stage, enum wr_type dep_type,
                              set<void *> var_init_new_set);

void reportFeedback(CSageCodeGen &codegen, CMirNode *lnode,
                    vector<void *> arr_report, int i, int j);
void reportUnsupportStatement(void *stmt, CMirNode *lnode,
                              CSageCodeGen &codegen);

void reportBusConflict(CMirNode *lnode, CSageCodeGen &codegen);
void reportNonCanonical(void *stmt, CMirNode *lnode, CSageCodeGen &codegen);
void reportLegalityCheck(CSageCodeGen &codegen, CMirNode *lnode, void *sg_array,
                         const string &reason, bool local);
void reportMultiDeclare(void *stmt, CMirNode *lnode, CSageCodeGen &codegen);

void reportOneStatement(CMirNode *lnode, CSageCodeGen &codegen);

// static const std::string VIVADO_str ("HLS");

void pragma_propagate(CSageCodeGen &codegen, CMarsIr *mars_ir,
                      map<void *, vector<void *>> existing_table,
                      map<void *, vector<void *>> gen_table,
                      set<void *> var_init_new_set);

bool label_processing(CSageCodeGen &codegen, void *pTopFunc, CMirNode *bNode);

void postprocessing_fail(CSageCodeGen &codegen, CMarsIr *mars_ir,
                         CMirNode *bNode, vector<void *> &var_decl_old,
                         vector<void *> &var_decl_new);

void postprocessing_success(CSageCodeGen &codegen, CMarsIr *mars_ir,
                            CMirNode *bNode, vector<void *> &var_decl_old,
                            vector<void *> &var_decl_new);

void remove_pragmas(CSageCodeGen &codegen, CMirNode *new_node);
