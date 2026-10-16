#ifndef __OTTUNITY_CFG_DFLT_H
#define __OTTUNITY_CFG_DFLT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OTTUNITY_CONFIG_FILE
  #include OTTUNITY_CONFIG_FILE
#elif defined(__has_include)
  #if __has_include("ottunity_config.h")
    #include "ottunity_config.h"
  #endif
#endif

#ifndef OTTUNITY_SUITE_GROUP_NUM_MAX
#define OTTUNITY_SUITE_GROUP_NUM_MAX 256
#endif

#ifndef OTTUNITY_GROUP_TEST_NUM_MAX
#define OTTUNITY_GROUP_TEST_NUM_MAX 256
#endif

#ifndef OTTUNITY_ANY_STR_NAME_MAX_LEN
#define OTTUNITY_ANY_STR_NAME_MAX_LEN 255
#endif

#ifndef OTTUNITY_ENV_SETUP
#define OTTUNITY_ENV_SETUP()
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OTTUNITY_CFG_DFLT_H */
