#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_BITWIDTH_OPT_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_BITWIDTH_OPT_H_
#include <vector>
#include <map>
#include <string>

#include "rose.h"

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

struct MemcpyPort {
  void *array;
  void *pntr;
  std::vector<void *> index;
  int pointer;
  MemcpyPort() {
    array = nullptr;
    pntr = nullptr;
    pointer = 0;
  }
  std::string print() {
    std::string res = (pointer ? "pointer" : "array");
    res += " with " + my_itoa(index.size()) + " dimension";
    return res;
  }
  MemcpyPort(const MemcpyPort &port)
      : array(port.array), pntr(port.pntr), index(port.index),
        pointer(port.pointer) {}
};

struct MemcpyInfo {
  std::vector<MemcpyPort> port;
  void *length_exp;
  void *base_type;
  //  read from external memory to local buffer
  bool read;
  int bitwidth;
  int opt_bitwidth;
  int dimension;
  int opt_dimension;
  bool unaligned;
  int aligned;
  std::vector<size_t> dimension_size;
  MemcpyInfo() {
    port.resize(2);
    length_exp = nullptr;
    read = false;
    dimension = -1;
    opt_dimension = -1;
    unaligned = false;
    aligned = 0;
    bitwidth = 0;
    opt_bitwidth = 0;
    base_type = nullptr;
  }
  MemcpyInfo(const MemcpyInfo &info)
      : port(info.port), length_exp(info.length_exp), base_type(info.base_type),
        read(info.read), bitwidth(info.bitwidth),
        opt_bitwidth(info.opt_bitwidth), dimension(info.dimension),
        opt_dimension(info.opt_dimension), unaligned(info.unaligned),
        aligned(info.aligned), dimension_size(info.dimension_size) {}
};

struct SingleAssignInfo {
  bool read;
  bool write;
  std::vector<void *> index;
  void *arg;
  int opt_bitwidth;
  int bitwidth;
  SingleAssignInfo() {
    read = false;
    write = false;
    arg = nullptr;
    opt_bitwidth = 0;
    bitwidth = 0;
  }
  SingleAssignInfo(const SingleAssignInfo &info)
      : read(info.read), write(info.write), index(info.index), arg(info.arg),
        opt_bitwidth(info.opt_bitwidth), bitwidth(info.bitwidth) {}
};

struct CallInfo {
  //  a postion of a argument of  a callEE function
  int pos;
  //  a variable ref of a argument of a callER function
  void *ref;
};

struct ArgumentInfo {
  bool any_burst_access;
  bool short_burst_access;
  bool all_support_access;
  bool suboptimal_coalescing;
  bool need_changed;
  void *base_type;
  int bitwidth;
  int opt_bitwidth;
  bool specified_bitwidth;
  std::map<void *, std::vector<CallInfo>> calls2ref;
  std::map<void *, MemcpyInfo> memcpy;
  std::map<void *, SingleAssignInfo> single_assign;
  ArgumentInfo() {
    any_burst_access = false;
    short_burst_access = false;
    all_support_access = true;
    suboptimal_coalescing = false;
    need_changed = false;
    base_type = nullptr;
    bitwidth = 0;
    opt_bitwidth = 0;
    specified_bitwidth = false;
  }
  ArgumentInfo(const ArgumentInfo &info)
      : any_burst_access(info.any_burst_access),
        short_burst_access(info.short_burst_access),
        all_support_access(info.all_support_access),
        suboptimal_coalescing(info.suboptimal_coalescing),
        need_changed(info.need_changed), base_type(info.base_type),
        bitwidth(info.bitwidth), opt_bitwidth(info.opt_bitwidth),
        specified_bitwidth(info.specified_bitwidth), calls2ref(info.calls2ref),
        memcpy(info.memcpy), single_assign(info.single_assign) {}
};

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_BITWIDTH_OPT_H_
