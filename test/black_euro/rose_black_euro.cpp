#include <math.h>
#include "merlin_type_define.h"
extern int __merlin_include__GB_merlin_type_define_h_1;
extern int __merlin_include__GB_math_h_0;

static void p_ZN9stockDataC2Efffff(struct stockData *this_,float timeT,float freeRate,float volatility,float initPrice,float strikePrice)
{
  this_ -> freeRate = freeRate;
  this_ -> initPrice = initPrice;
  this_ -> timeT = timeT;
  this_ -> volatility = volatility;
  this_ -> strikePrice = strikePrice;
}

static void p_ZN9stockDataC2ERK9stockData(struct stockData *this_,const struct stockData &data)
{
  this_ -> freeRate = data . freeRate;
  this_ -> initPrice = data . initPrice;
  this_ -> timeT = data . timeT;
  this_ -> volatility = data . volatility;
  this_ -> strikePrice = data . strikePrice;
}

static void p_ZN12blackScholesC2E9stockData(struct blackScholes *this_,struct stockData data)
{
  p_ZN9stockDataC2ERK9stockData(&this_ -> data,data);
}

static void p_ZN3RNG10init_arrayEP3RNGPjKi(struct RNG *rng,unsigned int *seed,const int size)
{
//uint tmp[size];
  unsigned int tmp[64];
  
#pragma ACCEL parallel
//loop_set_seed:for(int i=0;i<size;i++)
  
#pragma ACCEL PIPELINE auto{__PIPE__L1}
  loop_set_seed:
  for (int i = 0; i < 64; i++) {
    rng[i] . index = 0;
    rng[i] . seed = seed[i];
    tmp[i] = seed[i];
  }
  
#pragma ACCEL PIPELINE auto{__PIPE__L2}
  
#pragma ACCEL TILE FACTOR=auto{__TILE__L2}
  
#pragma ACCEL PARALLEL FACTOR=auto{__PARA__L2}
  loop_seed_init:
  for (int i = 0; i < 312; i++) {
    
#pragma ACCEL parallel
//loop_group_init:for(int k=0;k<size;k++)
    
#pragma ACCEL PIPELINE auto{__PIPE__L2_0}
    loop_group_init:
    for (int k = 0; k < 64; k++) {
      rng[k] . mt_e[i] = tmp[k];
      tmp[k] = ((unsigned int )0x6C078965) * (tmp[k] ^ tmp[k] >> 32 - 2) + ((unsigned int )(i * 2)) + ((unsigned int )1);
      rng[k] . mt_o[i] = tmp[k];
      tmp[k] = ((unsigned int )0x6C078965) * (tmp[k] ^ tmp[k] >> 32 - 2) + ((unsigned int )(i * 2)) + ((unsigned int )2);
    }
  }
}

static int p_ZN3RNG8increaseEi(struct RNG *this_,int k)
{
  int tmp = this_ -> index + k;
  return tmp >= 312?tmp - 312 : tmp;
}

static void p_ZN3RNG14extract_numberEPjPj(struct RNG *this_,unsigned int *num1,unsigned int *num2)
{
  int id1 = p_ZN3RNG8increaseEi(this_,1);
  int idm = p_ZN3RNG8increaseEi(this_,198);
  int idm1 = p_ZN3RNG8increaseEi(this_,199);
  unsigned int x = this_ -> seed;
  unsigned int x1 = this_ -> mt_o[this_ -> index];
  unsigned int x2 = this_ -> mt_e[id1];
  unsigned int xm = this_ -> mt_o[idm];
  unsigned int xm1 = this_ -> mt_e[idm1];
  x = (x & 0x80000000) + (x1 & ((unsigned int )0x7FFFFFFF));
  unsigned int xp = x >> 1;
  if ((x & ((unsigned int )0x01)) != ((unsigned int )0)) 
    xp ^= 0x9908B0DF;
  x = xm ^ xp;
  unsigned int y = x;
  y ^= y >> 11 & 0xFFFFFFFF;
  y ^= y << 7 & 0x9D2C5680;
  y ^= y << 15 & 0xEFC60000;
  y ^= y >> 18;
   *num1 = y;
  this_ -> mt_e[this_ -> index] = x;
  x1 = (x1 & 0x80000000) + (x2 & ((unsigned int )0x7FFFFFFF));
  unsigned int xt = x1 >> 1;
  if ((x1 & ((unsigned int )0x01)) != ((unsigned int )0)) 
    xt ^= 0x9908B0DF;
  x1 = xm1 ^ xt;
  unsigned int y1 = x1;
  y1 ^= y1 >> 11 & 0xFFFFFFFF;
  y1 ^= y1 << 7 & 0x9D2C5680;
  y1 ^= y1 << 15 & 0xEFC60000;
  y1 ^= y1 >> 18;
   *num2 = y1;
  this_ -> mt_o[this_ -> index] = x1;
  this_ -> index = id1;
  this_ -> seed = x2;
}

