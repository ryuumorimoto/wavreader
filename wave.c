#include "wave.h"



int monoWaveRead(MONO_PCM *pcm,char *filename){
  FILE *fp;
  WAVE_FORMAT waveFormat;
  if((fp=fopen(filename,"rb"))==NULL){
    printf("can't open file\n");
    return -1;
  }
  //RIFF chunk
  fread(waveFormat.riffChunk.chunkID,1,4,fp);
  fread(&waveFormat.riffChunk.chunkSize, 4,1, fp);
  fread(waveFormat.riffChunk.chunkFormType, 1, 4, fp);
  //fmt chunk
  fread(waveFormat.fmtChunk.chunkID, 1, 4, fp);
  fread(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
  fread(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
  fread(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
  fread(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
  fread(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
  fread(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
  fread(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);
  //data chunk
  fread(waveFormat.dataChunk.chunkID, 1, 4, fp);
  fread(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

  pcm->fs=waveFormat.fmtChunk.samplesPerSec;
  pcm->bits = waveFormat.fmtChunk.bitsPerSample;
  pcm->length = waveFormat.dataChunk.chunkSize;
  if(pcm->bits==16){
    pcm->s =(double*)malloc(sizeof(double)*waveFormat.dataChunk.chunkSize/SHORTBYTE);
  }else if(pcm->bits==32){
    pcm->s=(double*)malloc(sizeof(double)*waveFormat.dataChunk.chunkSize/INTBYTE);
  }
 
  //debug
  
  printf("RIFF ID %s\n",waveFormat.riffChunk.chunkID);
  printf("RIFF size %d\n",waveFormat.riffChunk.chunkSize);
  printf("RIFF chunkFormType %s\n",waveFormat.riffChunk.chunkFormType);

  printf("Fmt ID %s\n",waveFormat.fmtChunk.chunkID);
  printf("Fmt chunkSize %d\n",waveFormat.fmtChunk.chunkSize);
  printf("Fmt waveformat %d\n",waveFormat.fmtChunk.waveFormatType);
  printf("Fmt formatchannel %d\n",waveFormat.fmtChunk.formatChannel);
  printf("Fmt fs %ld\n",waveFormat.fmtChunk.samplesPerSec);
  printf("Fmt bytesPerSec %ld\n",waveFormat.fmtChunk.bytesPerSec);
  printf("Fmt blocksize %d\n",waveFormat.fmtChunk.blockSize);
  printf("Fmt bitsPerSample %d\n",waveFormat.fmtChunk.bitsPerSample);

  printf("data ID %s\n",waveFormat.dataChunk.chunkID);
  printf("data chunkSize %ld\n",waveFormat.dataChunk.chunkSize);
  
  if(waveFormat.fmtChunk.formatChannel==2){
    printf("stereo\n");
    return -1;
  }
  if(pcm->bits==16){
    short* tmp=(short*)malloc(pcm->length);
    fread(tmp,sizeof(short),pcm->length/SHORTBYTE,fp);
    for(int i=0;i<(pcm->length/SHORTBYTE);i++){
      pcm->s[i]=(double)tmp[i]/SHRT_MAX;
    }
    free(tmp);
  }else if(pcm->bits==32){
    int* tmp=(int*)malloc(pcm->length);
    fread(tmp,sizeof(int),pcm->length/INTBYTE,fp);
    for(int i=0;i<(pcm->length/INTBYTE);i++){
      pcm->s[i]=(double)tmp[i]/INT32_MAX;
    }
    free(tmp);
  }
  
  fclose(fp);
  return 0;
}

 int monoWaveWrite(MONO_PCM *pcm,char *filename){
  FILE *fp;
  WAVE_FORMAT waveFormat;

  strcpy(waveFormat.riffChunk.chunkID,"RIFF");
  waveFormat.riffChunk.chunkSize=36+pcm->length;
  strcpy(waveFormat.riffChunk.chunkFormType,"WAVE");

  strcpy(waveFormat.fmtChunk.chunkID,"fmt ");
  waveFormat.fmtChunk.chunkSize=16;
  waveFormat.fmtChunk.waveFormatType=1;
  waveFormat.fmtChunk.formatChannel=1;
  waveFormat.fmtChunk.samplesPerSec=pcm->fs;
  waveFormat.fmtChunk.bytesPerSec=(pcm->fs*pcm->bits)/8;
  waveFormat.fmtChunk.blockSize=pcm->bits/8;
  waveFormat.fmtChunk.bitsPerSample=pcm->bits;

  strcpy(waveFormat.dataChunk.chunkID,"data");
  waveFormat.dataChunk.chunkSize=pcm->length;
  //debug
  
  printf("RIFF ID %s\n",waveFormat.riffChunk.chunkID);
  printf("RIFF size %d\n",waveFormat.riffChunk.chunkSize);
  printf("RIFF chunkFormType %s\n",waveFormat.riffChunk.chunkFormType);

  printf("Fmt ID %s\n",waveFormat.fmtChunk.chunkID);
  printf("Fmt chunkSize %d\n",waveFormat.fmtChunk.chunkSize);
  printf("Fmt waveformat %d\n",waveFormat.fmtChunk.waveFormatType);
  printf("Fmt formatchannel %d\n",waveFormat.fmtChunk.formatChannel);
  printf("Fmt fs %ld\n",waveFormat.fmtChunk.samplesPerSec);
  printf("Fmt bytesPerSec %ld\n",waveFormat.fmtChunk.bytesPerSec);
  printf("Fmt blocksize %d\n",waveFormat.fmtChunk.blockSize);
  printf("Fmt bitsPerSample %d\n",waveFormat.fmtChunk.bitsPerSample);

  printf("data ID %s\n",waveFormat.dataChunk.chunkID);
  printf("data chunkSize %ld\n",waveFormat.dataChunk.chunkSize);

  if((fp=fopen(filename,"wb"))==NULL){
    printf("can't open file");
    return -1;
  }

  fwrite(waveFormat.riffChunk.chunkID, 1, 4, fp);
  fwrite(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
  fwrite(waveFormat.riffChunk.chunkFormType, 1, 4, fp);

  fwrite(waveFormat.fmtChunk.chunkID, 1, 4, fp);
  fwrite(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
  fwrite(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
  fwrite(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
  fwrite(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
  fwrite(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
  fwrite(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
  fwrite(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);

  fwrite(waveFormat.dataChunk.chunkID, 1, 4, fp);
  fwrite(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

  if(pcm->bits==16){
    short* tmp=(short*)malloc(pcm->length);
    for(int i=0;i<(pcm->length/SHORTBYTE);i++){
      tmp[i]=pcm->s[i]*SHRT_MAX;
    }
    fwrite(tmp,sizeof(short),pcm->length/SHORTBYTE,fp);
    free(tmp);
  }else if(pcm->bits==32){
    int* tmp=(int*)malloc(pcm->length);
    for(int i=0;i<(pcm->length/INTBYTE);i++){
      tmp[i]=pcm->s[i]*INT_MAX;
    }
    fwrite(tmp,sizeof(short),pcm->length/INTBYTE,fp);
    free(tmp);
  }
  fclose(fp);
 }

void freepcm(MONO_PCM* pcm){
  free(pcm->s);
}

