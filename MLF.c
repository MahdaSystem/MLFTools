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

//* Private Includes -------------------------------------------------------------- //
#include "MLF.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//* Private Macros ---------------------------------------------------------------*/
#define CheckASSERT(X) if (!X) return;                                                     // If the pointer (X) is not present, The function will be ended
#define leapYear(year) (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 1 : 0)   // Determines whether or not a year is leap year

//* Private defines --------------------------------------------------------------*/
#define MLF_HEADER_TAG  "MAHDALOGFILE" // Header Tag
#define MLF_VERSION     4              // MLF API Version
#define MLF_LOG_TYPE    1              // General Purpose Log
#define MLF_FLAG_ENDIAN 0              // 0: Little Endian | 1: Big Endian

// For using MLF_TimeSecond function (Recommended to use Unix time: 1970/1/1)
#define MLF_BaseYear   1970            // ...
#define MLF_BaseMonth  1               // 1 - 12
#define MLF_BaseDay    1               // 1 - ...

/**
 ** ==================================================================================
 **                      ##### Private constant Variables #####
 ** ==================================================================================
 **/

/**
 * @brief  Keeps the size of each data types
 * @retval None
 */
static const uint8_t DataTypeSize[13] =
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
  sizeof(uint64_t)
};

/**
 * @brief  Specifies the number of days in the month
 *         daysPerMonth[0][]: non leap year
 *         daysPerMonth[1][]: leap year
 */
