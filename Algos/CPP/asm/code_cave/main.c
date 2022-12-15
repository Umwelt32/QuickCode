      /*---------------------------------------------------------*/
     /* Author: https://github.com/Umwelt32/QuickCode           */
    /* Copyright: 2022                                         */
   /* Code incjection                                         */
  /* References:                                             */
 /* https://en.wikipedia.org/wiki/Code_cave                 */
/*---------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>
#include <stdint.h>
#include <inttypes.h>

/*https://defuse.ca/online-x86-assembler.htm*/

  /*----------------------------------------------*/
 /*--------------Constants-----------------------*/
/*----------------------------------------------*/
/*--------------------------*/
#define NULL_PTR ((void*)0u)
#define E_OK     (0u)
#define E_NOT_OK (1u)
  /*----------------------------------------------*/
 /*--------------Macros--------------------------*/
/*----------------------------------------------*/
/*--Memory operations--*/
#define TS_MEMCPY(d,s,l)    {(void)memcpy(d,s,l);}
#define TS_MEMSET(d,v,l)    {(void)memset(d,v,l);}
#define TS_SIZE_IN_BYTES(x) (sizeof(x))
/*--Memory operations--*/
#define TS_split4B(x) (((x)>>0)&0xFFU),(((x)>>8)&0xFFU),((((x)>>16)&0xFFU)),((((x)>>24)&0xFFU))
#define TS_OPCODE4B(y,x) (y),TS_split4B(x)
/*--OP CODES--*/
#define OP_x86_NOP()     (0x90U)
#define OP_x86_JMP_R(x)  TS_OPCODE4B(0xE9U,x)
/*--ASM--*/
#define ASM_NOP() {;/*__asm("NOP");*/}
  /*----------------------------------------------*/
 /*--------------Functions-----------------------*/
/*----------------------------------------------*/
/*-----------Local-functions-------------------*/
static uint8_t codeInject_writeProgramCode(void *dst,void *src,void *old_data,uint32_t len);
static uint8_t codeInject_retourFunction(void *_from,void *_to);
/*-----------functions----------------------*/
void test_1(void);
void test_2(void);

int main(void)
{
    /*Call test_2 function*/
    test_2();
    /*retour test_2 function to test_1 function*/
    (void)codeInject_retourFunction(&test_2,&test_1);
    test_2();
    while (1)
    {
        /*asm just for some code injection*/
        /*endless loop*/
        ASM_NOP();
        ASM_NOP();
        ASM_NOP();
    }
    return 0;
}

void test_1(void){printf("test_1\n");}
void test_2(void){printf("test_2\n");}

static uint8_t codeInject_writeProgramCode(void *dst,void *src,void *old_data,uint32_t len)
{
    uint8_t result_u8 = E_OK;
    DWORD old_protection = 0;
    HANDLE hProcess = 0;
    /*cache old program memory to RAM*/
    if(old_data!=NULL_PTR){TS_MEMCPY(old_data,dst,len);}
    hProcess = GetCurrentProcess();
    /*Make program memory writable*/
    result_u8|=(!VirtualProtectEx(hProcess, dst, len, PAGE_EXECUTE_READWRITE, &old_protection));
    /*Inject new program code*/
    TS_MEMCPY(dst,src,len);
    /*Make program memory not writtable*/
    result_u8|=(!VirtualProtectEx(hProcess, dst, len, old_protection,         &old_protection));
    return result_u8;
}

static uint8_t codeInject_retourFunction(void *_from,void *_to)
{
    uint8_t result_u8 = E_OK;
    uint32_t target_u32    = (uint32_t)_to;
    uint32_t reference_u32 = (uint32_t)_from;
    /*calculate 32bit jump offset (target-current-jump_op_code_size)*/
    int32_t  jmp_rel       = (target_u32-reference_u32)-5;
    /*build asm block*/
    uint8_t  _codebuffer[] = {OP_x86_JMP_R(jmp_rel)};
    /*inject asm block*/
    result_u8=codeInject_writeProgramCode(_from,&_codebuffer[0],NULL_PTR,TS_SIZE_IN_BYTES(_codebuffer));
    return result_u8;
}

