#include <stdio.h>
#include <stdint.h>
/*--------------------------------------------------------------------------*/
#define _GCC 1
#define _LOWFREQ 1
/*--------------------------------------------------------------------------*/
#if (_TASKING)
/*--------------------------------------------------------------------------*/
	#define TC_J_A11 __asm("ji a11");
	#define TC_NOP  __nop();
	#define TC_STOP __debug();
	#define TC_SYNC {__asm("dsync");__asm("isync");}
/*--------------------------------------------------------------------------*/
#elif (_GCC)
/*--------------------------------------------------------------------------*/
	#define TC_J_A11 {__asm__ volatile ("ji %a11");}
	#define TC_NOP __asm__ volatile ("nop");
	#define TC_STOP __asm__ volatile ("debug");
	#define TC_SYNC {__asm__ volatile ("dsync");__asm__ volatile ("isync");}
/*--------------------------------------------------------------------------*/
#else
	#define TC_J_A11
	#define TC_NOP
	#define TC_STOP
	#define TC_SYNC
#endif
/*--------------------------------------------------------------------------*/
#define _NULL ((void*)(0))
/*-------------------------------------------------------------------------*/
#define TC_4NOP  {TC_NOP;TC_NOP;TC_NOP;TC_NOP;}
#define _PRIVATE static
#define _INLINE static inline __attribute__((always_inline))
#define _PUBLIC extern
#define U32_REG_READ(addr)      (*((volatile uint32_t*)(addr)))
#define U32_REG_WRITE(addr,val) {U32_REG_READ(addr)=(uint32_t)val;}

#if (_LOWFREQ)
    #define BACON_REG_DL8b_NOT_LAST    (0x53A71FF0u)
    #define BACON_REG_DL8b_LAST        (0x53A71FF1u)
    #define BACON_REG_DL32b_LAST       (0x5FA71FF1u)
    #define BACON_REG_DL32b_NOT_LAST   (0x5FA71FF0u)
#else
    #define BACON_REG_DL8b_NOT_LAST    (0x53A00000u)
    #define BACON_REG_DL8b_LAST        (0x53A00001u)
    #define BACON_REG_DL32b_LAST       (0x5FA00001u)
    #define BACON_REG_DL32b_NOT_LAST   (0x5FA00000u)
#endif


/*--------------------------------------*/
typedef union
{
 uint8_t  u8_au8[8];
 uint16_t u16_au16[4];
 uint32_t u32_au32[2];
 uint64_t u64_au64[1];
}t32_single_longword_t;

typedef union
{
 uint8_t  u8_au8[4];
 uint16_t u16_au16[2];
 uint32_t u32_au32[1];
}t32_single_word_t;

typedef union
{
 uint8_t  u8_au8[2];
 uint16_t u16_au16[1];
}t32_single_halfword_t;
    
typedef struct
{
    t32_single_longword_t start_address;  /*0*/
    t32_single_longword_t user_param1;   /*8*/
    t32_single_longword_t user_param2;  /*16*/
    t32_single_longword_t user_param3;  /*24*/
    t32_single_longword_t reserved;    /*32*/
    t32_single_longword_t data_length; /*40*/
    t32_single_longword_t user_param4; /*48*/
    t32_single_longword_t command; /*56*/
    uint8_t databuffer[1]; /*64  / 0x40 */
}t32_nordrv_parameterblock_t;

typedef union
{
    t32_nordrv_parameterblock_t data;
    uint8_t raw_block_au8[65u];
}T32_NOR_operation_block_t;

__attribute__((section(".data_block")))
static T32_NOR_operation_block_t m_data_block;

_INLINE uint8_t qspi_read_register(uint32_t qspi,uint32_t register_u32);

