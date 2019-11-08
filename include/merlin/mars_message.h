#pragma once
#include "mars_opt.h"
#define MARS_INFO(pass, msg, num) XTR(pass), XTR(INFO), msg, (num) + 100
#define MARS_WARNING(pass, msg, num) XTR(pass), WARNING, msg, (num) + 200
#define MARS_ERROR(pass, msg, num) XTR(pass), XTR(ERROR), msg, (num) + 300
#define MULTIPLE_FUNCITON_CALLS                                                \
  ".\n  Hint: the following statement should contain only one function call"
#define LONG_BURST_HINT                                                        \
  "  Hint: 1. tile large loops into smaller ones to adjust the burst buffer "  \
  "size\n"                                                                     \
  "        2. specify the burst length threshold by adding interface "         \
  "attribute \n"                                                               \
  "        \'max_burst_size=?\'\n"                                             \
  "        3. decrease the single resource threshold by using \n"              \
  "        \'--attribute burst_single_size_threshold=?\'\n"                    \
  "        4. make range analysis accurate by changing \n"                     \
  "         pointer parameter into array parameter in sub function"

#define SHORT_BURST_HINT                                                       \
  "  Hint: 1. tile large loops into smaller ones to adjust the burst buffer "  \
  "size\n"                                                                     \
  "        2. specify the burst length threshold by adding interface "         \
  "attribute \n"                                                               \
  "        \'max_burst_size=?\'\n"                                             \
  "        3. increase the single resource threshold by using \n"              \
  "        \'--attribute burst_single_size_threshold=?\'\n"                    \
  "        4. make range analysis accurate by changing \n"                     \
  "         pointer parameter into array parameter in sub function"
#define INCREASE_TILING_FATOR "  Hint: increase tile factor"
#define DECREASE_TILING_FATOR "  Hint: decrease tile factor"
#define PROCS_INFO(num, msg) MARS_INFO(PROCS, msg, num)
#define PROCS_WARNING(num, msg) MARS_WARNING(PROCS, msg, num)
#define PROCS_ERROR(num, msg) MARS_ERROR(PROCS, msg, num)
#define MDARR_INFO(num, msg) MARS_INFO(MDARR, msg, num)
#define MDARR_WARNING(num, msg) MARS_WARNING(MDARR, msg, num)
#define MDARR_ERROR(num, msg) MARS_ERROR(MDARR, msg, num)
#define BURST_INFO(num, msg) MARS_INFO(BURST, msg, num)
#define BURST_WARNING(num, msg) MARS_WARNING(BURST, msg, num)
#define BURST_ERROR(num, msg) MARS_ERROR(BURST, msg, num)
#define DISAG_INFO(num, msg) MARS_INFO(DISAG, msg, num)
#define DISAG_WARNING(num, msg) MARS_WARNING(DISAG, msg, num)
#define DISAG_ERROR(num, msg) MARS_ERROR(DISAG, msg, num)
#define GLOBL_INFO(num, msg) MARS_INFO(GLOBL, msg, num)
#define GLOBL_WARNING(num, msg) MARS_WARNING(GLOBL, msg, num)
#define GLOBL_ERROR(num, msg) MARS_ERROR(GLOBL, msg, num)
#define FUCIN_INFO(num, msg) MARS_INFO(FUCIN, msg, num)
#define FUCIN_WARNING(num, msg) MARS_WARNING(FUCIN, msg, num)
#define FUCIN_ERROR(num, msg) MARS_ERROR(FUCIN, msg, num)
#define INFTF_ERROR(num, msg) MARS_ERROR(INFTF, msg, num)
#define INFTF_WARNING(num, msg) MARS_WARNING(INFTF, msg, num)
#define PRAGM_ERROR(num, msg) MARS_ERROR(PRAGM, msg, num)
#define KWRAP_INFO(num, msg) MARS_INFO(KWRAP, msg, num)
#define KWRAP_WARNING(num, msg) MARS_WARNING(KWRAP, msg, num)
#define KWRAP_ERROR(num, msg) MARS_ERROR(KWRAP, msg, num)
#define WDBUS_INFO(num, msg) MARS_INFO(WDBUS, msg, num)
#define WDBUS_WARNING(num, msg) MARS_WARNING(WDBUS, msg, num)
#define WDBUS_ERROR(num, msg) MARS_ERROR(WDBUS, msg, num)
#define REDUC_INFO(num, msg) MARS_INFO(REDUC, msg, num)
#define REDUC_WARNING(num, msg) MARS_WARNING(REDUC, msg, num)
#define REDUC_ERROR(num, msg) MARS_ERROR(REDUC, msg, num)
#define LINEBUF_INFO(num, msg) MARS_INFO(LINEBUF, msg, num)
#define LINEBUF_WARNING(num, msg) MARS_WARNING(LINEBUF, msg, num)
#define LINEBUF_ERROR(num, msg) MARS_ERROR(LINEBUF, msg, num)
#define CGPIP_INFO(num, msg) MARS_INFO(CGPIP, msg, num)
#define CGPIP_WARNING(num, msg) MARS_WARNING(CGPIP, msg, num)
#define CGPIP_ERROR(num, msg) MARS_ERROR(CGPIP, msg, num)
#define CGPAR_INFO(num, msg) MARS_INFO(CGPAR, msg, num)
#define CGPAR_WARNING(num, msg) MARS_WARNING(CGPAR, msg, num)
#define CGPAR_ERROR(num, msg) MARS_ERROR(CGPAR, msg, num)
#define LTILE_INFO(num, msg) MARS_INFO(LTILE, msg, num)
#define LTILE_WARNING(num, msg) MARS_WARNING(LTILE, msg, num)
#define LTILE_ERROR(num, msg) MARS_ERROR(LTILE, msg, num)
#define SEPAR_ERROR(num, msg) MARS_ERROR(SEPAR, msg, num)
#define SEPAR_WARNING(num, msg) MARS_WARNING(SEPAR, msg, num)
#define AST_IF_INFO(num, msg) MARS_INFO(AST_IF, msg, num)
#define AST_IF_WARNING(num, msg) MARS_WARNING(AST_IF, msg, num)
#define AST_IF_ERROR(num, msg) MARS_ERROR(AST_IF, msg, num)
#define FGPIP_INFO(num, msg) MARS_INFO(FGPIP, msg, num)
#define FGPIP_WARNING(num, msg) MARS_WARNING(FGPIP, msg, num)
#define AUREG_WARNING(num, msg) MARS_WARNING(AUREG, msg, num)
#define FGPIP_ERROR(num, msg) MARS_ERROR(FGPIP, msg, num)
#define MSG_RPT_INFO(num, msg) MARS_INFO(MSG_RPT, msg, num)
#define MSG_RPT_WARNING(num, msg) MARS_WARNING(MSG_RPT, msg, num)
#define MSG_RPT_ERROR(num, msg) MARS_ERROR(MSG_RPT, msg, num)

//  ////////////////////////////////////////////  /
//  All Error out messages (limitations)
//  ////////////////////////////////////////////  /
#define PROCS_ERROR_2(x)                                                       \
  PROCS_ERROR(2, "Found a local array variable " + (x) +                       \
                     " with statically unknown size in the kernel")
#define PROCS_ERROR_3(x)                                                       \
  PROCS_ERROR(3, "Found a variable " + (x) +                                   \
                     " with C++ reference type in the kernel")
#define PROCS_ERROR_4(x)                                                       \
  PROCS_ERROR(4, "Found an class memeber variable " + (x) + "in the kernel")
#define PROCS_ERROR_5(x)                                                       \
  PROCS_ERROR(5, "Missing the header file <assert.h> in the file " + (x))
#define PROCS_ERROR_6 PROCS_ERROR(6, "Cannot support multiple tasks")
#define PROCS_ERROR_7(x)                                                       \
  PROCS_ERROR(7, "Detecting an unsupported design: "                           \
                 "the kernel function and all the functions used in the "      \
                 "kernel must be in the same source file."                     \
                 " The following functions are not in the same file: \n" +     \
                     (x))
#define PROCS_ERROR_8(x)                                                       \
  PROCS_ERROR(8,                                                               \
              "Kernel function declaration cannot be in header files\n" + (x))
#define PROCS_ERROR_9 PROCS_ERROR(9, "Cannot support C/C++ mixed design")
#define PROCS_ERROR_10(x)                                                      \
  PROCS_ERROR(10, "Found a class member function " + (x))
#define PROCS_ERROR_11(x)                                                      \
  PROCS_ERROR(11, "Found a parameter " + (x) +                                 \
                      " whose type is reference in the kernel")
#define PROCS_ERROR_12(x, y)                                                   \
  PROCS_ERROR(                                                                 \
      12, "Cannot find a kernel function call following the task pragma " +    \
              (x) +                                                            \
              ((y) ? MULTIPLE_FUNCITON_CALLS                                   \
                   : ".\n  Hint: the task pragma should be placed right "      \
                     "before its "                                             \
                     "specified kernel function call"))
#define PROCS_ERROR_13(x, y)                                                   \
  PROCS_ERROR(13, "Cannot find the kernel function " + (x) +                   \
                      " specified by the pragma " + (y))
#define PROCS_ERROR_14(x, y)                                                   \
  PROCS_ERROR(14, "Cannot find a kernel function"                              \
                  " following the kernel pragma " +                            \
                      (x) +                                                    \
                      ((y) ? MULTIPLE_FUNCITON_CALLS                           \
                           : ".\n  Hint: the kernel pragma should be placed "  \
                             "right before its "                               \
                             "specified kernel function\n"))
#define PROCS_ERROR_15(x)                                                      \
  PROCS_ERROR(15, "TODO")  //  merged with RPOCS_ERROR_12
#define PROCS_ERROR_16(x)                                                      \
  PROCS_ERROR(16, "Cannot find the function definition of " + (x))
#define PROCS_ERROR_17(x)                                                      \
  PROCS_ERROR(17, "Cannot support recursive function " + (x))
#define PROCS_ERROR_18(x)                                                      \
  PROCS_ERROR(18, "TODO")  //  merged with PROCS_ERROR_14
#define PROCS_ERROR_19                                                         \
  PROCS_ERROR(                                                                 \
      19,                                                                      \
      "Cannot find kernel pragma "                                             \
      "in the program.\n"                                                      \
      " Hint: please add a kernel pragma right before the kernel function")
#define PROCS_ERROR_20(x)                                                      \
  PROCS_ERROR(20, "Found multiple definitions for the function " + (x) +       \
                      "\n Hint: it may be due to including a duplicated "      \
                      "input file in other input file")
#define PROCS_ERROR_21(x, y)                                                   \
  PROCS_ERROR(21, "Found the kernel " + (x) +                                  \
                      " function with unmatched task instance " + (y))
