#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_HISTORY_MESSAGE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_HISTORY_MESSAGE_H_
#include <string>

// Memory burst and coalescing
static const char BURST[] = "burst";                // memory_burst
static const char DISPLAY_NAME[] = "display_name";  // memory burst display name
static const char IS_READ[] = "is_read";            // is read burst
static const char IS_WRITE[] = "is_write";          // is write brust
static const char BURST_LENGTH[] = "burst_length";  // burst length
static const char MEMORY_COALESCING[] =
    "memory_coalescing";  // memory_coalescing
static const char MEMORY_COALESCING_BITWIDTH[] =
    "memory_coalescing_bitwidth";  // memory_coalescing bitwidth
// Fine-grained Optimization
static const char FGPAR[] =
    "fine_grained_parallel";  // fine grained parallel turn on/off
static const char FGPAR_COMPLETE[] =
    "fine_grained_parallel_complete";  // fine grained parallel complete
static const char FGPAR_FACTOR[] =
    "fine_grained_parallel_factor";  // fine grained parallel factor
static const char FGPIP[] =
    "fine_grained_pipeline";  // fine grained pipeline turn on/off
static const char FGPIP_II[] =
    "fine_grained_pipeline_II";  // fine grained pipeline II
static const char FALSE_DEPENDENCE[] =
    "fine_grained_opt_false_dependence";            // false dependency
static const char TILING_LOOP[] = "tiling_loop";    // tiling loop
static const char FLATTEN_LOOP[] = "flatten_loop";  // flattened loop
// Others
static const char SRC_TRIP_COUNT[] = "src_trip_count";  // source code tripcount
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_HISTORY_MESSAGE_H_
