#ifndef OWNER_H
#define OWNER_H

#include "buyer.h"
#include <fstream>
#include <sstream>
#include <mutex>

using namespace std;

// Declare the seeProfit function
void seeProfit();

// Declare the owner function here
void owner(vector<Product>& products);

#endif
