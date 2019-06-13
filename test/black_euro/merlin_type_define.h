
#ifndef __MERLIN_TYPE_DEFINE_H__

#define __MERLIN_TYPE_DEFINE_H__
struct stockData {float timeT;float freeRate;float volatility;float initPrice;float strikePrice;};
struct blackScholes {struct stockData data;};
struct RNG {int index;unsigned int seed;unsigned int mt_e[312];unsigned int mt_o[312];};

#endif
