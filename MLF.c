/**
 **********************************************************************************
 * @file   MLF.h
 * @author Ali Moallem (https://github.com/AliMoal)
 * @brief  Generate MAHDA log file format (.MLF)
 **********************************************************************************
 */

/* Includes ---------------------------------------------------------------------*/
#include "MLF.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Private Macros ---------------------------------------------------------------*/
#define CheckASSERT(X) if (!X) return;

/* Private Constants ------------------------------------------------------------*/
#define MLF_HEADER_TAG  "MAHDALOGFILE"
#define MLF_LOG_TYPE    0
#define MLF_VERSION     3


/* Private variables ------------------------------------------------------------*/
const uint8_t DataTypeSize[13] =
{
  0,
  sizeof(int8_t),
  sizeof(int16_t),
  sizeof(int32_t),
  sizeof(int64_t),
  sizeof(uint8_t),
  sizeof(uint16_t),
  sizeof(uint32_t),
  sizeof(uint64_t),
  sizeof(float),
  sizeof(double),
  sizeof(uint8_t),
	sizeof(uint32_t)
};

/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
MLF_CopyU32LittleEndian(uint8_t *Dest, uint32_t Value)
{
  Dest[0] = (Value);
  Dest[1] = (Value) >> (8);
  Dest[2] = (Value) >> (16);
  Dest[3] = (Value) >> (24);
}


/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */

/**
 * @brief  Generate header part of MLF file
 * @note   This function must be called at FIRST and ONCE!
 * @param  NumberOfChannels: number of channels
 * @param  ChNames: Pointer to channel names string
 * 				 @note		number of element: number of channels
 * @param	 ChDataTypes: Pointer to channels data types enum
 * 				 @note		number of element: number of channels
 *											 - MLF_ELEMENT_INVALID:  Do not use this data type.
 *                  		 - MLF_ELEMENT_INT8:     int8_t data type
 *                  		 - MLF_ELEMENT_INT16:    int16_t data type
 *                  		 - MLF_ELEMENT_INT32:    int32_t data type
 *                  		 - MLF_ELEMENT_INT64:    int64_t data type
 *                  		 - MLF_ELEMENT_UINT8:    uint8_t data type
 *                  		 - MLF_ELEMENT_UINT16:   uint16_t data type
 *                  		 - MLF_ELEMENT_UINT32:   uint32_t data type
 *                  		 - MLF_ELEMENT_UINT64:   uint64_t data type
 *                  		 - MLF_ELEMENT_FLOAT32:  float data type
 *                  		 - MLF_ELEMENT_FLOAT64:  double data type
 *                  		 - MLF_ELEMENT_BIT:      bit data type. use 1 byte for each bit		
 *											 - MLF_ELEMENT_DATETIME  uint32_t for Time  
 * @param  Buff: Pointer to buffer of MLF file.
 * @param  Size: Bytes count of data stored in Buff
 * // @param  WriteFunction: Pointer to writing function 
 * //	  		@note If you do not want to use, leave it as NULL
 * //				@brief  This is a function to write samples on MLF file
 * //       @param  Buff: Pointer to buffer of MLF file. Null value just calculates buffer size
 * //       @param  Size: bytes count of data stored in Buff
 * @retval None
 */
void
MLF_Init(MLF_Handler *Handler,uint32_t NumberOfChannels, MLF_ChannelName *ChNames, MLF_ChannelDataType *ChDataTypes ,uint8_t *Buff, uint32_t *Size/*,void *WriteFunction(uint8_t *Buff, size_t *Size)*/)
{

//	CheckASSERT(Handler);
//	CheckASSERT(ChNames)
//	CheckASSERT(ChDataTypes);
//  CheckASSERT(Buff);
//	CheckASSERT(Size);
	
	Handler->NumOfCh = NumberOfChannels;
	Handler->ChNumber = 0;
	Handler->ChDataType = ChDataTypes;

	*Size = (uint32_t)Buff;
  strcpy((char *)Buff, MLF_HEADER_TAG);
  Buff += strlen(MLF_HEADER_TAG);
  MLF_CopyU32LittleEndian(Buff, MLF_LOG_TYPE);
  Buff += 4;
  MLF_CopyU32LittleEndian(Buff, MLF_VERSION);
  Buff += 4;
  MLF_CopyU32LittleEndian(Buff, NumberOfChannels);
  Buff += 4;
	
	for (uint32_t counter = 0; counter < NumberOfChannels; counter++)
  {
    MLF_CopyU32LittleEndian(Buff, ChDataTypes[counter]);
		Buff += 1;
  }
	
  for (uint32_t counter = 0; counter < NumberOfChannels; counter++)
  {
    memset(Buff, 0, MAX_CHANNEL_NAME_SIZE + 1);
    strcpy((char *)Buff, (const char *)ChNames[counter]);
    Buff += strlen(ChNames[counter]) + 1; // Plus 1 to add a NULL value because of end of string
  }
	*Size = ((uint32_t)Buff) - (*Size);
}

/**
 * @brief  Add a sample to MLF file 
					 It has to be called for each channel BUT be carefull to call it in order
 * @param  Samples: Pointer to sample
 * @param  Buff: Pointer to buffer of MLF file.
 * @param  Size: bytes count of data stored in Buff.
 * @retval None
 */
void
MLF_AddSample(MLF_Handler *Handler,void *Samples, uint8_t *Buff, uint32_t *Size)
{
	
//	CheckASSERT(Handler); // Comment this line in High Rate
//	CheckASSERT(Samples); // Comment this line in High Rate
//  CheckASSERT(Buff);    // Comment this line in High Rate
//	CheckASSERT(Size);    // Comment this line in High Rate
	
	*Size = DataTypeSize[Handler->ChDataType[Handler->ChNumber]];
	Handler->ChNumber++;
	if ((Handler->ChNumber) >= (Handler->NumOfCh))
		Handler->ChNumber = 0;
	
	if ((*Size) > 0) // 1 byte : int8_t uint8_t
	{
		Buff[0] = ((uint8_t *)Samples)[0];
	}
	if ((*Size) > 1) // 2 bytes : int16_t uint16_t
	{
		Buff[1] = ((uint8_t *)Samples)[1];
	}
	if ((*Size) > 3) // 4 bytes : int32_t uint32_t float
	{
		Buff[2] = ((uint8_t *)Samples)[2];
		Buff[3] = ((uint8_t *)Samples)[3];
	}
	if ((*Size) > 7) // 8 bytes : int64_t uint64_t double
	{
		Buff[4] = ((uint8_t *)Samples)[4];
		Buff[5] = ((uint8_t *)Samples)[5];
		Buff[6] = ((uint8_t *)Samples)[6];
		Buff[7] = ((uint8_t *)Samples)[7];
	}
//	memcpy(Buff,Samples,*Size);
}

