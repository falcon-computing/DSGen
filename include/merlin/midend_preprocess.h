#ifndef _MIDEND_PREPROCESS_H_INCLUDED_
#define _MIDEND_PREPROCESS_H_INCLUDED_
#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"
class MidEndPreProcess {
  CSageCodeGen &m_ast;
  void *mTopFunc;
  CInputOptions &mOptions;
  CMarsIr mMars_ir;
  CMarsIrV2 mMars_ir_v2;
  bool mNaive_tag;
  bool mAuto_fgpip_tag;
  bool mAuto_fgpar_tag;
  bool mAltera_flow;
  bool mXilinx_flow;
  int mThreshold_fgpar;
  enum effort mEffort;

public:
  MidEndPreProcess(CSageCodeGen &codegen, void *pTopFunc,
                   CInputOptions &options)
      : m_ast(codegen), mTopFunc(pTopFunc), mOptions(options),
        mNaive_tag(false), mAuto_fgpip_tag(false), mAuto_fgpar_tag(false),
        mAltera_flow(false), mXilinx_flow(false), mEffort(MEDIUM) {
    init();
  }

  bool run();

private:
  bool insertFineGrainPipeline(CMirNode *lnode);
  // bool insertFineGrainPipelineWhile(CMirNode *lnode);
  // Added by Yuxin, 16-Feb-2016
  bool autoFineGrainParallel();
  bool autoFineGrainPipeline();
  bool autoFineGrainParallel(CMirNode *lnode);
  bool autoFineGrainParallel(CMirNode *lnode, int &limit);
  void init();
  void build_mars_ir(bool check_pragma, bool pragma_in_loop);
  void clear_mars_ir();

  bool check_partial_paralleled_loop();
  bool remove_parallel_pipeline_off_pragma();
  void collect_existing_hls_fg();

  bool check_auto_value_in_pragma();
  vector<void *> m_existing_hls_fg;
};
#endif