static const int8_t
daysPerMonth[2][13] =
{
  {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {-1, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/**
 *! ==================================================================================
 *!                          ##### Private Functions #####
 *! ==================================================================================
 **/

static void
MLF_CopyU32LittleEndian(uint8_t *Dest, uint32_t Value)
{
  Dest[0] = (Value);
  Dest[1] = (Value) >> (8);
  Dest[2] = (Value) >> (16);
  Dest[3] = (Value) >> (24);
}

static uint32_t
MLF_DateDef(uint8_t Day, uint8_t Month, uint16_t Year)
{
  int16_t y, m;
  uint32_t total_days = 0;

  if (Year < MLF_BaseYear)
    return 0;

  for (y = Year; y >= MLF_BaseYear; y--)
  {
    if (y == Year)
    {
      for (m = Month; m >= 1; m--)
      {
        if (m == Month)
          total_days += Day;
        else
          total_days += daysPerMonth[leapYear(y)][m];
      }
    }
    else if (y == MLF_BaseYear)
    {
      for (m = 12; m >= MLF_BaseMonth; m--)
      {
        if (m == MLF_BaseMonth)
          total_days += daysPerMonth[leapYear(y)][m] - MLF_BaseDay;
        else
          total_days += daysPerMonth[leapYear(y)][m];
      }
    }
    else
    {
      for (m = 12; m >= 1; m--)
      {
        total_days += daysPerMonth[leapYear(y)][m];
      }
    }
  }

  return total_days;
}

/**
 ** ==================================================================================
 **                           ##### Public Functions #####
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
 * @param  Size: Bytes count of data stored in Buff
 * !NOT IMPLEMENTED! @param  WriteFunction: Pointer to writing function
 * !NOT IMPLEMENTED! @note   If you do not want to use, leave it as NULL
 * !NOT IMPLEMENTED! @brief  This is a function to write samples on MLF file
 * !NOT IMPLEMENTED!         @param  Buff: Pointer to buffer of MLF file. Null value just calculates buffer size
 * !NOT IMPLEMENTED!         @param  Size: bytes count of data stored in Buff
 * @retval None
 */
void
MLF_Init(MLF_Handler_t *Handler, uint32_t NumberOfChannels, MLF_ChannelName_t *ChNames, MLF_ChannelDataType_t *ChDataTypes, uint8_t *Buff, uint32_t *Size /*,void *WriteFunction(uint8_t *Buff, size_t *Size)*/)
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
  // MLF Header Tag ("MAHDALOGFILE" is 12 Bytes)
  strcpy((char *)Buff, MLF_HEADER_TAG);
  Buff += strlen(MLF_HEADER_TAG);
  // Version
  MLF_CopyU32LittleEndian(Buff, MLF_VERSION);
  Buff += 4;
  // Log Type
  MLF_CopyU32LittleEndian(Buff, MLF_LOG_TYPE);
  Buff += 4;
  // Flags (for LittleBig Endian Bit: 0)
  MLF_CopyU32LittleEndian(Buff, 0 | MLF_FLAG_ENDIAN);
  Buff += 4;
  // Number Of Channels
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
    if (strlen(ChNames[counter]) <= MAX_CHANNEL_NAME_SIZE)
    {
      strcpy((char *)Buff, (const char *)ChNames[counter]);
      Buff += strlen(ChNames[counter]) + 1; // Plus 1 to add a NULL value because of end of string
    }
    else
    {
      memcpy((char *)Buff, (const char *)ChNames[counter], MAX_CHANNEL_NAME_SIZE);
      Buff += MAX_CHANNEL_NAME_SIZE + 1; // Plus 1 to add a NULL value because of end of string
    }
  }

  *Size = ((uint32_t)Buff) - (*Size);
}

/**
 * @brief  Adds a sample to MLF file
 * @note	 It has to be called for each channel BUT be carefull to call it in order
 * @param  Samples: Pointer to sample
 * @param  Buff: Pointer to buffer of MLF file.
 * @param  Size: bytes count of data stored in Buff.
 * @retval None
 */
void
MLF_AddSample(MLF_Handler_t *Handler, void *Samples, uint8_t *Buff, uint32_t *Size)
{

  //	CheckASSERT(Handler); // Comment this line in High Rate
  //	CheckASSERT(Samples); // Comment this line in High Rate
  //  CheckASSERT(Buff);    // Comment this line in High Rate
  //	CheckASSERT(Size);    // Comment this line in High Rate

  *Size = DataTypeSize[Handler->ChDataType[Handler->ChNumber]];
  if (Handler->ChDataType[Handler->ChNumber] == MLF_ELEMENT_DATETIME)
  {
    Buff[0] =  ((MLF_DateTime_t *)Samples)->Fraction & 0xFF;
    Buff[1] =  (((MLF_DateTime_t *)Samples)->Fraction >> 8) & 0xFF;
    Buff[2] =  (((MLF_DateTime_t *)Samples)->Fraction >> 16) & 0xFF;
    Buff[3] =  (((MLF_DateTime_t *)Samples)->Fraction >> 24) & 0x3F;
    Buff[3] |= (((MLF_DateTime_t *)Samples)->Second << 6) & 0xC0;
    Buff[4] =  (((MLF_DateTime_t *)Samples)->Second >> 2) & 0xFF;
    Buff[5] =  (((MLF_DateTime_t *)Samples)->Second >> 10) & 0xFF;
    Buff[6] =  (((MLF_DateTime_t *)Samples)->Second >> 18) & 0xFF;
    Buff[7] =  (((MLF_DateTime_t *)Samples)->Second >> 26) & 0xFF;
    Handler->ChNumber++;
    if ((Handler->ChNumber) >= (Handler->NumOfCh))
      Handler->ChNumber = 0;
    return;
  }

  Handler->ChNumber++;
  if ((Handler->ChNumber) >= (Handler->NumOfCh))
    Handler->ChNumber = 0;

  if ((*Size) > 0) // 1 byte : int8_t uint8_t bool
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
MLF_TimeSecond(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second)
{
  uint32_t DiffDay;
  int64_t DiffSecond;

  DiffDay = MLF_DateDef(Day, Month, Year);
  DiffSecond  = (int64_t)(DiffDay * 86400ul);
  DiffSecond += (int64_t)(Hour * 3600);
  DiffSecond += (int64_t)(Minute * 60);
  DiffSecond += (int64_t)(Second);

  return DiffSecond;
}
