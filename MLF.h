/**
 **********************************************************************************
 * @file   MLF.h
 * @author Ali Moallem (https://github.com/alimoal)
 * @brief  Generate MAHDA log file format (.MLF)
 **********************************************************************************
 */

/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _MLF_H_
#define _MLF_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>

	/* Exported Data Types ----------------------------------------------------------*/

	/**
 * @brief  MLF channel name data type
 */
	typedef char MLF_ChannelName_t[21];

	typedef enum
	{
		MLF_ELEMENT_INVALID = 0,
		MLF_ELEMENT_INT8,
		MLF_ELEMENT_INT16,
		MLF_ELEMENT_INT32,
		MLF_ELEMENT_INT64,
		MLF_ELEMENT_UINT8,
		MLF_ELEMENT_UINT16,
		MLF_ELEMENT_UINT32,
		MLF_ELEMENT_UINT64,
		MLF_ELEMENT_FLOAT32,
		MLF_ELEMENT_FLOAT64,
		MLF_ELEMENT_BIT,
		MLF_ELEMENT_DATETIME
	} MLF_ChannelDataType;

	/**
 * @brief  MLF file handler data type
 */
	typedef struct
	{
		uint32_t NumOfCh;
		uint32_t ChNumber;
		MLF_ChannelDataType *ChDataType;
	} MLF_Handler_t;

	/* Exported Constants -----------------------------------------------------------*/

	/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

	/**
 * @brief  Generate header part of MLF file
 * @note   This function must be called at FIRST and ONCE!
 * @param  NumberOfChannels: number of channels
 * @param  ChNames: Pointer to channel names string
 * 				 @note		number of element: number of channels
 * @param  ChDataTypes: Pointer to channels data types enum
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
 * @param  Buff: Pointer to buffer of MLF file.
 * @param  Size: Bytes count of data stored in Buff
 * // @param  WriteFunction: Pointer to writing function 
 * //	  		@note If you don not want to use, leave it as NULL
 * //				@brief  This is a function to write samples on MLF file
 * //       @param  Buff: Pointer to buffer of MLF file. Null value just calculates buffer size
 * //       @param  Size: bytes count of data stored in Buff
 * @retval None
 */
	void
	MLF_Init(MLF_Handler_t *Handler, uint32_t NumberOfChannels, MLF_ChannelName_t *ChNames, MLF_ChannelDataType *ChDataTypes, uint8_t *Buff, uint32_t *Size /*,void *WriteFunction(uint8_t *Buff, uint16_t *Size)*/);

	/**
 * @brief  Add a sample to MLF file 
					 It has to be called for each channel BUT be carefull to call it in order
 * @param  Samples: Pointer to sample
 * @param  Buff: Pointer to buffer of MLF file.
					 @note If you are using WriteFunction you can leave it as NULL
 * @param  Size: bytes count of data stored in Buff.
					 @note If you are using WriteFunction you can leave it as NULL
 * @retval None
 */
	void
	MLF_AddSample(MLF_Handler_t *Handler, void *Samples, uint8_t *Buff, uint32_t *Size);

#ifdef __cplusplus
}
#endif

#endif