static void p_ZN3RNG10BOX_MULLEREPfPfff(struct RNG *this_,float *data1,float *data2,float ave,float deviation)
{
  float tp;
  float tmp1;
  float tmp2;
  unsigned int num1;
  unsigned int num2;
  p_ZN3RNG14extract_numberEPjPj(this_,&num1,&num2);
  tmp1 = ((float )num1) * ((float )2.328306e-10);
  tmp2 = ((float )num2) * ((float )2.328306e-10);
  tp = sqrtf(((((float )(- 2)) * logf(tmp1) > ((float )0)?((float )(- 2)) * logf(tmp1) : ((float )0))) * deviation);
   *data1 = cosf(((float )2) * ((float )3.14159265358979323846) * tmp2) * tp + ave;
   *data2 = sinf(((float )2) * ((float )3.14159265358979323846) * tmp2) * tp + ave;
}
const int _ZN12blackScholes8NUM_SIMSE = 512;
const int _ZN12blackScholes8NUM_RNGSE = 64;
const int _ZN12blackScholes13NUM_SIMGROUPSE = 1024 * 64;

static void p_ZN12blackScholes9sampleSIMEP3RNGPfPf(struct blackScholes *this_,struct RNG *mt_rng,float *pCall,float *pPut)
{
  const float ratio1 = this_ -> data . strikePrice * expf(-this_ -> data . freeRate * this_ -> data . timeT);
  const float ratio2 = (this_ -> data . freeRate - 0.5f * this_ -> data . volatility * this_ -> data . volatility) * this_ -> data . timeT;
  const float ratio3 = this_ -> data . volatility * this_ -> data . volatility * this_ -> data . timeT;
  const float ratio4 = ratio2 - logf(this_ -> data . strikePrice / this_ -> data . initPrice);
  float sumCall = 0.0f;
  float sumPut = 0.0f;
  float pCall1[64][512];
  float pCall2[64][512];
  float pPut1[64][512];
  float pPut2[64][512];
  float num1[64][512];
  float num2[64][512];
  
#pragma ACCEL PIPELINE auto{__PIPE__L3}
  
#pragma ACCEL TILE FACTOR=auto{__TILE__L3}
  
#pragma ACCEL PARALLEL FACTOR=auto{__PARA__L3}
  loop_init:
  for (int k = 0; k < _ZN12blackScholes8NUM_SIMSE; k++) {
    
#pragma ACCEL parallel
    
#pragma ACCEL PIPELINE auto{__PIPE__L3_0}
    loop_r:
    for (int i = 0; i < _ZN12blackScholes8NUM_RNGSE; i++) {
      pCall1[i][k] = 0.0f;
      pCall2[i][k] = 0.0f;
      pPut1[i][k] = 0.0f;
      pPut2[i][k] = 0.0f;
    }
  }
  
#pragma ACCEL PIPELINE auto{__PIPE__L4}
  
#pragma ACCEL TILE FACTOR=auto{__TILE__L4}
  
#pragma ACCEL PARALLEL FACTOR=auto{__PARA__L4}
  loop_main:
  for (int j = 0; j < _ZN12blackScholes13NUM_SIMGROUPSE; j += 2) {
    
#pragma ACCEL PIPELINE auto{__PIPE__L4_0}
    
#pragma ACCEL TILE FACTOR=auto{__TILE__L4_0}
    
#pragma ACCEL PARALLEL FACTOR=auto{__PARA__L4_0}
    loop_share:
    for (unsigned int k = (unsigned int )0; k < ((unsigned int )_ZN12blackScholes8NUM_SIMSE); k++) {
      
#pragma ACCEL parallel
      
#pragma ACCEL PIPELINE auto{__PIPE__L4_0_0}
      loop_parallel:
      for (int i = 0; i < _ZN12blackScholes8NUM_RNGSE; i++) {
        p_ZN3RNG10BOX_MULLEREPfPfff(&mt_rng[i],&num1[i][k],&num2[i][k],ratio4,ratio3);
        float payoff1 = expf(num1[i][k]) - 1.0f;
        float payoff2 = expf(num2[i][k]) - 1.0f;
        if (num1[i][k] > 0.0f) 
          pCall1[i][k] += payoff1;
         else 
          pPut1[i][k] -= payoff1;
        if (num2[i][k] > 0.0f) 
          pCall2[i][k] += payoff2;
         else 
          pPut2[i][k] -= payoff2;
      }
    }
  }
  
#pragma ACCEL PIPELINE auto{__PIPE__L5}
  
#pragma ACCEL TILE FACTOR=auto{__TILE__L5}
  
#pragma ACCEL PARALLEL FACTOR=auto{__PARA__L5}
  loop_sumCall:
  for (int k = 0; k < _ZN12blackScholes8NUM_SIMSE; k++) {
    
#pragma ACCEL parallel
    
#pragma ACCEL PIPELINE auto{__PIPE__L5_0}
    loop_sr:
    for (int i = 0; i < _ZN12blackScholes8NUM_RNGSE; i++) {
      sumCall += pCall1[i][k] + pCall2[i][k];
      sumPut += pPut1[i][k] + pPut2[i][k];
    }
  }
   *pCall = ratio1 * sumCall / ((float )_ZN12blackScholes8NUM_RNGSE) / ((float )_ZN12blackScholes13NUM_SIMGROUPSE) / ((float )_ZN12blackScholes8NUM_SIMSE);
   *pPut = ratio1 * sumPut / ((float )_ZN12blackScholes8NUM_RNGSE) / ((float )_ZN12blackScholes13NUM_SIMGROUPSE) / ((float )_ZN12blackScholes8NUM_SIMSE);
}