_INLINE void nor_qspi_write_reg_bacon(uint32_t qspi,uint32_t data) {U32_REG_WRITE(qspi+0x60u,data);/*baconentry*/}
_INLINE void nor_qspi_write_reg_dataentry(uint32_t qspi,uint32_t data){U32_REG_WRITE(qspi+0x64u,data);/*dataentry*/}
_INLINE uint32_t nor_qspi_read_reg_rxexit(uint32_t qspi){return U32_REG_READ(qspi+0x90u);/*rxexit*/}
_INLINE void nor_qspi_clear_status(uint32_t qspi){U32_REG_WRITE(qspi+0x54u,0xFFFFFFFFu); /*clear status*/}
_INLINE void nor_qspi_clear_status_rxtx(uint32_t qspi){U32_REG_WRITE(qspi+0x54u,0x600); /*clear status*/}
_INLINE void nor_qspi_wait_tx_flag(uint32_t qspi)  {do {TC_NOP;}  while((U32_REG_READ(qspi+0x40u)&0x200u)==0u);/*wait until tx*/}
_INLINE void nor_qspi_wait_rx_flag(uint32_t qspi)  {do {TC_NOP;}  while((U32_REG_READ(qspi+0x40u)&0x400u)==0u);/*wait until rx*/}
_INLINE void nor_qspi_wait_txrx_flag(uint32_t qspi){do {TC_NOP;}  while((U32_REG_READ(qspi+0x40u)&0x600u)==0u);/*wait until tx/rx*/}
_INLINE void nor_qspi_wait_for_data(uint32_t qspi) {do {TC_NOP;}  while ((U32_REG_READ(qspi+0x40u)&0x00380000u)==0u);} /*rxfifolevel*/
_INLINE void nor_qspi_wait_tx_done(uint32_t qspi) {do {TC_NOP;}  while ((U32_REG_READ(qspi+0x40u)&0x00070000u)!=0u);} /*txfifolevel*/
_INLINE void nor_qspi_wait_busy(uint32_t qspi) {do {TC_NOP;}  while ((U32_REG_READ(qspi+0x40u)&0xF0000000u)!=0u);} /*PHASE*/
_INLINE void nor_qspi_sr1v_wait_WIP(uint32_t qspi) { do{TC_NOP;}while((qspi_read_register(qspi,0x800000)&0x01)!=0); }
_INLINE void nor_qspi_sr1v_wait_WEL(uint32_t qspi) { do{TC_NOP;}while((qspi_read_register(qspi,0x800000)&0x02)==0); }

_INLINE void nor_qspi_wait_read_data(uint32_t qspi,uint8_t level,uint32_t *data)
{
    uint32_t reg;
    nor_qspi_wait_tx_done(qspi);
    do {TC_NOP;}  while (((U32_REG_READ(qspi+0x40u)>>19)&0x07)<level);
    while(level>0)
    {
        reg=nor_qspi_read_reg_rxexit(qspi);
        if (data!=_NULL){(*data++)=reg;}
        --level;
    }
}

_INLINE void qspi_read_id(uint32_t qspi,uint8_t *manufacturer,uint8_t *id)
{
    uint32_t data;
    nor_qspi_clear_status_rxtx(qspi);
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL32b_LAST);
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,0x9F000000u);
    nor_qspi_wait_read_data(qspi,1,&data);
    nor_qspi_wait_busy(qspi);
    (*id)           = (data>>8);
    (*manufacturer) = (data>>16);
}

_INLINE uint8_t qspi_read_register(uint32_t qspi,uint32_t register_u32)
{
    uint32_t data[2];
    nor_qspi_clear_status_rxtx(qspi);
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL32b_NOT_LAST);
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,((0x65000000u)|(register_u32&0x00FFFFFFu)));
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL32b_LAST);
    nor_qspi_write_reg_dataentry(qspi,0);
    nor_qspi_wait_read_data(qspi,2,&data[0]);
    return ((data[1])&0xFF);
}

_INLINE void qspi_wren(uint32_t qspi)
{
    nor_qspi_clear_status_rxtx(qspi);
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_LAST);
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,0x06);
    nor_qspi_wait_read_data(qspi,1,_NULL);
    nor_qspi_wait_busy(qspi);
    nor_qspi_sr1v_wait_WIP(qspi);
    nor_qspi_sr1v_wait_WEL(qspi);
    
}


