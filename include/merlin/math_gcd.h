#ifndef MATHGCD_H
#define MATHGCD_H

#include <vector>

using namespace std;

int gcd2(int a, int b);
int gcd3(int a, int b, int c);
int intdiv(int a, int b);
// int ngcd(int *a, int n);
int ngcd(vector<int> a, int n);
bool isPo2(int bank);
// void scalePo2(int base, int &factor);

#endif