static void p_ZN12blackScholes10simulationEPfPf(struct blackScholes *this_,float *pCall,float *pPut)
{
  struct RNG mt_rng[64];
  unsigned int seeds[64];
  
#pragma ACCEL parallel
  
#pragma ACCEL PIPELINE auto{__PIPE__L0}
  loop_seed:
  for (int i = 0; i < _ZN12blackScholes8NUM_RNGSE; i++) {
    seeds[i] = ((unsigned int )i);
  }
  p_ZN3RNG10init_arrayEP3RNGPjKi(mt_rng,seeds,_ZN12blackScholes8NUM_RNGSE);
  p_ZN12blackScholes9sampleSIMEP3RNGPfPf(this_,mt_rng,pCall,pPut);
}
#pragma ACCEL kernel

void blackEuro(float pCall[1],float pPut[1],float timeT,float freeRate,float volatility,float initPrice,float strikePrice)
{
  
#pragma ACCEL interface variable=pCall depth=1
  
#pragma ACCEL interface variable=pPut depth=1
  struct stockData sd;
  p_ZN9stockDataC2Efffff(&sd,timeT,freeRate,volatility,initPrice,strikePrice);
  struct stockData temp;
  p_ZN9stockDataC2ERK9stockData(&temp,sd);
  struct blackScholes bs;
  p_ZN12blackScholesC2E9stockData(&bs,temp);
  float call;
  float put;
  p_ZN12blackScholes10simulationEPfPf(&bs,&call,&put);
   *pCall = call;
   *pPut = put;
  return ;
}