_INLINE void nor_spi_transfer_erase_page(uint32_t qspi,uint32_t start_addr,uint32_t len)
{
    uint8_t  opcode = (len > 0x1000u) ? 0xDC : 0x21;

    qspi_wren(qspi);
    nor_qspi_clear_status_rxtx(qspi);

    nor_qspi_write_reg_bacon(qspi, BACON_REG_DL32b_NOT_LAST);
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,(uint32_t)(((uint32_t)opcode << 24)|((uint32_t)(start_addr >> 8) & 0x00FFFFFFu)));
    
    nor_qspi_write_reg_bacon(qspi, BACON_REG_DL8b_LAST);
    nor_qspi_write_reg_dataentry(qspi, (uint8_t)(start_addr & 0xFF));

    nor_qspi_wait_read_data(qspi,2,_NULL);
    nor_qspi_wait_busy(qspi);
    nor_qspi_sr1v_wait_WIP(qspi);
}

_INLINE void nor_spi_transfer_erase(uint32_t qspi,uint32_t start_addr,uint32_t len)
{
    uint32_t i;
    uint32_t opcode_len = (len > 0x1000u) ? 0x40000u : 0x1000u;
    for (i=0;i < len;i+=opcode_len)
    {
        nor_spi_transfer_erase_page(qspi,start_addr+i,opcode_len);
    }
}


_INLINE void nor_spi_transfer_read(uint32_t qspi,uint32_t start_addr,uint32_t len,uint8_t *data)
{
    uint32_t word;
    nor_qspi_clear_status_rxtx(qspi);
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL32b_NOT_LAST);
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,(0x13000000u)|((start_addr>>8u)&0x00FFFFFFu));
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_NOT_LAST);
    nor_qspi_write_reg_dataentry(qspi,(start_addr&0xFF));
    nor_qspi_wait_read_data(qspi,2,_NULL);
    while (len>0)
    {
        if (len==1)nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_LAST);
        nor_qspi_write_reg_dataentry(qspi,0);
        nor_qspi_wait_read_data(qspi,1,&word);
        *(data++)=word;
        len--;
    }
}

_INLINE void nor_spi_transfer_write_page(uint32_t qspi,uint32_t start_addr,uint32_t len,uint8_t *data)
{
    qspi_wren(qspi);

    nor_qspi_clear_status_rxtx(qspi);

    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL32b_NOT_LAST);
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,(0x12000000u)|((start_addr>>8u)&0x00FFFFFFu));
    nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_NOT_LAST);
    nor_qspi_write_reg_dataentry(qspi,(start_addr&0xFF));
    nor_qspi_wait_read_data(qspi,2,_NULL);
    while (len>0)
    {
        if (len==1)nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_LAST);
        nor_qspi_write_reg_dataentry(qspi,(uint8_t)(*(data++)));
        nor_qspi_wait_read_data(qspi,1,_NULL);
        len--;
    }

    nor_qspi_wait_tx_done(qspi);
    nor_qspi_sr1v_wait_WIP(qspi);
}

_INLINE void nor_spi_transfer_write(uint32_t qspi,uint32_t start_addr,uint32_t len,uint8_t *data)
{
    uint32_t i;
    for (i=0;i<len;i+=0x100)
    {
        nor_spi_transfer_write_page(qspi,start_addr+i,(len>0x100)?0x100:len,&data[i]);
    }
}

_INLINE uint8_t qspi_transfer_1byte(uint32_t qspi,uint8_t data)
{
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_write_reg_dataentry(qspi,data);
    nor_qspi_wait_for_data(qspi);
    return (uint8_t)nor_qspi_read_reg_rxexit(qspi);
}

