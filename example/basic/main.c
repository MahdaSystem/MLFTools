/**
 **********************************************************************************
 * @file   MLF_Config.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  Basic example for MLF library
 **********************************************************************************
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "MLF.h"


int main(void)
{
  printf("MLF Library test\n");

  MLF_Handler_t FileObject = {0};
  MLF_ChannelName_t MLF_ChannelName[3] = {0};
  MLF_ChannelDataType_t ChannelsDataType[3] = {0};

  uint8_t *Buffer = NULL;
  uint32_t Size = 500;
  FILE *f = NULL;

  // Allocate memory for buffer
  Buffer = calloc(Size + 1, sizeof(uint8_t));
  if (Buffer == NULL)
  {
    fprintf(stderr, "Memory allocation failed. size: %lu Byte", Size + 1);
    return 1;
  }

  const char *FilePath = "./build/test.mlf";
  f = fopen(FilePath, "wb+");
  if (f == NULL)
  {
    fprintf(stderr, "File open failed. Path: %s", FilePath);
    return -1;
  }

  // Initialize MLF channels
  ChannelsDataType[0] = MLF_ELEMENT_DATETIME;
  strcpy(MLF_ChannelName[0], "Channel Time");

  ChannelsDataType[1] = MLF_ELEMENT_INT32;
  strcpy(MLF_ChannelName[1], "Channel INT32");

  ChannelsDataType[2] = MLF_ELEMENT_FLOAT32;
  strcpy(MLF_ChannelName[2], "Channel FLOAT32");

  // Initialize MLF file
  MLF_Init(&FileObject, 3,
           MLF_ChannelName, ChannelsDataType,
           Buffer, &Size);
  fwrite(Buffer, 1, Size, f);


  // Write data to Time channel
  struct timeval now = {0};
  struct tm *tm = NULL;
  MLF_DateTime_t ChannelTimeData = {0};
  gettimeofday(&now, NULL);
  tm = localtime(&now.tv_sec);
  ChannelTimeData.Second = MLF_TimeSecond(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                                          tm->tm_hour, tm->tm_min, tm->tm_sec);
  ChannelTimeData.Fraction = now.tv_usec;
  MLF_AddSample(&FileObject, &ChannelTimeData, Buffer, &Size);
  fwrite(Buffer, 1, Size, f);

  // Write data to INT32 channel
  int32_t ChannelINT32Data = 123456;
  MLF_AddSample(&FileObject, &ChannelINT32Data, Buffer, &Size);
  fwrite(Buffer, 1, Size, f);

  // Write data to FLOAT32 channel
  float ChannelFLOAT32Data = 123.456;
  MLF_AddSample(&FileObject, &ChannelFLOAT32Data, Buffer, &Size);
  fwrite(Buffer, 1, Size, f);

  // Close MLF file
  fclose(f);

  return 0;
}