#pragma once

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <stdio.h>
#include <string.h>
#include <string_view>
#include <stdexcept>

#define U32_MAX (0xFFFFFFFFU)

#define _ASSERT_(x) {if((x)==0){_DBG_MSG_("_ASSERT_FAILED_!");do{}while(1);}}
#define _DBG_MSG_(x) {printf("[%s:%s:%u]=> %s\n",__FILE__,__FUNCTION__,__LINE__,std::string(x).c_str());}
#define _NULL_PTR_ (nullptr)
#define _MEMCPY_(dst,src,len){memcpy(dst,src,len);}
#define _MEMSET_(dst,val,len){memset(dst,val,len);}
#define _STRLEN_(s) (s==nullptr?0:strlen((char*)s))
#define _SIZEOF_(s) (sizeof(s))

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

typedef char S8;
typedef short S16;
typedef int S32;

typedef float F32;
typedef double F64;

