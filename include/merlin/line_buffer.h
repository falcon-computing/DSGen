#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LINE_BUFFER_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LINE_BUFFER_H_
#include <map>
#include <vector>

#include "ir_types.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "mars_message.h"

#define NOREUSE_WIND 1

int array_linearize_top(CSageCodeGen *codegen, void *pTopFunc,
                        const CInputOptions &options);

class LineBuffer {
 private:
  CSageCodeGen *m_ast;
  void *mTopFunc;
  CInputOptions mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;

  size_t mSingleBufferSize;  //  option from memory burst
  size_t mWindowSize;
  bool mAltera_flow;
  bool mXilinx_flow;

  map<void *, int64_t> loop_ub_cache;
  map<void *, int64_t> loop_lb_cache;
  map<void *, map<void *, size_t>> access_ub;
  map<void *, map<void *, size_t>> access_lb;

 public:
  LineBuffer(CSageCodeGen *codegen, void *pTopFunc,
             const CInputOptions &options)
      : m_ast(codegen), mTopFunc(pTopFunc), mOptions(options),
        mAltera_flow(false), mXilinx_flow(false) {
    init();
  }
  void init();
  bool run();
  //  top function
  void line_buffer_processing();
  //  Check wether if the accesses are stencil
  int check_stencil(
      CMirNode *bNode, void *arr_init, std::vector<void *> *stencil_loops,
      std::vector<size_t> *cache_wind,
      std::vector<std::vector<size_t>> *reuse_winds,
      std::map<void *, int> *reuse_pntr2wind,
      std::map<void *, std::vector<int>> *base_distance,
      std::vector<int> *base_cache_address, int is_delinear,
      std::map<void *, std::vector<MarsProgramAnalysis::CMarsExpression>>
          *mapIndex2Delinear,
      std::vector<size_t> *delinear_arr_size,
      std::vector<size_t> *new_arr_size);
  //  Create the line buffers and sliding windows
  //  Replace the original accesses
  //  Update the line buffers and sliding windows
  void code_transform(CMirNode *bNode, void *arr_init,
                      const std::vector<void *> &stencil_loops,
                      const std::vector<size_t> &cache_wind,
                      const std::vector<std::vector<size_t>> &reuse_winds,
                      const std::map<void *, int> &reuse_pntr2wind,
                      std::map<void *, std::vector<int>> *base_distance,
                      const std::vector<int> &base_cache_address,
                      int is_delinear,
                      const std::vector<size_t> &dim_split_steps,
                      const std::vector<size_t> &new_arr_size);
  //  Check if the interface variable is delinearizable
  int check_delinearize_availability(
      void *sg_scope, void *sg_array, std::vector<size_t> *dim_split_steps,
      std::map<void *, std::vector<MarsProgramAnalysis::CMarsExpression>>
          *mapIndex2Delinear,
      std::vector<size_t> *delinear_arr_size);
  //  Get the delinearze info
  void update_array_size_after_delinearize(void *sg_init,
                                           std::vector<size_t> *size,
                                           void **if_pragma);
  void generate_line_buf_refs(std::vector<void *> line_buffers,
                              std::vector<void *> refs, void *op, void *iter,
                              void *location, bool is_read);

 protected:
  //  Remove the pragmas
  void postprocessing();
  void generate_linearize_ref(std::vector<void *> *index_vector,
                              const std::vector<size_t> &dim_split_steps);
  void cache_loop_tripcount(const std::vector<void *> &stencil_loops);
  // Create line buffer variable declarations
  void create_line_buffers(std::vector<void *> *line_buffers, void *arr_init,
                           const std::vector<size_t> &cache_wind,
                           void *slide_scope, void *outside_loop);
  // Create sliding window variable declarations
  void create_sliding_windows(std::vector<void *> *vec_winds,
                              void **tmp_buf_decl, void *arr_init,
                              const std::vector<vector<size_t>> &reuse_winds,
                              const std::vector<void *> &stencil_loops,
                              void *slide_scope, void *outside_loop);
  void move_stencil_in_condition(void *arr_init,
                                 const std::vector<void *> &stencil_loops,
                                 void *slide_scope);
};
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LINE_BUFFER_H_
