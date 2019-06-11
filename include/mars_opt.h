#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "rose.h"
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>

#define DEBUG 0
#define XMD_GEN 0
#define RUNTIME_GEN 1
#define PCIE_GEN 2
#define CPU_GEN 3
#define MERLIN_WRAPPER_PREFIX "__merlinwrapper_"
#define MERLIN_WRITE_BUFFER_PREFIX "__merlin_write_buffer_"
#define MERLIN_READ_BUFFER_PREFIX "__merlin_read_buffer_"
#define MERLIN_EXECUTE_PREFIX "__merlin_execute_"
#define KERNEL_FILE_JSON "kernel_file.json"
#define KERNEL_LIST_JSON "kernel_list.json"
#define TASK_KERNEL_JSON "task_kernel.json"
#define TASK_ATTRIBUTE_JSON "task_attribute.json"
#define MAX_SIZE_THRESHOLD (1LL << 32)
#define MAX_CONST_BYTE 1024 * 1024
#define XILINX_ARG_LIMITATION 256
#define INTEL_ARG_LIMITATION 256
#define WARNING_ARG_LIMITATION 16
#define XILINX_ARG_SCALAR_LIMITATOIN 16
#define INTEL_DEFAULT_REGISTER_THRESHOLD 4096
#define INTEL_MAX_REGISTER_THRESHOLD 8192
#define MERLIN_SMALL_TRIP_COUNT_THRESHOLD 16
// pass_name: MemBurst CoarseOpt FineOpt WideBus Outline Reduction
// level: INFO, WARNING, ERROR
// message: multi-line display message
// number: INFO 1xx, Warning 2xx, Error 3xx
class CSageCodeGen;
void dump_critical_message(string pass_name, string level, string message,
                           int number = -1, int user_display = 0,
                           CSageCodeGen *ast = nullptr, void *node = nullptr);

// To parallelize builds, let's dump a few things in the header:
void GetTLDMInfo_withPointer(void *sg_node, void *pArg);
int io_cfg_gen();
void dep_graph_gen();
int get_map_additionals(CSageCodeGen &codegen, string sPort, void *sg_scope,
                        map<string, string> &mapAdditional);
void *get_surrounding_graph_from_task(void *task_bb, CSageCodeGen &codegen,
                                      string sTaskName, string &graph_name,
                                      string &additional);

int profiling_top(CSageCodeGen &codegen, void *pTopFunc,
                  CInputOptions &options);
int cpu_ref_add_timer(CSageCodeGen &codegen, void *pTopFunc,
                      CInputOptions &options);
int preprocessing(CSageCodeGen &codegen, void *pTopFunc,
                  CInputOptions &options);
int local_array_delinearization(CSageCodeGen &codegen, void *pTopFunc,
                                CInputOptions &options);
int mars_gen_preproc(CSageCodeGen &codegen, void *pTopFunc,
                     CInputOptions &options);
int mars_gen_check_task_interface_format(CSageCodeGen &codegen, void *pTopFunc,
                                         CInputOptions options);
int tldm_extraction_mars_gen(CSageCodeGen &codegen, void *pTopFunc,
                             CInputOptions options);

extern map<void *, vector<int>> mapTask2OrderVec;
extern map<void *, vector<int>> mapGraph2OrderVec;

// added by Hui
struct GraphOrderLT {
  bool operator()(void *graph1, void *graph2) const {
    size_t i;
    assert(mapGraph2OrderVec.count(graph1) > 0 &&
           mapGraph2OrderVec.count(graph2) > 0);
    vector<int> order_vec1 = mapGraph2OrderVec[graph1];
    vector<int> order_vec2 = mapGraph2OrderVec[graph2];

    // debug
    if (DEBUG) {
      cerr << "graph 1:";
      for (i = 0; i < order_vec1.size(); i++)
        cerr << order_vec1[i] << "\t";
      cerr << endl;
      cerr << "graph 2:";
      for (i = 0; i < order_vec2.size(); i++)
        cerr << order_vec2[i] << "\t";
      cerr << endl;
    }

    for (i = 0; i < min(order_vec1.size(), order_vec2.size()); i++) {
      if (order_vec1[i] < order_vec2[i])
        return true;
      if (order_vec1[i] > order_vec2[i])
        return false;
    }
    // cout<<"Error: Graph order vector overlap..."<<endl;
    // assert(0);
    return false;
  }
};

struct TaskOrderLT {
  bool operator()(void *task1, void *task2) const {
    size_t i;
    assert(mapTask2OrderVec.count(task1) > 0 &&
           mapTask2OrderVec.count(task2) > 0);
    vector<int> order_vec1 = mapTask2OrderVec[task1];
    vector<int> order_vec2 = mapTask2OrderVec[task2];
    for (i = 0; i < min(order_vec1.size(), order_vec2.size()); i++) {
      if (order_vec1[i] < order_vec2[i])
        return true;
      if (order_vec1[i] > order_vec2[i])
        return false;
    }
    // cout<<"Error: Task order vector overlap..."<<endl;
    // assert(0);
    return false;
  }
};

extern set<string> dump_array;
extern set<string> graph_names;
extern map<string, string> mapArray2DimStr;
extern map<string, string> mapArray2TypeName;
extern map<void *, set<void *, TaskOrderLT>> execute_seq;
extern set<void *, GraphOrderLT> sorted_execute_seq;
extern vector<string> graph_execute_seq;

extern int g_debug_mode;

#define STR(MSG) #MSG
#define XTR(MSG) STR(MSG)
#define WARNING XTR(WARNING)