#pragma once

#include <map>
#include <string>
#include <vector>

#include "rose.h"

using namespace std;

const int const_index = -2;
const int max_value = INT_MAX;
const int no_value = 0;

typedef std::map<int, int> index_expr_map; // FIXME:  (variable, coefficient)
typedef std::map<int, std::string>
    symbol_table_map; // FIXME: Table of iterative variables
typedef std::map<int, int>
    range_table_map; // FIXME: Table of iteration space, correct only when the
                     // itartion domain is affine

/*cmost pragma*/
static const std::string ACCEL_PRAGMA("ACCEL"); // current version
static const std::string CMOST_PRAGMA("CMOST"); // previous version
static const std::string TLDM_PRAGMA("TLDM");   // previous previous version
static const std::string CMOST_TASK("TASK");    // previous previous version
static const std::string CMOST_KERNEL("KERNEL");
static const std::string CMOST_die("DIE");
static const std::string CMOST_TASK_BEGIN("TASK_BEGIN");
static const std::string CMOST_TASK_END("TASK_END");
static const std::string CMOST_TASK_WAIT("TASKWAIT");
static const std::string CMOST_sync_SUBPRAGMA("ASYNC");
static const std::string CMOST_PARALLEL("PARALLEL");
static const std::string CMOST_coalescing("COALESCING");
static const std::string CMOST_coalescing_width("COALESCING_WIDTH");
// static const std::string CMOST_LOOP_TILING ("LOOP_TILING");
static const std::string CMOST_INLINE("INLINE");
static const std::string CMOST_LOOP_TILE("TILE");
static const std::string CMOST_LOOP_TILED_TAG("TILED_TAG");
static const std::string CMOST_cache("CACHE");
static const std::string CMOST_LOOP_TILING("TILING");
static const std::string CMOST_loop_tiling_option("TILING_OPTION");
static const std::string CMOST_REDUCTION("REDUCTION");
static const std::string CMOST_LINE_BUFFER("LINE_BUFFER");
static const std::string CMOST_INTERFACE("INTERFACE");
static const std::string CMOST_complete("COMPLETE");
static const std::string CMOST_parallel_option("PARALLEL_OPTION");
static const std::string CMOST_parallel_factor("FACTOR");
static const std::string CMOST_parallel_multi_level_factor("PARALLEL_FACTOR");
static const std::string CMOST_parallel("PARALLEL");
static const std::string CMOST_PIPELINE("PIPELINE");
static const std::string CMOST_PIPE_PARALLEL("PIPELINE_PARALLEL");
static const std::string CMOST_pipe_parallel_factor("FACTOR");
static const std::string CMOST_BURST("BURST");
static const std::string CMOST_bus_bitwidth("BUS_BITWIDTH");
static const std::string CMOST_FLATTEN("FLATTEN");
static const std::string CMOST_flatten_option("FLATTEN");
static const std::string CMOST_factor("FACTOR");
static const std::string CMOST_prior("PRIORITY");
static const std::string CMOST_ii("II");
static const std::string CMOST_variable("VARIABLE");
static const std::string CMOST_new_variable("NEW_VARIABLE");
static const std::string CMOST_name("NAME");
static const std::string CMOST_api_name("API_NAME");
static const std::string CMOST_dim("DIM");
static const std::string CMOST_type("TYPE");
static const std::string CMOST_data_type("DATA_TYPE");
static const std::string CMOST_depth("DEPTH");
static const std::string CMOST_max_depth("MAX_DEPTH");
static const std::string CMOST_exclusive_option("EXCLUSIVE_OPTION");
static const std::string CMOST_exclusive("EXCLUSIVE");
static const std::string CMOST_fifo("FIFO");
static const std::string CMOST_bus("AXI");
static const std::string CMOST_interface("INTERFACE");
static const std::string CMOST_attribute("ATTRIBUTE");
static const std::string CMOST_systolic("SYSTOLIC");
static const std::string CMOST_data_reuse("DATA_REUSE");
static const std::string CMOST_write_only("WRITE_ONLY");
static const std::string CMOST_scope("SCOPE");
static const std::string CMOST_scope_type("SCOPE_TYPE");
static const std::string CMOST_io("IO");
static const std::string CMOST_SKIP_SYNC("SKIP_SYNC");
static const std::string CMOST_SKIP("SKIP");
static const std::string CMOST_OFF("OFF");
static const std::string CMOST_ON("ON");
static const std::string CMOST_force("FORCE");
static const std::string CMOST_off_option("OFF_OPTION");
static const std::string CMOST_SPAWN("SPAWN");
static const std::string CMOST_continue("CONTINUE");
static const std::string CMOST_auto("AUTO");
static const std::string HLS_PRAGMA("HLS"); // vivado pragma
static const std::string AP_PRAGMA("AP");   // previous vivado pragma
static const std::string HLS_PARTITION("ARRAY_PARTITION");
static const std::string HLS_TRIPCOUNT("LOOP_TRIPCOUNT");
static const std::string HLS_MAX("max");
static const std::string HLS_MIN("min");
static const std::string HLS_PIPELINE("PIPELINE");
static const std::string HLS_PIPELINE_II("II");
static const std::string HLS_UNROLL("UNROLL");
static const std::string CMOST_copy_in("COPY_IN");
static const std::string CMOST_copy_out("COPY_OUT");
static const std::string CMOST_burst_off("BURST_OFF");
static const std::string CMOST_memory_burst("MEMORY_BURST");
static const std::string CMOST_burst_max_size("MAX_BURST_SIZE");
static const std::string CMOST_false_dep("FALSE_DEPENDENCE");
static const std::string CMOST_loop_flatten("LOOP_FLATTEN");
static const std::string CMOST_stream_prefetch("STREAM_PREFETCH");
static const std::string CMOST_scheme("SCHEME");
static const std::string CMOST_block("block");
static const std::string CMOST_cyclic("cyclic");
static const std::string CMOST_reduction_scheme("REDUCTION_SCHEME");
static const string PRAGMA_TYPE_NODE = "STREAM_NODE"; // sub kernel
static const string PRAGMA_TYPE_PORT =
    "STREAM_PORT"; // single direction operation FIFO<->Array
