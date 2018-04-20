#ifndef STAT_MAKECONFIG_H
#define STAT_MAKECONFIG_H
#include "ttDM/stat/interface/Config.h"
#include <memory>
#include <cstring>

inline std::shared_ptr<Config> makeConfig() {
  char arg0[] = "test";
  char arg1[] = "-d";
  char arg2[] = "test/data.txt";
  char arg3[] = "-D";
  char arg4[] = "unitTest";
  char * argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0] };
  int argc = sizeof(argv)/sizeof(argv[0]);
  return std::make_shared<Config>(argc, argv);
}

inline std::shared_ptr<Config> makeConfigMCStat() {
  char arg0[] = "test";
  char arg1[] = "-d";
  char arg2[] = "test/data_mcstat.txt";
  char arg3[] = "-D";
  char arg4[] = "unitTest";
  char * argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0] };
  int argc = sizeof(argv)/sizeof(argv[0]);
  return std::make_shared<Config>(argc, argv);
}

inline std::shared_ptr<Config> makeConfigMCStatComplete() {
  char arg0[] = "test";
  char arg1[] = "-d";
  char arg2[] = "test/data_mcstat_complete.txt";
  char arg3[] = "-D";
  char arg4[] = "unitTest";
  char * argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0] };
  int argc = sizeof(argv)/sizeof(argv[0]);
  return std::make_shared<Config>(argc, argv);
}

inline std::shared_ptr<Config> makeConfigMultiSig() {
  char arg0[] = "test";
  char arg1[] = "-d";
  char arg2[] = "test/data_multiSig.txt";
  char arg3[] = "-D";
  char arg4[] = "unitTest";
  char * argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0] };
  int argc = sizeof(argv)/sizeof(argv[0]);
  return std::make_shared<Config>(argc, argv);
}
inline std::shared_ptr<Config> makeWrongConfig() {
  char arg0[] = "test";
  char arg1[] = "-d";
  char arg2[] = "test/pincopallino.txt";
  char arg3[] = "-D";
  char arg4[] = "unitTest";
  char * argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0] };
  int argc = sizeof(argv)/sizeof(argv[0]);
  return std::make_shared<Config>(argc, argv);
}


#endif