_INLINE void nor_spi_transfer_1b(uint32_t qspi,uint8_t *txbuf,uint8_t *rxbuf,uint32_t txlen,uint32_t rxlen,uint8_t read_dummy_cycles)
{
    uint8_t dummy_u32;
    nor_qspi_clear_status_rxtx(qspi);
    if((txlen>1) || (rxlen>1)){nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_NOT_LAST);}
    while ((txlen>0) || (rxlen>0))
    {
        if ((txlen<=1) && (rxlen<=1)){nor_qspi_write_reg_bacon(qspi,BACON_REG_DL8b_LAST);}
        /*-------------------------------------------------------------------*/
        dummy_u32=qspi_transfer_1byte(qspi,(txlen==0)?0x00:(*(txbuf++)));
        /*-----------------------------------------------------------------*/
        if ((rxlen>0)&&(read_dummy_cycles==0)){(*(rxbuf++))=dummy_u32;(--rxlen);}
        if (txlen>0){(--txlen);}
        if (read_dummy_cycles>0){--read_dummy_cycles;}
    }
    nor_qspi_wait_tx_done(qspi);
    nor_qspi_wait_busy(qspi);
}

__attribute__((section(".main")))
int main(void)
{
    uint32_t qspi,temp1,temp2;
    t32_single_word_t buffer[8];
    void *data = (void *)&m_data_block;
    void *ptr = data;
    
    T32_NOR_operation_block_t *operation = (T32_NOR_operation_block_t*)ptr;
    t32_single_word_t *data_buffer = (t32_single_word_t*)&operation->data.databuffer[0];
    qspi = operation->data.user_param1.u32_au32[0];
    TC_SYNC;
    if (operation->data.command.u8_au8[0]==0x20u) /*FLASHFILE.GETID*/
    {
        qspi_read_id(qspi,&buffer[0].u8_au8[0],&buffer[0].u8_au8[1]);
        data_buffer[0].u16_au16[0]=(buffer[0].u8_au8[0]); /*Manufacturer*/
        data_buffer[0].u16_au16[1]=(buffer[0].u8_au8[1]); /*Device ID*/
        data_buffer[4].u16_au16[0]=0x100;     /*page size*/
        data_buffer[5].u16_au16[0]=0x40;     /*spare size*/
        data_buffer[6].u32_au32[0]=0x40000u; /*block size*/
        data_buffer[7].u16_au16[0]=0x02; /*Flash type - SPI flash*/
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x11u) /*FLASHFILE.SAVE / FLASHFILE.DUMP*/
    {
        nor_spi_transfer_read(qspi,operation->data.start_address.u32_au32[0],operation->data.data_length.u32_au32[0],(uint8_t*)&data_buffer[0]);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x19u) /*FLASHFILE.Erase*/
    {
        nor_spi_transfer_erase(qspi,operation->data.start_address.u32_au32[0],operation->data.data_length.u32_au32[0]);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x13u) /*FLASHFILE.Erase*/
    {
        nor_spi_transfer_erase(qspi,operation->data.start_address.u32_au32[0],operation->data.data_length.u32_au32[0]);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x12u) /*FLASHFILE.Set*/
    {
        nor_spi_transfer_write(qspi,operation->data.start_address.u32_au32[0],operation->data.data_length.u32_au32[0],(uint8_t*)&data_buffer[0]);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x18u) /*FLASHFILE.LOAD*/
    {
        nor_spi_transfer_write(qspi,operation->data.start_address.u32_au32[0],operation->data.data_length.u32_au32[0],(uint8_t*)&data_buffer[0]);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x17u) /*FLASHFILE.DUMP*/
    {
        nor_spi_transfer_read(qspi,operation->data.start_address.u32_au32[0],operation->data.data_length.u32_au32[0],(uint8_t*)&data_buffer[0]);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else if (operation->data.command.u8_au8[0]==0x2Au) /*FLASHFILE.SPI.CMD*/
    {
        temp1=data_buffer[0].u32_au32[0];  /*Rx Len*/
        temp2=data_buffer[1].u32_au32[0]; /*Tx Len*/
        nor_spi_transfer_1b(qspi,(uint8_t*)&data_buffer[8],(uint8_t*)&data_buffer[0],temp2,temp1,temp2);
        operation->data.command.u8_au8[0]=0; /*No errors*/
    }
    else
    {
        ;
    }
    
    TC_4NOP;
    TC_SYNC;
    TC_4NOP;
	TC_J_A11;
    TC_4NOP;
	TC_STOP;
    return 0; // Never reached
}