static const string PRAGMA_TYPE_CHANNEL = "STREAM_CHANNEL"; // FIFO
static const string PRAGMA_TYPE_BUFFER =
    "STREAM_BUFFER"; // connect two ports via Array
static const string PRAGMA_NODE_ATTRIBUTE = "NODE";
static const string PRAGMA_PORT_ATTRIBUTE = "PORT";
static const string PRAGMA_SPAWN_ATTRIBUTE = "SPAWN";
static const string PRAGMA_PREFETCH_ATTRIBUTE = "PREFETCH";
static const string PRAGMA_READ_NODE_ATTRIBUTE = "READ_NODE";
static const string PRAGMA_WRITE_NODE_ATTRIBUTE = "WRITE_NODE";
static const string PRAGMA_VARIABLE_ATTRIBUTE = "VARIABLE";
static const string PRAGMA_PARALLEL_FACTOR_ATTRIBUTE = "PARALLEL_FACTOR";
static const string PRAGMA_PARALLEL_ITERATOR_ATTRIBUTE = "PARALLEL_ITERATOR";
static const string PRAGMA_DIM_NUM_ATTRIBUTE = "DIM_NUM";
static const string PRAGMA_ACCESS_TYPE_ATTRIBUTE = "ACCESS_TYPE";
static const string PRAGMA_CHANNEL_DEPTH_ATTRIBUTE = "CHANNEL_DEPTH";
static const string PRAGMA_CHANNEL_DEPTH_ATTRIBUTE_DEFAULT = "32";
