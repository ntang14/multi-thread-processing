/*Nancy Tang Assignment 2 Shop.h file
This file contains the class for a Shop object that is a monitor for multiple
threads. There are barber threads and customer threads working together in the
shop.*/

#ifndef _SHOP_H_
#define _SHOP_H_
#include <pthread.h> // the header file for the pthread library
#include <queue> // the STL library: queue
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

using namespace std;

#define DEFAULT_CHAIRS 3 // the default number of chairs for waiting = 3
#define DEFAULT_BARBERS 1 // the default number of barbers = 1

class Shop {
   
   public:
      Shop( int nBarbers, int nChairs ); // initialize a Shop object with nBarbers and nChairs
      Shop( ); // initialize a Shop object with 1 barber and 3 chairs
      ~Shop( );
      int visitShop( int id); // return a non-negative number only when a customer got a service
      void leaveShop( int customerId, int barberId );
      void helloCustomer( int id );
      void byeCustomer( int id );
      int nDropsOff = 0; // the number of customers dropped off
   
   private:
      int barbers, chairs, availableChairs;
      pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
      pthread_cond_t *barberCond;
      pthread_cond_t *customerCond;
      std::queue<pthread_cond_t*> customers;
      std::queue<int> availableBarbers;
      bool doneHaircut = false;
      int *barberCustomerPair;
      bool *haircutDone;
};
#endif