#define PROCS_ERROR_22(x)                                                      \
  PROCS_ERROR(22, "Cannot find the kernel function " + (x))
#define PROCS_ERROR_23(x)                                                      \
  PROCS_ERROR(                                                                 \
      23,                                                                      \
      "Found an invalid task pragma: " + (x) +                                 \
          "\n  Hint: the correct format is \n"                                 \
          "    #pragma ACCEL task name=\"taskname\"\n"                         \
          "      (\'taskname\' can only include digits, characters or \'_\' "  \
          "but cannot start with digits)\n"                                    \
          "    Or \n"                                                          \
          "    #pragma ACCEL task")
#define PROCS_ERROR_24(x)                                                      \
  PROCS_ERROR(24, "Found an invalid kernel pragma: " + (x) +                   \
                      "\n  Hint: the correct format is #pragma ACCEL kernel "  \
                      "name=\"taskname\" \n"                                   \
                      "        Or \n"                                          \
                      "        #pragma ACCEL kernel")
#define PROCS_ERROR_25(x)                                                      \
  PROCS_ERROR(25, "Found an invalid interface pragma: " + (x))
#define PROCS_ERROR_26(x, y)                                                   \
  PROCS_ERROR(26, "Cannot find the reduction function " + (x) +                \
                      " specified by the pragma " + (y))
#define PROCS_ERROR_27(x)                                                      \
  PROCS_ERROR(27,                                                              \
              "Cannot support returning a pointer or reference in function " + \
                  (x))
#define PROCS_ERROR_28(x)                                                      \
  PROCS_ERROR(28, "Cannot support structure type which contains "              \
                  "unsupported types, \ne.g. recursive type, function "        \
                  "pointer, static member at the kernel interface:\n " +       \
                      (x))
#define PROCS_ERROR_29(x)                                                      \
  PROCS_ERROR(29, "Cannot support class type which contains "                  \
                  "unsupported types, \ne.g. recursive type, function "        \
                  "pointer, static member at the kernel interface:\n " +       \
                      (x))
#define PROCS_ERROR_30(x)                                                      \
  PROCS_ERROR(30, "Cannot support union type at the kernel interface: " + (x))
#define PROCS_ERROR_31(x)                                                      \
  PROCS_ERROR(31, "Cannot support enum type at the kernel interface: " + (x))
#define PROCS_ERROR_32(x, y, z)                                                \
  PROCS_ERROR(32, "Cannot support implicit type cast for argument " + (x) +    \
                      " : implicit cast from actual argument type " + (y) +    \
                      " to formal argument type " + (z))
#define PROCS_ERROR_33(x)                                                      \
  PROCS_ERROR(33, "Cannot support function call " + (x) + "\n" +               \
                      " because its callee definition is not included in the " \
                      "input file(s)")
#define PROCS_ERROR_34(x)                                                      \
  PROCS_ERROR(34, "The reduction function is invalid: " + (x) +                \
                      ". Please check Merlin User Guide for the correct "      \
                      "coding style of the reduction function.")
#define PROCS_ERROR_35(x)                                                      \
  PROCS_ERROR(35, "Cannot support old style function declaration "             \
                  "whose parameter type(s) is(are) specified outside the "     \
                  "function parameter list: " +                                \
                      (x))
#define PROCS_ERROR_36(x)                                                      \
  PROCS_ERROR(                                                                 \
      36,                                                                      \
      "Cannot support duplicate type definitions outside header files:\n" +    \
          (x))
#define PROCS_ERROR_37(x)                                                      \
  PROCS_ERROR(                                                                 \
      37, "Cannot support a pointer to interface arrays/pointers: \n" + (x))
#define PROCS_ERROR_38(x)                                                      \
  PROCS_ERROR(38, "Cannot support function pointer call " + (x))
#define PROCS_ERROR_39(x)                                                      \
  PROCS_ERROR(                                                                 \
      39,                                                                      \
      "Cannot support the assignment on interface arrays/pointers: \n" + (x))
#define PROCS_ERROR_40                                                         \
  PROCS_ERROR(40, "Found an empty design")  //  ZP: rephrase it
#define PROCS_ERROR_41(x)                                                      \
  PROCS_ERROR(41, "Cannot support anonymous type on interface: " + (x) +       \
                      "\n  Hint: please change it into named type")
#define PROCS_ERROR_42(x)                                                      \
  PROCS_ERROR(42, "Cannot specify system function " + (x) + " as kernel.")
#define PROCS_ERROR_43(x)                                                      \
  PROCS_ERROR(43, "Cannot support multiple kernel with same task name \"" +    \
                      (x) + "\"")
#define PROCS_ERROR_44(x)                                                      \
  PROCS_ERROR(44, "Cannot support multiple calls on parameter " + (x) +        \
                      " by both local and interface variables\n" +             \
                      "  Hint: please uniquify the function manually")
#define PROCS_ERROR_45(x, y)                                                   \
  PROCS_ERROR(45, "Cannot support kernel " + (x) +                             \
                      " which is called by another kernel " + (y))
#define PROCS_ERROR_46(x)                                                      \
  PROCS_ERROR(                                                                 \
      46, "Cannot support reference type at the kernel interface:\n " + (x))
#define PROCS_ERROR_47(x)                                                      \
  PROCS_ERROR(47, "Cannot support kernel " + (x) + " with name 'main'")
#define PROCS_ERROR_48(x) PROCS_ERROR(48, "Cannot support asm statement " + (x))
#define PROCS_ERROR_51(x)                                                      \
  PROCS_ERROR(51,                                                              \
              "Cannot support " + (x) + " because its name is OpenCL keyword")
#define PROCS_ERROR_52(x)                                                      \
  PROCS_ERROR(                                                                 \
      52, "Cannot support class declaration within a function body: " + (x) +  \
              "\n" + "  Hint: please move it out of function")
#define PROCS_ERROR_53(x)                                                      \
  PROCS_ERROR(53, "Cannot support the kernel interface port: " + (x))
#define PROCS_ERROR_54(x)                                                      \
  PROCS_ERROR(54, "Cannot support unamed kernel interface port: " + (x))
#define PROCS_ERROR_55(x)                                                      \
  PROCS_ERROR(55, "Cannot support the loop with continue in its "              \
                  "init/condition/increment : " +                              \
                      (x))
#define PROCS_ERROR_56(x)                                                      \
  PROCS_ERROR(56, "Cannot support kernel name : " + (x) +                      \
                      ", Please change to another name")
#define PROCS_ERROR_57(x)                                                      \
  PROCS_ERROR(57, "Cannot support kernel name start with \"__merlin\" : " +    \
                      (x) + ", Please change to another name")
#define PROCS_ERROR_58(x)                                                      \
  PROCS_ERROR(58, "Cannot support function call " + (x) +                      \
                      " because of mismatch between formal parameters and "    \
                      "actual arguments")
#define PROCS_ERROR_59(x)                                                      \
  PROCS_ERROR(59, "Cannot support reimplementing system function : " + (x))
#define PROCS_ERROR_60(x)                                                      \
  PROCS_ERROR(60, "Cannot support function " + (x) +                           \
                      " used non-ordinary function call")
#define PROCS_ERROR_61(x)                                                      \
  PROCS_ERROR(61, "Cannot support variable argument operation " + (x))
#define PROCS_ERROR_62(x, y)                                                   \
  PROCS_ERROR(62, "Found an invalid " + (x) + " pragma: " + (y))
#define PROCS_ERROR_63(x, y, z)                                                \
  PROCS_ERROR(63, "Found an invalid " + (x) + " pragma: " + (y) +              \
                      ".\n  Diagnosis information: \n" + (z))
#define PROCS_ERROR_64(x, y)                                                   \
  PROCS_ERROR(64,                                                              \
              "Found interface " + (x) + " and " + (y) + " are pointer alias")
#define PROCS_ERROR_66(x) PROCS_ERROR(66, "Cannot support pointer cast " + (x))
#define PROCS_ERROR_67(x, y)                                                   \
  PROCS_ERROR(67,                                                              \
              "Cannot support implicit function call " + (x) +                 \
                  " declared in " + (y) + "\n" +                               \
                  "Hint: Please explicitly specify the function definition " + \
                  "under the delaration")
#define PROCS_ERROR_68(x)                                                      \
  PROCS_ERROR(68, "Cannot support more than one task pragma.")
#define PROCS_ERROR_69(x)                                                      \
  PROCS_ERROR(69, "Find name " + (x) + " which conflict with OpenCL key word.")
#define PROCS_ERROR_70(x, y, z)                                                \
  PROCS_ERROR(70, "Argument number for kernel " + (x) + " is " + (y) +         \
                      ".\n"                                                    \
                      "The limitation is " +                                   \
                      (z) + ".")
#define PROCS_ERROR_71(x, y, z)                                                \
  PROCS_ERROR(71, "Scalar argument number for kernel " + (x) + " is " + (y) +  \
                      ".\n"                                                    \
                      "The limitation is " +                                   \
                      (z) + ".")
#define PROCS_ERROR_72(x)                                                      \
  PROCS_ERROR(72, "Cannot support expression " + (x) + " with long long type.")
#define PROCS_ERROR_73(x)                                                      \
  PROCS_ERROR(73, "Pointer comparison detected " + (x) +                       \
                        "\n"                                                   \
                        "Vendor tool may not fully support this operation.")
#define PROCS_ERROR_74(x)                                                      \
  PROCS_ERROR(74, "Found a variable with multi-dimensional pointer"            \
                    " which is not assigned to any address spaces " +          \
                        (x) +                                                  \
                        "."                                                    \
                        " Vendor tool can not support this type"               \
                        " in accleration.")
#define MDARR_ERROR_2(x)                                                       \
  MDARR_ERROR(2, "The dimensions of interface array "                          \
                 "variables do not match in multiple "                         \
                 "function calls or alias: \n" +                               \
                     (x))
#define MDARR_ERROR_3(x)                                                       \
  MDARR_ERROR(                                                                 \
      3,                                                                       \
      "Found overflowed dimension size of the interface variable : \n" + (x))
#define MDARR_ERROR_4(x)                                                       \
  MDARR_ERROR(4, "Cannot support complex pointer reinterpretation on "         \
                 "interface array/pointer : \n" +                              \
                     (x))
#define MDARR_ERROR_5(var_info, type_info, pragma_info)                        \
  MDARR_ERROR(                                                                 \
      5, "Cannot determine some of the dimension sizes of the interface "      \
         "variable " +                                                         \
             (var_info) + " of type \'" + (type_info) +                        \
             "\'.\n  Hint: please " +                                          \
             "specify the missing dimension sizes in the following pragma "    \
             "and add it " +                                                   \
             "to your kernel:\n" + (pragma_info))
