#include "ir_types.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "mars_message.h"

int array_linearize_top(CSageCodeGen &codegen, void *pTopFunc,
                        CInputOptions &options);

class LineBuffer {
private:
  CSageCodeGen &m_ast;
  void *mTopFunc;
  CInputOptions &mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;

  size_t mSingleBufferSize; // option from memory burst
  size_t mWindowSize;
  bool mAltera_flow;
  bool mXilinx_flow;

public:
  LineBuffer(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &options)
      : m_ast(codegen), mTopFunc(pTopFunc), mOptions(options),
        mAltera_flow(false), mXilinx_flow(false) {
    init();
  }
  void init();
  bool run();
  // top function
  void line_buffer_processing();
  // Check wether if the accesses are stencil
  int check_stencil(
      CMirNode *bNode, void *arr_init, vector<void *> &stencil_loops,
      vector<size_t> &cache_wind, vector<vector<size_t>> &reuse_winds,
      map<void *, int> &reuse_pntr2wind, map<void *, vector<int>> &base_const,
      map<void *, vector<int>> &base_distance, vector<int> &base_cache_address,
      int is_delinear, map<void *, vector<CMarsExpression>> mapIndex2Delinear,
      vector<size_t> &delinear_arr_size, vector<size_t> &new_arr_size,
      map<void *, size_t> &access_ub, map<void *, size_t> &access_lb);
  // Create the line buffers and sliding windows
  // Replace the original accesses
  // Update the line buffers and sliding windows
  void code_transform(
      CMirNode *bNode, void *arr_init, vector<void *> stencil_loops,
      vector<size_t> &cache_wind, vector<vector<size_t>> &reuse_winds,
      map<void *, int> reuse_pntr2wind, map<void *, vector<int>> base_const,
      map<void *, vector<int>> base_distance, vector<int> base_cache_address,
      int is_delinear, vector<size_t> dim_split_steps,
      vector<size_t> new_arr_size, map<void *, size_t> &access_ub,
      map<void *, size_t> &access_lb);
  // Remove the pragmas
  void postprocessing();
  // Check if the interface variable is delinearizable
  int check_delinearize_availability(
      void *sg_scope, void *sg_array, vector<size_t> &dim_split_steps,
      map<void *, vector<CMarsExpression>> &mapIndex2Delinear,
      vector<size_t> &delinear_arr_size);
  // Get the delinearze info
  void update_array_size_after_delinearize(void *sg_init, vector<size_t> &size,
                                           void *&if_pragma);
  void generate_line_buf_refs(vector<void *> line_buffers, vector<void *> refs,
                              void *op, void *iter, void *location,
                              bool is_read);
};
