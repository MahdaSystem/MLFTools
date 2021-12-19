/**
 **********************************************************************************
 * @file   MLF.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 *         Ali Moallem (https://github.com/AliMoal)
 * @brief  Generate MAHDA log file format (.MLF)
 * @note   Use "Better comments" extension for VSCode to get the best details
 *         ExtensionID: aaron-bond.better-comments
 **********************************************************************************
 */

//* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _MLF_H_
#define _MLF_H_

#ifdef __cplusplus
extern "C"
{
#endif

//* Includes ---------------------------------------------------------------------*/
#include <stdint.h>

//? User Configurations and Notes ------------------------------------------------- //
#define MAX_CHANNEL_NAME_SIZE 20 // Maximum number of channel name size
//? ------------------------------------------------------------------------------- //

/**
 ** ==================================================================================
 **                           ##### Public Typedef #####
 ** ==================================================================================
 **/
/**
 * @brief  MLF channel name
 */
typedef char
MLF_ChannelName_t[MAX_CHANNEL_NAME_SIZE + 1];
/**
 ** ==================================================================================
 **                                ##### Enums #####
 ** ==================================================================================
 **/
/**
 * @brief  MLF channel data type
 */
typedef enum
{
  MLF_ELEMENT_INVALID = 0, // Do not use this data type.
  MLF_ELEMENT_INT8,        // int8_t data type
  MLF_ELEMENT_INT16,       // int16_t data type
  MLF_ELEMENT_INT32,       // int32_t data type
  MLF_ELEMENT_INT64,       // int64_t data type
  MLF_ELEMENT_UINT8,       // uint8_t data type
  MLF_ELEMENT_UINT16,      // uint16_t data type
  MLF_ELEMENT_UINT32,      // uint32_t data type
  MLF_ELEMENT_UINT64,      // uint64_t data type
  MLF_ELEMENT_FLOAT32,     // float data type
  MLF_ELEMENT_FLOAT64,     // double data type
  MLF_ELEMENT_BOOL,        // bit data type. use 1 byte for each bit
  MLF_ELEMENT_DATETIME     // MLF_DateTime data type
} MLF_ChannelDataType_t;
/**
 ** ==================================================================================
 **                               ##### Structs #####
 ** ==================================================================================
 **/
/**
 * @brief  MLF file handler
 * @note   This struct will be filled in MLF_Init function
 */
typedef struct
{
  uint32_t NumOfCh;                  // !!! DO NOT USE OR EDIT THIS !!!
  uint32_t ChNumber;                 // !!! DO NOT USE OR EDIT THIS !!!
  MLF_ChannelDataType_t *ChDataType; // !!! DO NOT USE OR EDIT THIS !!!
} MLF_Handler_t;
/**
 ** ==================================================================================
 **                               ##### Unions #####
 ** ==================================================================================
 **/
/**
 * @brief  MLF date and time struct
 */
typedef struct
{
  uint32_t Fraction; // 30 Bit | positive fractions (10e6) of a second
  uint64_t Second;   // 34 Bit | date and time in second from 1970/1/1 to NOW, It can be calculated by MLF_TimeSecond function
} MLF_DateTime_t;
/**
 ** ==================================================================================
 **                          ##### Public Functions #####
 ** ==================================================================================
 **/
/**
 * @brief  Generates header part of MLF file
 * @note   This function must be called at FIRST and ONCE!
 * @param  NumberOfChannels: number of channels
 * @param  ChNames: Pointer to channel names string, See MLF_ChannelName_t struct.
 * 				 @note		number of element: number of channels
 * @param	 ChDataTypes: Pointer to channels data types enum, See MLF_ChannelDataType_t struct.
 * 				 @note		number of element: number of channels //! USER MUST NOT EDIT THIS DURING PROGRAM
 * @param  Buff: Pointer to buffer of MLF file.
 *         @note    To get the max number of data to be written, pass Buff as NULL
 * @param  Size: Bytes count of data stored in Buff
 * !NOT IMPLEMENTED! @param  WriteFunction: Pointer to writing function
 * !NOT IMPLEMENTED! @note   If you do not want to use, leave it as NULL
 * !NOT IMPLEMENTED! @brief  This is a function to write samples on MLF file
 * !NOT IMPLEMENTED!         @param  Buff: Pointer to buffer of MLF file. Null value just calculates buffer size
 * !NOT IMPLEMENTED!         @param  Size: bytes count of data stored in Buff
 * @retval None
 */
void
MLF_Init(MLF_Handler_t *Handler, uint32_t NumberOfChannels, MLF_ChannelName_t *ChNames, MLF_ChannelDataType_t *ChDataTypes, uint8_t *Buff, uint32_t *Size /*,void *WriteFunction(uint8_t *Buff, uint16_t *Size)*/);
/**
 * @brief  Adds a sample to MLF file
 * @note	 It has to be called for each channel BUT be carefull to call it in order
 * @param  Samples: Pointer to sample
 * @param  Buff: Pointer to buffer of MLF file.
 *         @note    To get the max number of data to be written, pass Buff as NULL
 * @param  Size: bytes count of data stored in Buff.
 * @retval None
 */
void
MLF_AddSample(MLF_Handler_t *Handler, void *Samples, uint8_t *Buff, uint32_t *Size);
/**
 * @brief  Calculates "second" part of MLF_DateTime_t from normal time and date
 * @param  Year: Normal Year (1970 to ...)
 * @param  Month: Normal Month (1 to 12)
 * @param  Day: Normal Day (1 to 31)
 * @param  Hour: Normal Hour (0 to 23)
 * @param  Minute: Normal Minute (0 to 59)
 * @param  Second: Normal Second (0 to 59)
 * @retval "Second" part of MLF_DateTime_t
 */
uint64_t
MLF_TimeSecond(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second);

#ifdef __cplusplus
}
#endif

#endif