#define MDARR_ERROR_6(x)                                                       \
  MDARR_ERROR(6, "Cannot support complex access on multiple dimension "        \
                 "array/pointer : \n" +                                        \
                     (x))
#define BURST_ERROR_1                                                          \
  BURST_ERROR(1, "Cannot pass function uniquifying checker\n"                  \
                 "  Hint: Please turn off memory burst or uniquify function "  \
                 "calls\n")  // ZP: rephrase it
#define INFTF_ERROR_6(x, y)                                                    \
  INFTF_ERROR(                                                                 \
      6, "Found multiple interface pragmas for port " + (x) + ":\n" + (y) +    \
             "  Hint: please try to merge them into one single pragma")
#define SEPAR_ERROR_1(x)                                                       \
  SEPAR_ERROR(1, "Cannot support recursive function: " + (x))
#define SEPAR_ERROR_2(x) SEPAR_ERROR(2, "Cannot support recursive type: " + (x))
#define SEPAR_ERROR_3(x)                                                       \
  SEPAR_ERROR(3, "Cannot support function pointer: " + (x))
#define SEPAR_ERROR_4(x)                                                       \
  SEPAR_ERROR(4, "Cannot support template function: " + (x))
#define SEPAR_ERROR_5(x) SEPAR_ERROR(5, "Cannot support template class: " + (x))
#define SEPAR_ERROR_6(x) SEPAR_ERROR(6, "Cannot support function call:\n" + (x))
#define SEPAR_ERROR_7(x) SEPAR_ERROR(7, "Cannot support system type: " + (x))
#define SEPAR_ERROR_8(x)                                                       \
  SEPAR_ERROR(8, "Cannot support inlined or undefined type: " + (x))
#define SEPAR_ERROR_9(x)                                                       \
  SEPAR_ERROR(9, "Cannot support global variable " + (x) +                     \
                     " with recursive assignment")
#define SEPAR_ERROR_10(x)                                                      \
  SEPAR_ERROR(10, "Cannot support anonymous type: " + (x) +                    \
                      "\n  Hint: please add a name for the above type")
#define SEPAR_ERROR_11(var, var_pos, x)                                        \
  SEPAR_ERROR(11, "Cannot support global or static variable '" + (var) +       \
                      "' " + (var_pos) + "\n  shared in multiple kernels " +   \
                      (x))
#define SEPAR_ERROR_12(x) SEPAR_ERROR(12, x)
#define SEPAR_ERROR_13(x) SEPAR_ERROR(13, x)
#define SEPAR_ERROR_14(x) SEPAR_ERROR(14, x)
#define SEPAR_ERROR_15(x) SEPAR_ERROR(15, x)
#define SEPAR_ERROR_16(x) SEPAR_ERROR(16, x)
#define SEPAR_ERROR_17(x) SEPAR_ERROR(17, x)
#define SEPAR_ERROR_18(x) SEPAR_ERROR(18, x)
#define SEPAR_ERROR_19(x) SEPAR_ERROR(19, x)
#define SEPAR_WARNING_1(x)                                                     \
  SEPAR_WARNING(                                                               \
      1, "Cannot support nested struct definitions: " + (x) +                  \
             "\n  Hint: please replace nested struct with ordinary struct")
#define SEPAR_WARNING_2(x, y)                                                  \
  SEPAR_WARNING(2, "Stream channel '" + (x) +                                  \
                       "' lacks the 'channel_depth' attribute. Set as " +      \
                       (y) + " by default.\n")
#define SEPAR_WARNING_3(x)                                                     \
  SEPAR_WARNING(                                                               \
      3, "Stream channel '" + (x) +                                            \
             "' is read-only in the task, which may lead to deadlock.\n")
#define SEPAR_WARNING_4(x)                                                     \
  SEPAR_WARNING(                                                               \
      4, "Stream channel '" + (x) +                                            \
             "' is write-only in the task, which may lead to deadlock.\n")

#define CLGEN_INFO(num, msg) MARS_INFO(CLGEN, msg, num)
#define CLGEN_WARNING(num, msg) MARS_WARNING(CLGEN, msg, num)
#define CLGEN_ERROR(num, msg) MARS_ERROR(CLGEN, msg, num)
#define CLGEN_ERROR_1(x)                                                       \
  CLGEN_ERROR(1, "Cannot support function declaration in the header file.\n" + \
                     (x))
#define CLGEN_ERROR_2(x)                                                       \
  CLGEN_ERROR(                                                                 \
      2, "Finding a pointer variable " + (x) +                                 \
             " which is assigned to both global variable and local variable")
#define CLGEN_ERROR_3(x) CLGEN_ERROR(3, x)
#define RUNTM_INFO(num, msg) MARS_INFO(RUNTM, msg, num)
#define RUNTM_WARNING(num, msg) MARS_WARNING(RUNTM, msg, num)
#define RUNTM_ERROR(num, msg) MARS_ERROR(RUNTM, msg, num)
#define RUNTM_ERROR_1(x)                                                       \
  RUNTM_ERROR(1, "Cannot support expression as kernel argument: " + (x))
#define RUNTM_ERROR_2(x)                                                       \
  RUNTM_ERROR(                                                                 \
      2,                                                                       \
      "Finding array/pointer variable with unknown dimension size : " + (x) +)
#define RUNTM_ERROR_3(x)                                                       \
  RUNTM_ERROR(                                                                 \
      3,                                                                       \
      "Cannot support multiple dimensional array in kernel interface: " + (x))
#define RUNTM_ERROR_4(x)                                                       \
  RUNTM_ERROR(4, "Cannot support kernel interface variable with both read "    \
                 "and write accesses in the kernel: " +                        \
                     (x))
#define RUNTM_ERROR_5(x)                                                       \
  RUNTM_ERROR(5, "Cannot support global variable as the actual declaration "   \
                 "for the kernel interface port: " +                           \
                     (x))
#define RUNTM_ERROR_6(x)                                                       \
  RUNTM_ERROR(                                                                 \
      6,                                                                       \
      "Cannot find the unique actual declaration for the variable: " + (x) +)

#define POSTW_ERROR(num, msg) MARS_ERROR(POSTW, msg, num)

#define POSTW_ERROR_1(x, y)                                                    \
  POSTW_ERROR(1, "Cannot support the kernel function " + (x) +                 \
                     " with reference return type: " + (y))
#define POSTW_ERROR_2(x)                                                       \
  POSTW_ERROR(2, "Kernel return expresion can not be aggregate initilizer: " + \
                     (x))

//  ////////////////////////////////////////////  /
//  Pure information messages (Optimization applied, or configurarion is set
//  according to the use inputs)
//  ////////////////////////////////////////////  /
#define PROCS_INFO_1                                                           \
  PROCS_INFO(1, "Automatic fine-grained loop pipelining is enabled")
#define PROCS_INFO_3                                                           \
  PROCS_INFO(3, "Automatic find-grained loop parallelization is enabled")
#define BURST_INFO_1(x, z, y)                                                  \
  BURST_INFO(1, "Memory burst inferred: variable " + (x) + " in scope " +      \
                    (z) + "\n  burst buffer size = " + (y) + " bytes")
#define BURST_INFO_2(x, y)                                                     \
  BURST_INFO(2, "Memory burst inferred manually: variable " + (x) +            \
                    "\n  burst buffer size = " + (y) + " bytes")
#define BURST_INFO_3(x)                                                        \
  BURST_INFO(3, "Memory burst inferred manually: variable " + (x))
#define WDBUS_INFO_1(x, y)                                                     \
  WDBUS_INFO(1, "Memory coalescing inferred: variable " + (x) +                \
                    "\n"                                                       \
                    "  interface bitwidth = " +                                \
                    (y) + " bits")
#define WDBUS_INFO_2(x, y)                                                     \
  WDBUS_INFO(2, "Memory coalescing inferred: variable " + (x) +                \
                    "\n"                                                       \
                    "  max interface bitwidth = " +                            \
                    (y) + " bits")
#define REDUC_INFO_1(x, y)                                                     \
  REDUC_INFO(1, "Applying " + (x) + " reduction on operation " + (y))

#define CGPIP_INFO_1(x, y, z)                                                  \
  CGPIP_INFO(1, "Coarse-grained pipelining applied on loop " + (x) + "  " +    \
                    (y) + " nodes are scheduled into " + (z) +                 \
                    " pipeline stages ")
#define CGPAR_INFO_1(x)                                                        \
  CGPAR_INFO(1, "Coarse-grained parallelization applied: loop " + (x))
#define FGPIP_INFO_2(x, y, z)                                                  \
  FGPIP_INFO(2, "Loop fine-grained parallelization applied: loop " + (x) +     \
                    (y) + "  parallel units:  " + (z))
#define FGPIP_INFO_3(x, y)                                                     \
  FGPIP_INFO(3, "Loop fine-grained pipelining applied: loop " + (x) + (y))
#define FGPIP_INFO_8(x, y, z)                                                  \
  FGPIP_INFO(8, "Loop fine-grained pipelineing applied: loop " + (x) + (y) +   \
                    "  pipeline II:  " + (z))
#define FGPIP_INFO_5(x, y)                                                     \
  FGPIP_INFO(5, "Loop parallelization applied: loop " + (x) + "\n" +           \
                    "  parallel units:  " + (y) +                              \
                    "\n")  // ZP: which one is applied, INFO_2 or INFO_5
#define FGPIP_INFO_6(x, y)                                                     \
  FGPIP_INFO(6, "False loop-carried data dependence applied to variable " +    \
                    (x) + "\n in loop " + (y))
#define FGPIP_INFO_7(x)                                                        \
  FGPIP_INFO(7, "False loop-carried data dependence applied to all the "       \
                "arrays in loop " +                                            \
                    (x))

#define AUREG_WARNING_1(x)                                                     \
  AUREG_WARNING(                                                               \
      1, "Warning: auto register allocation is disabled,\n"                    \
         "\t because aocl does not allow register size larger than 8192.\n"    \
         "\t Please use a smaller value for 'auto_register_size_threshold'(" + \
             (x) + ")")

//  ////////////////////////////////////////////  /
//  General Warning Messages
//  ////////////////////////////////////////////  /
//  General coding style warning
#define MDARR_WARNING_1(x)                                                     \
  MDARR_WARNING(                                                               \
      1, "Found a potential out-of-bound write access on "                     \
         "the argument " +                                                     \
             (x) + "\n" +                                                      \
             "  Hint: please check validty of 'depth' or 'max_depth' "         \
             "attribute")  //  ZP: needs to be tested, it is a useful checker
