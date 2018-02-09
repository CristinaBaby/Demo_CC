//
//  ActionMacro.h
//
//
//

#ifndef ActionMacro_H
#define ActionMacro_H

#define CombActionNamespace CombActionNS
#ifdef __cplusplus
#define NS_CombAction_BEGIN                     namespace CombActionNS {
#define NS_CombAction_END                       }
#define US_NS_CombActionNS                     using namespace CombActionNS
#else
#define NS_ST_BEGIN
#define NS_ST_END
#define USING_NS_ST
#endif

#endif