
#ifndef _PLTF_ANNOTATION_H_INCLUDED_
#define _PLTF_ANNOTATION_H_INCLUDED_

#include "cmdline_parser.h"
#include "xml_parser.h"

typedef struct CXMLAnn_imp_option__ {
  string name;
  string task;
  string generator;
  string report;
  string module_impl_dir;
  string evaluator;
  string src_dir;
  map<string, string> metrics;
  int regenerate; // 0 for output, 1 for input, 2 for iterator
  string component_name;
} CXMLAnn_imp_option;

class CXMLAnn_PL_Component : public CXMLNodeAnnotationBase {
public:
  // enum
  //{
  //	IOTYPE_OUTPUT = 0,
  //	IOTYPE_INPUT,
  //	IOTYPE_ITERATOR
  //};

public:
  virtual string GetClassString() { return "CXMLAnn_PL_Component"; }

  void SetParam(string key, string val) { m_mapParams[key] = val; }
  string GetParam(string key) {
    if (m_mapParams.end() == m_mapParams.find(key))
      return "";
    else
      return m_mapParams[key];
  }

  void AppendImp(CXMLAnn_imp_option imp) { m_vecImps.push_back(imp); }
  int GetImpNum() { return m_vecImps.size(); }
  CXMLAnn_imp_option &GetImp(int i) { return m_vecImps[i]; }

  string print();

  void AppendBaseAddr(string port_name, string addr) {
    m_base_addr[port_name] = addr;
  }
  string GetBaseAddr(string port_name) { return m_base_addr[port_name]; }

protected:
  // Scalar parameters
  map<string, string> m_mapParams;

  map<string, string> m_base_addr;

  vector<CXMLAnn_imp_option> m_vecImps;
};

int mark_pltf_annotation(CXMLNode *pTLDMRoot);
int print_pltf_annotation(CXMLNode *pTLDMRoot);
int mark_pltf_annotation_release_resource();
CXMLAnn_PL_Component *get_platform_component_ann_by_name(string sName);
CXMLAnn_imp_option &get_platform_impl_ann_by_name(string sName);

#endif //_PLTF_ANNOTATION_H_INCLUDED_