#define MDARR_WARNING_2(x)                                                     \
  MDARR_WARNING(                                                               \
      2,                                                                       \
      "Found a potential out-of-bound read access on "                         \
      "the argument " +                                                        \
          (x) + "\n" +                                                         \
          "  Hint: please check validty of 'depth' or 'max_depth' attribute")
#define MDARR_WARNING_3(x)                                                     \
  MDARR_WARNING(                                                               \
      3, "Found a task interface variable unused in the kernel function " +    \
             (x))  //  ZP: rephrase it
#define PROCS_INFO_2                                                           \
  PROCS_INFO(                                                                  \
      2,                                                                       \
      "Hint: you may specify the range of the variables in the loop bounds "   \
      "using 'assert'. Please refer to Merlin User Guide")  //  ZP: Hint ->
                                                            //  Warning?

//  Vendor tool warning
#define DISAG_WARNING_1(x, y)                                                  \
  DISAG_WARNING(1, "Intel SDK cannot support more than " + (x) +               \
                       " arguments.\n" + (y) +                                 \
                       "Please try to pack struct/class members to reduce "    \
                       "the number of the kernel interface variables")
#define GLOBL_WARNING_1(x, y)                                                  \
  GLOBL_WARNING(                                                               \
      1, "Intel SDK cannot support more than " + (x) + " arguments.\n" + (y) + \
             "Please try to change global variables into local variables or "  \
             "merge multiple scalar global variables into a single array "     \
             "global variable")
#define WDBUS_WARNING_1                                                        \
  WDBUS_WARNING(                                                               \
      1,                                                                       \
      "OpenCL simulation is disabled "                                         \
      "as SDAccel does not support OpenCL simulation for memory coalescing "   \
      "in C designs.\n"                                                        \
      "  Hint: move the design to C++ or disable memory coalescing.")

//  ////////////////////////////////////////////  /
//  All Disabling Messages (pragma is ignored)
//  ////////////////////////////////////////////  /
#define PROCS_WARNING_5(x)                                                     \
  PROCS_WARNING(5, "Ignoring pragma " + (x) +                                  \
                       " because it can only be applied to a for-loop")
#define PROCS_WARNING_6(x)                                                     \
  PROCS_WARNING(6, "Ignoring pragma " + (x) +                                  \
                       " which isnot followed by a for-loop")
#define PROCS_WARNING_16(x) PROCS_WARNING(16, "Found an unknown pragma: " + (x))
#define PROCS_WARNING_17(x)                                                    \
  PROCS_WARNING(17, "Found an invalid pipeline pragma: " + (x))
#define PROCS_WARNING_18(x)                                                    \
  PROCS_WARNING(18, "Found an invalid parallel pragma: " + (x))
#define PROCS_WARNING_21(x)                                                    \
  PROCS_WARNING(                                                               \
      21,                                                                      \
      "Found an interface pragma without specified interface depth: " + (x))
#define PROCS_WARNING_20(x)                                                    \
  PROCS_WARNING(20, "Found an invalid pipeline_parallel pragma: " + (x))
#define PROCS_WARNING_29(x)                                                    \
  PROCS_WARNING(29, "Found an invalid loop tiling pragma: " + (x))
#define PROCS_WARNING_30(x, y)
#define PROCS_WARNING_31(x)                                                    \
  PROCS_WARNING(31, "Cannot flatten loop " + (x) +                             \
                        " because of unsupported subloops (such as while, "    \
                        "do-while and memcpy)")
#define PROCS_WARNING_32(x, y, z)
#define PROCS_WARNING_33(x)                                                    \
  PROCS_WARNING(33,                                                            \
                "Ignoring pragma " + (x) +                                     \
                    " because Intel SDK cannot support a false dependence "    \
                    "pragma for a specified non-pointer variable")
#define PROCS_WARNING_34(x)                                                    \
  PROCS_WARNING(34, "Ignoring pragma " + (x) +                                 \
                        " because Xilinx tool cannot support a false "         \
                        "dependence pragma without a specified variable")
#define PROCS_WARNING_35(x)                                                    \
  PROCS_WARNING(34, "Ignoring pragma " + (x) +                                 \
                        " because cannot find the specified variable")
#define PROCS_WARNING_36(x)                                                    \
  PROCS_WARNING(                                                               \
      36, "Cannot parallel loop " + (x) +                                      \
              " because of unsupported loop types (such as while, do-while)")
#define PROCS_WARNING_37(x)                                                    \
  PROCS_WARNING(                                                               \
      37, "Found pointer type casting " + (x) +                                \
              ". Vendor tool may have unstable bahaviour on such operations")
#define PROCS_WARNING_38(x)                                                    \
  PROCS_WARNING(                                                               \
      38, "Found non-const initialization on const global vars" + (x) +        \
              ". Vendor tool may not treat it as a compile-time constant.")
#define PROCS_WARNING_39(x)                                                    \
  PROCS_WARNING(39, "Cannot support pragma " + (x) +                           \
                        " with \'auto\' value.\n" +                            \
                        "  Hint: please enable custom DSE flow")
#define PROCS_WARNING_40(x)                                                    \
  PROCS_WARNING(40, "Found unaligned memcpy on " + (x) +                       \
                        "\n"                                                   \
                        " Hint: Please consider a copying size that aligns "   \
                        "source buffer size of memcpy")
#define PROCS_WARNING_41(x, y)                                                 \
  PROCS_WARNING(41, "Found a large const array " + (x) + " of size " + (y) +   \
                        "Byte"                                                 \
                        " which may result in excessive on-chip memory usage." \
                        "  Suggest the const array size smaller than 1MB.")
#define PROCS_WARNING_42(x, y, z)                                              \
  PROCS_WARNING(44, "Argument number for kernel " + (x) + " is " + (y) +       \
                        ".\n"                                                  \
                        "This may caused the unoptimal design. "               \
                        "Suggest kernel argument number not larger than " +    \
                        (z) + ".")
#define PROCS_WARNING_45(x)                                                    \
  PROCS_WARNING(5, "Found a loop " + (x) + " without a bound on " +            \
                        "its tripcount. " +                                    \
                        "Please specify one using " +                          \
                        "'#pragma HLS loop_tripcount max=?' or 'assert'.")
#define BURST_WARNING_1(x, y)                                                  \
  BURST_WARNING(                                                               \
      1, "Memory burst NOT inferred: variable " + (x) +                        \
             "\n"                                                              \
             "  Reason: the bound of the access range is not determined (" +   \
             (y) +                                                             \
             ")\n"                                                             \
             "  Hint: please specify the bounds of the variables using "       \
             "'assert'")
#define BURST_WARNING_2(x, z, y, c)                                            \
  BURST_WARNING(2, "Memory burst inferred: variable " + (x) + " in scope " +   \
                       (z) +                                                   \
                       "\n"                                                    \
                       "  with insufficient on-chip memory (" +                \
                       (y) + " bytes required)\n" +                            \
                       ((c) ? DECREASE_TILING_FATOR : LONG_BURST_HINT))
#define BURST_WARNING_3(x)                                                     \
  BURST_WARNING(3, "Cannot burst all the accesses of variable " + (x) +        \
                       " because of data dependence")
#define BURST_WARNING_4(x, y)                                                  \
  BURST_WARNING(                                                               \
      4, "Memory burst NOT inferred: variable " + (x) +                        \
             ". Reason: burst length cannot be determined (" + (y) +           \
             ") \n"                                                            \
             "  Hint:   set accurate bound for variables using 'assert'")
#define BURST_WARNING_5(x, z, y, c)                                            \
  BURST_WARNING(                                                               \
      5, "Memory burst NOT inferred : variable " + (x) + "in scope " + (z) +   \
             " with big on-chip \n"                                            \
             "  buffer.\n" +                                                   \
             "  Reason: required burst buffer is too large (" + (y) +          \
             " bytes)\n" + ((c) ? DECREASE_TILING_FATOR : LONG_BURST_HINT))
#define BURST_WARNING_6(x)                                                     \
  BURST_WARNING(6, "Memory burst NOT inferred: variable " + (x) +              \
                       ". Reason: improper streaming access order")
#define BURST_WARNING_7(x)                                                     \
  BURST_WARNING(7, "Memory burst NOT inferred: variable " + (x) +              \
                       ". Reason: unsupported access, such as pointer alias, " \
                       "pointer arithmetic operation")
#define BURST_WARNING_8(x)                                                     \
  BURST_WARNING(8, "Memory burst NOT inferred: variable " + (x) +              \
                       ". Reason: improper streaming access order")
#define BURST_WARNING_9(x)                                                     \
  BURST_WARNING(9, "Memory burst NOT inferred: variable " + (x) +              \
                       ". Reason: improper streaming access order")
#define BURST_WARNING_10(x)                                                    \
  BURST_WARNING(10, "Memory burst NOT inferred: variable " + (x) +             \
                        ". Reason: both read and write access")
#define BURST_WARNING_11(x)                                                    \
  BURST_WARNING(11, "Memory burst NOT inferred: variable " + (x) +             \
                        ". Reason: improper streaming access order")
#define BURST_WARNING_12(x, y)                                                 \
  BURST_WARNING(                                                               \
      12, "Memory burst NOT inferred: variable " + (x) +                       \
              ". Reason: access bound cannot be determined (" + (y) +          \
              ") \n"                                                           \
              "  Hint:   set accurate bound for variables using 'assert'")
#define BURST_WARNING_13(x)                                                    \
  BURST_WARNING(13, "Memory burst NOT inferred: variable " + (x) +             \
                        ". Reason: possible negative burst length\n" +         \
                        "  Hint: please specify the bounds of the variables "  \
                        "using 'assert' ")
#define BURST_WARNING_14(x, y)                                                 \
  BURST_WARNING(                                                               \
      14, "Memory burst NOT inferred: variable " + (x) +                       \
              ". Reason: negative burst start address (" + (y) + ")\n" +       \
              "  Hint:  set accurate bound for variables using 'assert'")
#define WDBUS_WARNING_2(x, y)                                                  \
  WDBUS_WARNING(                                                               \
      2, "Memory coalescing NOT inferred: variable " + (x) +                   \
             "\n  Reason: unsupported reference " + (y) +                      \
             "\n  Hint: Please use simple assignment or memcpy if possible")
#define WDBUS_WARNING_4(x)                                                     \
  WDBUS_WARNING(                                                               \
      4, "Memory coalescing NOT inferred: variable " + (x) +                   \
             "\n  Reason: the number of array dimensions is larger than 4 ")
#define WDBUS_WARNING_7(x)                                                     \
  WDBUS_WARNING(7, "Memory coalescing not inferred because of unsupported "    \
                   "member function\n" +                                       \
                       (x))
