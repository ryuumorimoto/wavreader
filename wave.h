#ifndef WAVE_H_
#define WAVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#define INTBYTE 4
#define SHORTBYTE 2


typedef struct{
  int fs;//サンプリング周波数
  int bits;//量子化ビット数
  int length;//データ長
  double *s;//音データ
} MONO_PCM;
  //モノラル音声データ

typedef struct{
  char chunkID[4];
  int chunkSize;
  char chunkFormType[4];
} RIFF_CHUNK;
 //RIFFチャンク

typedef struct{
  char chunkID[4];
  int chunkSize;
  short waveFormatType;
  short formatChannel;
  long samplesPerSec;
  long bytesPerSec;
  short blockSize;
  short bitsPerSample;
} FMT_CHUNK;

typedef struct{
  char chunkID[4];
  long chunkSize;
  short data;
} DATA_CHUNK;

typedef struct{
  RIFF_CHUNK riffChunk;
  FMT_CHUNK fmtChunk;
  DATA_CHUNK dataChunk;
} WAVE_FORMAT;

int monoWaveRead(MONO_PCM *pcm,char *filename);
int monoWaveWrite(MONO_PCM *pcm,char *filename);
void freepcm(MONO_PCM* pcm);

#endif
  


