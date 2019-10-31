#pragma once

#include "PolyModel.h"
#include <scoplib/scop.h>

scoplib_scop_p PolyModel_to_Scoplib(PolyMatrix &domain, PolyMatrix &schedule,
                                    void *sg_scop_root, void *sg_scop_body,
                                    vector<void *> iter_symbols);