#define WDBUS_WARNING_8                                                        \
  WDBUS_WARNING(                                                               \
      8, "Memory coalescing not inferred because of mixed C/C++ design")
#define WDBUS_WARNING_9(x, y)                                                  \
  WDBUS_WARNING(9, "Memory coalescing variable " + (x) +                       \
                       " may cause long compilation time of downstream tool."  \
                       "\n  Reason: the access offset is not divisible by " +  \
                       (y))
#define WDBUS_WARNING_10(x, y, z)                                              \
  WDBUS_WARNING(10, "Memory coalescing NOT inferred: variable " + (x) +        \
                        "\n  Reason: type " + (y) +                            \
                        " is not supported now.\n"                             \
                        "  Note: only " +                                      \
                        (z) + " are supported now.")
#define WDBUS_WARNING_11(x, y)                                                 \
  WDBUS_WARNING(11, "Memory coalescing NOT inferred: variable " + (x) +        \
                        "\n  Reason: burst length is less than threshold (" +  \
                        (y) + " bytes).\n" +                                   \
                        "  Hint: reset threshold by using --attribute "        \
                        "bus_length_threshold=?")
#define WDBUS_WARNING_12(x)                                                    \
  WDBUS_WARNING(12,                                                            \
                "Memory coalescing NOT inferred explicitly: variable " + (x))
#define WDBUS_WARNING_13(x, y)                                                 \
  WDBUS_WARNING(13, "Memory coalescing NOT inferred: variable " + (x) +        \
                        "\n  because of invalid bus bitwidth (" + (y) +        \
                        " bits).")
#define REDUC_WARNING_1(x, y)                                                  \
  REDUC_INFO(2, "Disabling automatic reduction in loop " + (x) +               \
                    " because of multiple reduction operations:\n" + (y) +     \
                    "   Hint: try to group these operations into a reduction " \
                    "function")
#define REDUC_WARNING_2(x)                                                     \
  REDUC_WARNING(                                                               \
      2, "Reduction on operation " + (x) + " not applied: no benefit.\n" +     \
             "Hint: using parallel factor to apply loop tiling may help "      \
             "trigger reduction optimization and enhance the performance. ")
#define REDUC_WARNING_3(x)                                                     \
  REDUC_WARNING(3, "Reduction in loop " + (x) +                                \
                       " not applied: "                                        \
                       "noncanonical loop.")
#define REDUC_WARNING_4(x)                                                     \
  REDUC_WARNING(4, "Reduction in loop " + (x) +                                \
                       " not applied: "                                        \
                       ": it uses the outer loop's iterator in " +             \
                       "the loop bound expression.")
#define REDUC_WARNING_5(x, y)                                                  \
  REDUC_WARNING(5, "Reduction on variable '" + (x) + "' in loop " + (y) +      \
                       " not applied: "                                        \
                       "no valid reduction operation.")
#define REDUC_WARNING_6(x, y)                                                  \
  REDUC_WARNING(                                                               \
      6, "Reduction on variable '" + (x) + "' in loop " + (y) +                \
             " not applied"                                                    \
             ": multiple accesses on the variable exist in the loop.\n"        \
             "Hint: try to reduce the accesses to the variable in the loop " + \
             "or change the scope of reduction optimization. ")
#define REDUC_WARNING_7(x, y)                                                  \
  REDUC_WARNING(                                                               \
      7, "Reduction on variable '" + (x) + "' in loop " + (y) +                \
             " may has no performance benefit.\n" +                            \
             "Hint: using parallel factor to apply loop tiling may help " +    \
             "enhance the performance. ")
#define REDUC_WARNING_8(x, y)                                                  \
  REDUC_WARNING(8, "Reduction on variable '" + (x) + "' in loop " + (y) +      \
                       " not applied: unknown trip count.\n"                   \
                       "  Hint: please specify the bounds of the trip count "  \
                       "using 'assert' ")
#define REDUC_WARNING_9(x, y, z)                                               \
  REDUC_WARNING(9, "Reduction on variable '" + (x) + "' in loop " + (y) +      \
                       " not applied: " +                                      \
                       "unsupported reduction operation '" + (z) + "'")
#define LINEBUF_INFO_1(x, y)                                                   \
  LINEBUF_INFO(1, "Applying line buffer on variable '" + (x) + "' in loop " +  \
                      (y))
#define LINEBUF_WARNING_1(x, y)                                                \
  LINEBUF_WARNING(1, "Stopping line buffer optimization on variable '" + (x) + \
                         "' in loop " + (y) + ": line buffer is only for " +   \
                         "off-chip memory accesses.\n")
#define LINEBUF_WARNING_2(x, y)                                                \
  LINEBUF_WARNING(2, "Stopping line buffer optimization on variable '" + (x) + \
                         "' in loop " + (y) +                                  \
                         ": the accesses are not stencil.\n")
#define LINEBUF_WARNING_3(x, y)                                                \
  LINEBUF_WARNING(3, "Stopping line buffer optimization on variable '" + (x) + \
                         "' in loop " + (y) +                                  \
                         ": the variable cannot be delinearized.\n")
#define LINEBUF_WARNING_4(x, y)                                                \
  LINEBUF_WARNING(                                                             \
      4, "Stopping line buffer optimization on variable '" + (x) +             \
             "' in loop " + (y) +                                              \
             ": the array size is inconsistent with the loop's trip count.\n")
#define CGPIP_WARNING_1(x, y)                                                  \
  CGPIP_WARNING(                                                               \
      1,                                                                       \
      "Pipelining NOT applied on loop " + (x) +                                \
          "\n  Reason: data dependency across loop iterations on" + (y) +      \
          "\n  Hint:   try to move dependency into the same loop iteration")
#define CGPIP_WARNING_2(x, y, z, w)                                            \
  CGPIP_WARNING(                                                               \
      2, "Coarse-grained pipelining NOT applied on loop " + (x) +              \
             "\n  Reason: " + (y) +                                            \
             " statements are not allowed in coarse-grained pipeline " + (z) + \
             (w))
#define CGPIP_WARNING_3(x, y, z)                                               \
  CGPIP_WARNING(3, "Coarse-grained pipelining NOT applied on loop " + (x) +    \
                       "\n  Reason: multiple accesses to " + (y) +             \
                       " in multiple pipeline stages " + (z) +                 \
                       ".\n  Hint:   try to group the pipeline stages using "  \
                       "brackets {}. Refer to Merlin User Guide")
#define CGPIP_WARNING_4(x)                                                     \
  CGPIP_WARNING(4, "Coarse-grained pipelining NOT applied on loop " + (x) +    \
                       "\n  Reason: multiple bus accesses exist in multiple "  \
                       "pipeline stages.")
#define CGPIP_WARNING_5(x)                                                     \
  CGPIP_WARNING(5, "Coarse-grained pipelining NOT applied on loop " + (x) +    \
                       "\n  Reason: the loop is non-canonical. ")
#define CGPIP_WARNING_6(x, y, z)                                               \
  CGPIP_WARNING(6, "Coarse-grained pipelining NOT applied on loop " + (x) +    \
                       "\n  Reason: potential cross-iteration data "           \
                       "dependence on variable " +                             \
                       (y) + ".\n" + (z))
#define CGPIP_WARNING_7(x, y)                                                  \
  CGPIP_WARNING(                                                               \
      7, "Coarse-grained pipelining NOT applied on loop " + (x) +              \
             "\n  Reason: multiple variable declarations in a statement " +    \
             (y))
#define CGPIP_WARNING_8(x)                                                     \
  CGPIP_WARNING(8,                                                             \
                "Coarse-grained pipelining NOT applied on loop " + (x) +       \
                    "\n  Reason: there is only one statement in the loop. ")
#define CGPIP_WARNING_9(x, y)                                                  \
  CGPIP_WARNING(                                                               \
      9,                                                                       \
      "Coarse-grained pipelining NOT applied on loop " + (x) +                 \
          "\n  Reason: potential uninitialized data elements in variable " +   \
          (y) + ".")
#define CGPAR_WARNING_1(x, y)                                                  \
  CGPAR_WARNING(1, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of access on the external memory port " +     \
                       (y))
#define CGPAR_WARNING_2(x, y)                                                  \
  CGPAR_WARNING(2,                                                             \
                "Cannot parallel the loop " + (x) + " because the access " +   \
                    "to the following array(s) cannot be separated among " +   \
                    "parallel threads. Please consider to separate the " +     \
                    "accesses by introducing a dimension along " + (x) +       \
                    " to " + "reshape the array(s):\n" + (y))
#define CGPAR_WARNING_3(x)                                                     \
  CGPAR_WARNING(3, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of non-canonical loop")
#define CGPAR_WARNING_4(x)                                                     \
  CGPAR_WARNING(4, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of non-constant loop bound")
#define CGPAR_WARNING_5(x)                                                     \
  CGPAR_WARNING(5, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of break statements")
#define CGPAR_WARNING_6(x)                                                     \
  CGPAR_WARNING(6, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of continue statements")
#define CGPAR_WARNING_7(x)                                                     \
  CGPAR_WARNING(7, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of return statements")
#define CGPAR_WARNING_8(x)                                                     \
  CGPAR_WARNING(8, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of goto statements")
#define CGPAR_WARNING_9(x)                                                     \
  CGPAR_WARNING(9, "Coarse-grained parallelization NOT applied: loop " + (x) + \
                       " because of label statements")
#define CGPAR_WARNING_10(x, y)                                                 \
  CGPAR_WARNING(10, "Coarse-grained parallelization NOT applied: loop " +      \
                        (x) + " because of unsupported task spawn:\n" + (y))
#define CGPAR_WARNING_11(x, y)                                                 \
  CGPAR_WARNING(                                                               \
      11, "Coarse-grained parallelization NOT applied: loop " + (x) +          \
              " because of access on non-constant global variable " + (y))
#define CGPAR_WARNING_12(x, y)                                                 \
  CGPAR_WARNING(                                                               \
      12, "Coarse-grained parallelization NOT applied: loop " + (x) +          \
              " because of access on non-constant local static variable " +    \
              (y))
#define CGPAR_WARNING_13(x, y, z)                                              \
  CGPAR_WARNING(13, "Coarse-grained parallelization NOT applied: loop " +      \
                        (y) + " because access on array " + (x) +              \
                        " cannot be parallelized due to " + (z))

#define CGPAR_WARNING_14(y, z)                                                 \
  CGPAR_WARNING(14, "Coarse-grained parallelization NOT applied: loop " +      \
                        (y) +                                                  \
                        " because access on ports cannot be streamed due to "  \
                        "following reasons\n" +                                \
                        (z))

#define LTILE_WARNING_1(x)                                                     \
  LTILE_WARNING(1, "Ignoring a loop tiling pragma for non-canonical loop: " +  \
                       (x))
#define LTILE_WARNING_2(x)                                                     \
  LTILE_WARNING(2, "Ignoring a loop tiling pragma for loop with decremental "  \
                   "iteration space: " +                                       \
                       (x))
#define LTILE_WARNING_3(x)                                                     \
  LTILE_WARNING(                                                               \
      3,                                                                       \
      "Ignoring a loop tiling pragma for loop with non-constant step: " + (x))
#define LTILE_WARNING_4(x)                                                     \
  LTILE_WARNING(4, "Cannot tile a loop with loop increment not equal to 1: " + \
                       (x))
#define LTILE_WARNING_5(x)                                                     \
  LTILE_WARNING(5, "Ignoring a loop tiling pragma for loop with non-constant " \
                   "lower bound: " +                                           \
                       (x))
#define LTILE_WARNING_6(x)                                                     \
  LTILE_WARNING(6, "Ignoring a loop tiling pragma for a tiling factor <=1: " + \
                       (x))
#define LTILE_WARNING_7(x)                                                     \
  LTILE_WARNING(7, "Ignoring a loop tiling pragma for an indivisible loop "    \
                   "with label in its loop body: " +                           \
                       (x))
#define LTILE_WARNING_8(x)                                                     \
  LTILE_WARNING(8,                                                             \
                "Loop will be parallelized but without tiling because Factor " \
                ">= Loop Trip Count: " +                                       \
                    (x))
#define LTILE_WARNING_9(x)                                                     \
  LTILE_WARNING(9, "Ignoring a loop tiling pragma for a loop with write "      \
                   "access on its iterator in its loop body: " +               \
                       (x))
#define LTILE_WARNING_10(x)                                                    \
  LTILE_WARNING(10, "Loop tiling applied on a loop with in-divisible "         \
                    "trip count: " +                                           \
                        (x) + ", which may introduce extra logic.")

#define FGPIP_WARNING_2(x, y)                                                  \
  FGPIP_WARNING(2,                                                             \
                "Loop fine-grained parallelization CANNOT be applied: loop " + \
                    (x) + (y) +                                                \
                    " because the loop contains subloop(s) not being "         \
                    "parallelized.")
#define FGPIP_WARNING_4(x, y)                                                  \
  FGPIP_WARNING(4, "Parallelization NOT applied: loop " + (x) +                \
                       "  Reason: Invalid parallel factor: " + (y) +           \
                       " \n"                                                   \
                       "  Hint:   parallel factor can only be constant "       \
                       "positive integer ")
#define FGPIP_WARNING_5(x, y, z)                                               \
  FGPIP_WARNING(                                                               \
      5,                                                                       \
      "Parallelization NOT applied: loop " + (x) +                             \
          "  Reason: Reason: do not support the partitioning on varaible " +   \
          (y) + "' dimension " + (z))
#define FGPIP_WARNING_6(x, y)                                                  \
  FGPIP_WARNING(6, "Fine-grained pipelining NOT applied: loop " + (x) +        \
                       "  Reason: conflict partitioning factor on variable " + \
                       (y) + " ")

#define FGPIP_WARNING_7(x)                                                     \
  FGPIP_WARNING(7, "Parallelization NOT applied: loop " + (x) +                \
                       "  Reason: loop " + (x) + "' is non-canonical ")
#define FGPIP_WARNING_10(x, y)                                                 \
  FGPIP_WARNING(10, "Fine-grained pipelining NOT applied: loop " + (x) +       \
                        "  Reason: no partitioning solution for variable " +   \
                        (y) + " in the loop")
#define FGPIP_WARNING_12(x, y)                                                 \
  FGPIP_WARNING(                                                               \
      12, "Loop fine-grained parallelization may NOT be applied: loop " +      \
              (x) + (y) +                                                      \
              "because cannot completely parallel a loop with an unknown "     \
              "trip count.")
#define FGPIP_WARNING_14(x, y, z, t)                                           \
  FGPIP_WARNING(                                                               \
      14, "Partitioning array " + (y) + " on dimension " + (z) +               \
              " with factor " + (t) +                                          \
              "\n"                                                             \
              "Note that loop " +                                              \
              (x) +                                                            \
              " probably cannot be fully-pipelined due to partially access " + \
              "conflicts of variable " + (y))
#define FGPIP_WARNING_15(x, y, z, t)                                           \
  FGPIP_WARNING(15, "Suboptimal loop parallelization probably in Loop " +      \
                        (x) + " as " + "variable " + (y) + " dimension " +     \
                        (z) + " cannot be splitted into more than " + (t) +    \
                        " banks due to vendor tool's threshold.")
#define FGPIP_WARNING_16(x, y)                                                 \
  FGPIP_WARNING(16, "Loop fine-grained pipelining may NOT be applied: loop " + \
                        (x) + (y) +                                            \
                        " because the loop may contain subloop(s) not being "  \
                        "parallelized.")
#define FGPIP_WARNING_17(x, y)                                                 \
  FGPIP_WARNING(17,                                                            \
                "To apply dependency false pragma "                            \
                "on variable " +                                               \
                    (x) + " in loop " + (y) +                                  \
                    " may result in an incorrect hardware implementation.")
#define FGPIP_WARNING_18(x, y)                                                 \
  FGPIP_WARNING(                                                               \
      18, "Ignoring pragma '" + (x) +                                          \
              "'  because of invalid dimension information 'dim=" + (y) + "'")
#define FGPIP_WARNING_19(x, y)                                                 \
  FGPIP_WARNING(19, "To apply fine-grained parallelization on loop " + (x) +   \
                        (y) + " may lead to vendor tool error out " +          \
                        "because loop trip count is variable. \n")
#define FGPIP_WARNING_21(x, y)                                                 \
  FGPIP_WARNING(21, "Ignoring pragma '" + (x) +                                \
                        "'  because of unknown partitioning factor '" + (y) +  \
                        "'")

//  ////////////////////////////////////////////  /
//  All QoR Warning Messages (potential sub-optimal)
//  ////////////////////////////////////////////  /
#define PROCS_WARNING_3(x)                                                     \
  PROCS_WARNING(3, "Found a loop " + (x) + " with unknown loop trip count. ")
#define PROCS_WARNING_28(x, y)                                                 \
  PROCS_WARNING(                                                               \
      28, ((x) ? "Cannot determine the lower bound for the expression "        \
               : "Cannot determine the upper bound for the expression ") +     \
              (y) + ", this may disable certain optimizations")
#define BURST_WARNING_15(x, y, z, tiled_loop, full_access)                     \
  BURST_WARNING(15, "Low bandwidth efficiency of memory burst: variable " +    \
                        (x) + " in scope " + (z) + ".\n" +                     \
                        "  Reason: short burst length = " + (y) + " bytes\n" + \
                        ((full_access)                                         \
                             ? " due to small external memory"                 \
                             : ((tiled_loop) ? INCREASE_TILING_FATOR : "")))

#define BURST_WARNING_16(x, y, r, s)                                           \
  BURST_WARNING(                                                               \
      16, "Stopping inferring longer memory burst: variable " + (x) +          \
              " in scope " + (y) + ".\n" + "  Reason: required resource (" +   \
              (r) + " bytes)  more than lifting threshold (" + (s) +           \
              " bytes).\n" +                                                   \
              "  Hint: increase the lifting threshold by setting \n" +         \
              "        \'burst_lifting_size_threshold\'")

#define BURST_WARNING_17(x)                                                    \
  BURST_WARNING(                                                               \
      17, "Stopping inferring longer memory burst: variable " + (x) + ".\n" +  \
              "  Reason: required resource is more than the remaining "        \
              "resource.\n" +                                                  \
              "  Hint: increase the total resource threshold by setting \n" +  \
              "        \'burst_total_size_threshold\'")

#define BURST_WARNING_18(x, y, c, r, s)                                        \
  BURST_WARNING(18, "Stopping inferring longer memory burst: variable " +      \
                        (x) + " in scope " + (y) + ".\n" +                     \
                        "  Reason: required resource (" + (r) +                \
                        " bytes) more than single resource "                   \
                        "threshold (" +                                        \
                        (s) + " bytes).\n" +                                   \
                        ((c) ? DECREASE_TILING_FATOR : SHORT_BURST_HINT))

#define BURST_WARNING_19(x, y, z)                                              \
  BURST_WARNING(                                                               \
      19, "Found a possible "                                                  \
          "negative lower bound " +                                            \
              (x) + " in memory burst for array " + (y) + " reference " +      \
              (z) + "\n" + "  Hint: possible solutions are\n" +                \
              "  1. Set accurate bound for variables using 'assert'\n" +       \
              "  2. Please consider shifting the data layout to ensure "       \
              "the lower " +                                                   \
              " bounds are non-negative\n" +                                   \
              "  3. Or consider separating the boundary loop "                 \
              "iterations from the main " +                                    \
              "loop, so that the main loop will always have a "                \
              "non-negative lower bound " +                                    \
              "for memory burst")

#define BURST_WARNING_20                                                       \
  BURST_WARNING(                                                               \
      20, "Found on-chip buffer is almost used out before inferring burst")

#define WDBUS_WARNING_3(x, y)                                                  \
  WDBUS_WARNING(3, "Suboptimal memory coalescing: variable " + (x) +           \
                       "\n  Reason: the lowest dimension size of on-chip "     \
                       "buffer is not divisible by " +                         \
                       (y))
#define WDBUS_WARNING_5(x, y)                                                  \
  WDBUS_WARNING(5, "Suboptimal memory coalescing: variable " + (x) +           \
                       "\n  Reason: the access offset is not divisible by " +  \
                       (y))
#define WDBUS_WARNING_6(x)                                                     \
  WDBUS_WARNING(6, "Suboptimal memory coalescing: variable " + (x) +           \
                       "  Reason:  the lowest dimension of on-chip buffer "    \
                       " is less than the coalescing length")

#define FGPIP_WARNING_1(x)                                                     \
  FGPIP_WARNING(                                                               \
      1,                                                                       \
      "Suboptimal loop parallelization: variable " + (x) +                     \
          "  Reason: too many parallelized accesses on the same array\n"       \
          "  Hint: try to match the access pattens in the different loops\n"   \
          "        with the access pattern in memory coalescing")

#define FGPIP_WARNING_11(x, y)                                                 \
  FGPIP_WARNING(11, "Memory partitioning NOT applied: scalar " + (x) +         \
                        "  \nReason: possible out-of-bound access or invalid " \
                        "parallel factor in loop " +                           \
                        (y) + " ")
#define FGPIP_WARNING_13(x, y)                                                 \
  FGPIP_WARNING(13, "Loop " + (x) +                                            \
                        " probably cannot be fully-pipelined as variable " +   \
                        (y) +                                                  \
                        " has an irregular access pattern and cannot be "      \
                        "split into multiple banks.")
#define FGPIP_WARNING_3(x)                                                     \
  FGPIP_WARNING(                                                               \
      3, "suboptimal loop parallelization: loop " + (x) +                      \
             "  Reason: Unknown loop bound may disable parallelization.")
#define FGPIP_WARNING_8(x, y)                                                  \
  FGPIP_WARNING(                                                               \
      8, "Ignoring pragma " + (x) +                                            \
             "  because loop-carried dependence exists on variable " + (y))
#define FGPIP_WARNING_20(x)                                                    \
  FGPIP_WARNING(20, "Potentially inefficient variable configuration: "         \
                    "array " +                                                 \
                        (x) +                                                  \
                        "\n Reason: multiple accesses or complex "             \
                        "access pattern. ")

//  ////////////////////////////////////////////  /
//  To be discussed
//  ////////////////////////////////////////////  /
#define PROCS_WARNING_1(x)                                                     \
  PROCS_WARNING(1, "Ignoring all pragmas in the header file " +                \
                       (x))  //  ZP: is it still necessary
#define PROCS_WARNING_9(x)                                                     \
  PROCS_WARNING(9, "Goto statement may cause sub-optimal results " +           \
                       (x))  //  ZP: do we support goto
#define PROCS_WARNING_10(x)                                                    \
  PROCS_WARNING(                                                               \
      10, "Cannot support the function " + (x) +                               \
              " with the return statements " +                                 \
              "in the middle of the function body.\n")  // ZP: to confirm, seems
                                                        //  removable
#define PROCS_WARNING_15(x)                                                    \
  PROCS_WARNING(15, "Found an incomplete pragma: " +                           \
                        (x))  //  ZP: to give more useful info, like the full
                              //  format for the specific pragma
#define PROCS_WARNING_23(x, y)                                                 \
  PROCS_WARNING(23, "Found the access indices of the array " + (x) +           \
                        " are not identical within " + (y) +                   \
                        ".\n   Hints: Coarse-grained pipelining requires "     \
                        "access indices to be identical in each loop.")
#define PROCS_WARNING_26(x)                                                    \
  PROCS_WARNING(26, "Found the access indices of array " + (x) +               \
                        " are not identical"                                   \
                        ".\n  Hints: Array multidimensional delinearization "  \
                        "requires access indices to be identical.")
#define PROCS_WARNING_27(x)                                                    \
  PROCS_WARNING(27, "Cannot support loop with the continue/break statement " + \
                        (x))
#define DISAG_ERROR_1(x)                                                       \
  DISAG_ERROR(                                                                 \
      1, "Cannot decompose " + (x) +                                           \
             "\n Hint: complex pointer re-interpretation is not supported. " + \
             "Please rewrite")
#define DISAG_ERROR_2(x) DISAG_ERROR(2, "Cannot support " + (x))
#define DISAG_ERROR_3(x, y)                                                    \
  DISAG_ERROR(3, "Intel SDK cannot support more than " + (x) +                 \
                     " arguments.\n" + (y) +                                   \
                     "Please try to pack struct/class members to reduce the "  \
                     "number of the kernel interface variables")
#define DISAG_ERROR_4(x) DISAG_ERROR(4, "Cannot support depth " + (x))

#define GLOBL_ERROR_3(x)                                                       \
  GLOBL_ERROR(3, "Cannot modify global pointer in the kernel\n" + (x))
#define GLOBL_ERROR_4(x)                                                       \
  GLOBL_ERROR(                                                                 \
      4,                                                                       \
      "Global variable name conflicts with a local variable or argument\n" +   \
          (x))

#define INFTF_ERROR_4(x) INFTF_ERROR(4, x)
#define INFTF_ERROR_5(x) INFTF_ERROR(5, x)

#define INFTF_ERROR_7(x, y, z)                                                 \
  INFTF_ERROR(7, "Found incorrect depth info '" + (y) + "' for port " + (x) +  \
                     "with dimension size " +                                  \
                     (z))  //  why incorrect? expectation?
#define INFTF_ERROR_8(x, y, z)                                                 \
  INFTF_ERROR(8, "Found incorrect max_depth info '" + (y) + "'for port " +     \
                     (x) + "with dimension size " + (z))
#define INFTF_ERROR_9(x)                                                       \
  INFTF_ERROR(9,                                                               \
              "Found non-static pointer " + (x) +                              \
                  " (this pointer may refer to different memory locations)")
#define PRAGM_ERROR_1(x) PRAGM_ERROR(1, x)  //  ?? move the text here
#define PRAGM_ERROR_2(x) PRAGM_ERROR(2, x)
#define PRAGM_ERROR_3(x) PRAGM_ERROR(3, x)
#define PRAGM_ERROR_4(x) PRAGM_ERROR(4, x)
#define PRAGM_ERROR_5(x) PRAGM_ERROR(5, x)
#define PRAGM_ERROR_6(x) PRAGM_ERROR(6, x)
#define PRAGM_ERROR_7(x) PRAGM_ERROR(7, x)
#define PRAGM_WARNING(num, msg) MARS_WARNING(INFTF, msg, num)
#define PRAGM_WARNING_1(x) PRAGM_WARNING(1, x)

#define CGPAR_ERROR_1(x, y)                                                    \
  CGPAR_ERROR(                                                                 \
      1,                                                                       \
      "Undetermined channel size for access '" + (x) + "' in scope '" + (y) +  \
          "'\nHint: Possible solutions: \n  a) Use copy_in/copy_out clause "   \
          "in the parallel pragma\n  b) Set range assertion for the "          \
          "surrouding loops with unknown bounds\n  c) Remove parallel pragma")

#define LTILE_ERROR_1(x)                                                       \
  LTILE_ERROR(1, "Cannot tile a loop with non-zero start: " + (x))
#define LTILE_ERROR_2(x)                                                       \
  LTILE_ERROR(                                                                 \
      2,                                                                       \
      "Cannot tile a loop with a bound not divisible by the factor: " + (x))
#define LTILE_ERROR_3(x, y)                                                    \
  LTILE_ERROR(3, "Cannot tile an indivisible loop with labels: " + (x) +       \
                     "Reason: the label '" + (y) +                             \
                     "' is targeted by a goto statement.")

#define AST_IF_WARNING_1(x)                                                    \
  AST_IF_WARNING(1, "Unknown bitwidth for interface type: " + (x) +            \
                        "\n")  // ZP: is it a limitation to error out?

#define FGPIP_ERROR_1(x, y)                                                    \
  FGPIP_ERROR(1, "Unable to completely parallel loop " + (x) + (y) +           \
                     "because its trip count is variable.")
#define FGPIP_ERROR_2(x, y)                                                    \
  FGPIP_ERROR(2, "Loop fine-grained pipelining CANNOT be applied: loop " +     \
                     (x) + (y) +                                               \
                     "Because unable to achieve non-conflict memory access.")

#define FGPIP_WARNING_9(x, y, z)                                               \
  FGPIP_WARNING(9, "Parallelization NOT applied: loop " + (x) +                \
                       "\n"                                                    \
                       "  Reason: Invalid parallel factor: " +                 \
                       (y) +                                                   \
                       " \n"                                                   \
                       "  Hint:   parallel factor should be an integer "       \
                       "within the loop trip count " +                         \
                       (z) + ".")
#define FGPIP_INFO_1(x, y, z)                                                  \
  FGPIP_INFO(1, "Partitioning array '" + (x) + "' on dimension " + (y) +       \
                    (z))  //  / ZP: to be only displayed internally?
#define FGPIP_INFO_4                                                           \
  FGPIP_INFO(                                                                  \
      4,                                                                       \
      "Automated loop flattening is turned off\n")  // ZP: I understand auto
                                                    //  loop flatten is
                                                    //  globally turned off?

//  ////////////////////////////////////////////  /
//  To be discarded (suppose not happen)
//  ////////////////////////////////////////////  /
#define PROCS_WARNING_4(x)                                                     \
  PROCS_WARNING(4, "Found an class member variable " + (x) + "in the kernel")
#define PROCS_WARNING_7(x)                                                     \
  PROCS_WARNING(7, "Ignoring pragma " + (x) +                                  \
                       " because only complete parallelism is supported.")
#define PROCS_WARNING_8(x, y)                                                  \
  PROCS_WARNING(8, "Ignoring parallel pragma " + (x) + " on the outer loop " + \
                       (y) +                                                   \
                       " when its sub-loops are not fully parallelized.\n" +   \
                       "  Hint: use \"parallel flatten\" clause to "           \
                       "parallelize all the sub-loops")
#define PROCS_WARNING_11(x)                                                    \
  PROCS_WARNING(11, "Found a class member function " + (x))
#define PROCS_WARNING_19                                                       \
  PROCS_WARNING(19, "The pragma 'pipeline_parallel' is deprecated.\n"          \
                    "  Hint: please use both individual 'pipeline' and "       \
                    "'parallel' pragmas instead")
#define PROCS_ERROR_1(x)                                                       \
  PROCS_ERROR(1, "Found an overloaded kernel function " + (x))
#define GLOBL_ERROR_1(x)                                                       \
  GLOBL_ERROR(1, "Kernel and all the functions used in a kernel cannot be "    \
                 "declared in the header "                                     \
                 "file, if it contains global variable\n" +                    \
                     (x))
#define GLOBL_ERROR_2(x)                                                       \
  GLOBL_ERROR(2, "Kernel and all the functions used in a kernel cannot be "    \
                 "shared by host, "                                            \
                 "if it contains global variable\n" +                          \
                     (x))
#define INFTF_WARNING_1(x)                                                     \
  INFTF_WARNING(1, "Cannot support stream prefetch for port " + (x) +          \
                       " in Xilinx flow")
#define INFTF_ERROR_1(x, y)                                                    \
  INFTF_ERROR(                                                                 \
      1, "Kernel and all the functions used in a kernel cannot be declared "   \
         "in the header file, if it contains struct parameters\n"              \
         "    Function name  : " +                                             \
             (x) + "\n    File location  : " + (y))
#define INFTF_ERROR_2(x, y)                                                    \
  INFTF_ERROR(                                                                 \
      2, "Kernel and all the functions used in a kernel  cannot be declared "  \
         "in the header file, if it contains multiple dimension parameters\n"  \
         "    Function name  : " +                                             \
             (x) + "\n    File location  : " + (y))
#define INFTF_ERROR_3(x)                                                       \
  INFTF_ERROR(3, "Invalid interface depth value : " + (x) +                    \
                     "\n    Hint: depth can only be constant integer number "  \
                     "or scalar from function parameter but not expression")
#define INFTF_ERROR_10(x, y, z)                                                \
  INFTF_ERROR(                                                                 \
      10, "We do not support functions with arguments that have both local "   \
          "variables and global variables as parameters in different "         \
          "function calls\n    Function name  : " +                            \
              (x) + "\n    Parameter name  : " + (y) +                         \
              "\n    File location  : " + (z))

#define KWRAP_ERROR_1(x)                                                       \
  KWRAP_ERROR(                                                                 \
      1, "Cannot support user defined types outside header files: \n" + (x))
#define FUCIN_ERROR_1(x)                                                       \
  FUCIN_ERROR(1, "Cannot support non-constant global variable in kernel: \n" + \
                     (x))
#define FUCIN_ERROR_2(x)                                                       \
  FUCIN_ERROR(2, "Host function and kernel function cannot be "                \
                 "defined in the same file, please separate them into "        \
                 "different files. \n" +                                       \
                     (x))
#define FUCIN_ERROR_3(x, y)                                                    \
  FUCIN_ERROR(3, "A kernel function and a function used in the kernel are "    \
                 "defined in the different files.\n"                           \
                 "Kernel function: " +                                         \
                     (y) +                                                     \
                     "\n"                                                      \
                     "Sub function: " +                                        \
                     (x) + "\n  Hint: please merge them into the same file")
#define FUCIN_ERROR_4(x)                                                       \
  FUCIN_ERROR(4,                                                               \
              "Cannot inline memcpy because of non primitive array copy: \n" + \
                  (x) + "Please rewrite")
#define FUCIN_ERROR_5(x)                                                       \
  FUCIN_ERROR(5, "Cannot inline memcpy because of different base types: \n" +  \
                     (x) + "Please rewrite")

#define SWITCH_MESSAGE                                                         \
  ""  //  "\n  The design flow will switch to low effort mode automatically"

#define PROCS_WARNING_2(x, y)                                                  \
  PROCS_WARNING(2, "Found a non-canonical loop " + (x) +                       \
                       " which may affect the quality of the result" +         \
                       ((y) ? SWITCH_MESSAGE : ""))

#define PROCS_WARNING_24(x)                                                    \
  PROCS_WARNING(                                                               \
      24, "Found multiple loops containing the accesses of an local array " +  \
              (x) + ".\n" + SWITCH_MESSAGE)
#define PROCS_WARNING_25(x)                                                    \
  PROCS_WARNING(                                                               \
      25, "Found more than one loop with the accesses of a port array " +      \
              (x) +                                                            \
              ".\n   Hints: Coarse-grained pipelining requires one read-only " \
              "or write-only loop for each port array.\n" +                    \
              SWITCH_MESSAGE)

#define FUCIN_WARNING_1(x, y)                                                  \
  FUCIN_WARNING(1, "Cannot support a function with void return type called "   \
                   "as an expression \n" +                                     \
                       (x) + ((y) ? SWITCH_MESSAGE : ""))
#define FUCIN_WARNING_2(x, y)                                                  \
  FUCIN_WARNING(2, "Cannot inline a function with \"return\" in the middle "   \
                   "of function body \n" +                                     \
                       (x) + ((y) ? SWITCH_MESSAGE : ""))
#define FUCIN_WARNING_3(x, y)                                                  \
  FUCIN_WARNING(3, "Cannot inline a function with \"goto\" statement \n" +     \
                       (x) + ((y) ? SWITCH_MESSAGE : ""))

#define COMCK_INFO(num, msg) MARS_INFO(COMCK, msg, num)
#define COMCK_WARNING(num, msg) MARS_WARNING(COMCK, msg, num)
#define COMCK_ERROR(num, msg) MARS_ERROR(COMCK, msg, num)

#define COMCK_WARNING_1(x)                                                     \
  COMCK_WARNING(1, "Cannot support non-canonical loop " + (x))
#define COMCK_WARNING_2(x)                                                     \
  COMCK_WARNING(2, "Cannot support the loop " + (x) +                          \
                       " without the simple bound.")
#define COMCK_WARNING_3(x)                                                     \
  COMCK_WARNING(3, "Cannot support loop with the continue/break statement " +  \
                       (x))
#define COMCK_WARNING_4(x, y)                                                  \
  COMCK_WARNING(4, "The Communication on the shared port " + (x) +             \
                       " whose references are on either the loop " + (y) +     \
                       " or if condition cannot be supported.")
#define COMCK_WARNING_5(x)                                                     \
  COMCK_WARNING(5, "Found a conditionally executed loop: \n " + (x) +          \
                       " \n  Coarse-grained pipeline does not allow loop "     \
                       "within if-statement")
#define COMCK_WARNING_6(x)                                                     \
  COMCK_WARNING(6, "Cannot support pointer alias as shared port " + (x))
#define COMCK_WARNING_7(x)                                                     \
  COMCK_WARNING(7, "Cannot support the while/do-while loop " + (x))

#define STREAM_ERROR(num, msg) MARS_ERROR(STREAM, msg, num)
#define STREAM_ERROR_1(x)                                                      \
  STREAM_ERROR(1, "cannot detect access patten for variable: " + (x))
#define STREAM_ERROR_2(x, y)                                                   \
  STREAM_ERROR(2, "stream port " + (x) + " cannot be separated:\n    " + (y))
#define STREAM_ERROR_3(x)                                                      \
  STREAM_ERROR(3, "spawn pragma " + (x) +                                      \
                      " must be defined before singal "                        \
                      "function call statement")
#define STREAM_ERROR_4(x)                                                      \
  STREAM_ERROR(4, "spawn function " + (x) + " can not have return inside")
#define STREAM_ERROR_5(x)                                                      \
  STREAM_ERROR(                                                                \
      5, "Found merlin_stream variable usage in " + (x) + "\n" +               \
             "merlin_stream variable must be used in function call named:\n"   \
             "merlin_stream_init\n"                                            \
             "merlin_stream_reset\n"                                           \
             "merlin_stream_write\n"                                           \
             "merlin_stream_read\n")
#define STREAM_ERROR_6(x)                                                      \
  STREAM_ERROR(6, "Found spawn kernel interface not from "                     \
                  "original kenrel interface : " +                             \
                      (x) + "\n")
#define STREAM_ERROR_7(x)                                                      \
  STREAM_ERROR(7, "Spawn pragma with parallel factor : " + (x) +               \
                      "\n"                                                     \
                      "must defined inside for loop.")
#define STREAM_ERROR_8(x)                                                      \
  STREAM_ERROR(8, "Spawn pragma with parallel factor : " + (x) +               \
                      "\n"                                                     \
                      "the factor must equal to the loop bound.")

#define STREAM_WARNING(num, msg) MARS_WARNING(STREAM, msg, num)
#define STREAM_WARNING_1(x)                                                    \
  STREAM_WARNING(1, "stream ports cannot be separated:\n    " + (x))
#define STREAM_WARNING_2(x)                                                    \
  STREAM_WARNING(                                                              \
      2, "Streaming port " + (x) +                                             \
             " with multiple accesses may result in QoR "                      \
             "degradation.\n" +                                                \
             "  Hint: please merge multiple accesses into single access")

#define FETCH_WARNING_1(x)                                                     \
  FETCH_WARNING(                                                               \
      1,                                                                       \
      "we do not support multiple access in stream prefetch for variable: " +  \
          (x))
#define FETCH_WARNING(num, msg) MARS_WARNING(FETCH, msg, num)

#define CMOPT_INFO(num, msg) MARS_INFO(CMOPT, msg, num)
#define CMOPT_WARNING(num, msg) MARS_WARNING(CMOPT, msg, num)
#define CMOPT_ERROR(num, msg) MARS_ERROR(CMOPT, msg, num)

#define CMOPT_ERROR_1(x)                                                       \
  CMOPT_ERROR(                                                                 \
      1, "Cannot apply the synchronization on the non-canonical loop " + (x))

#define CMOPT_WARNING_1(x)                                                     \
  CMOPT_WARNING(                                                               \
      1, "Coarse-grained dataflow pipelining cannot be generated because " +   \
             (x) + " is accessed in more than two loops")
#define CMOPT_WARNING_2(x)                                                     \
  CMOPT_WARNING(                                                               \
      2, "Coarse-grained dataflow pipelining cannot be generated because " +   \
             (x) + " is read in more than one loop")
#define CMOPT_WARNING_3(x)                                                     \
  CMOPT_WARNING(                                                               \
      3, "Coarse-grained dataflow pipelining cannot be generated because " +   \
             (x) + " is writen in more than one loop")
#define CMOPT_WARNING_4(x)                                                     \
  CMOPT_WARNING(4, "Coarse-grained dataflow pipelining cannot be generated "   \
                   "becuase of the possible backward dependency " +            \
                       (x))
#define CMOPT_WARNING_5(x)                                                     \
  CMOPT_WARNING(                                                               \
      5, "Coarse-grained dataflow pipelining cannot be generated because " +   \
             (x) + " is accessed multiple times in a loop")
#define CMOPT_WARNING_6(x)                                                     \
  CMOPT_WARNING(6, "Coarse-grained dataflow pipelining cannot be generated "   \
                   "because the indices of array " +                           \
                       (x) + " is too complex")
#define CMOPT_WARNING_7(x)                                                     \
  CMOPT_WARNING(7, "Coarse-grained dataflow pipelining cannot be generated "   \
                   "because the range of the read and write access to " +      \
                       (x) + " is not matched")
#define CMOPT_WARNING_8(x)                                                     \
  CMOPT_WARNING(8, "Coarse-grained dataflow pipelining cannot be generated "   \
                   "because the parallelism of two references to " +           \
                       (x) + " is not matched")
#define CMOPT_WARNING_9(x)                                                     \
  CMOPT_WARNING(9, "Coarse-grained dataflow pipelining cannot be generated "   \
                   "because of synchronization on host interface " +           \
                       (x))
#define CMOPT_WARNING_10(x)                                                    \
  CMOPT_WARNING(10, "Coarse-grained dataflow pipelining cannot be generated "  \
                    "because the index patterns of two references to " +       \
                        (x) + " are not matched")

#define CMOPT_INFO_1(x)                                                        \
  CMOPT_INFO(1, "Setting the variable " + (x) + " into FIFO channel type")

#define MSG_RPT_WARNING_1(x)                                                   \
  MSG_RPT_WARNING(1, "Define varaible " + (x) +                                \
                         " in header file is not a good "                      \
                         "practice")
