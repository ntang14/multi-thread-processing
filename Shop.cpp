/*Nancy Tang   Assignment 2   Shop.cpp file
This file contains the fucntions for a Shop object that is a monitor for 
multiple threads. There are barber threads and customer threads working
together in the shop.*/

#include "Shop.h"

using namespace std;

Shop::Shop( int nBarbers, int nChairs ) {
   barbers = nBarbers;
   chairs = nChairs;
   availableChairs = chairs;
   customerCond = new pthread_cond_t[barbers];
   barberCond = new pthread_cond_t[barbers];
   barberCustomerPair = new int [barbers];
   haircutDone = new bool [barbers];
   for ( int i = 0; i < barbers; i++ ) {
      customerCond[i] = PTHREAD_COND_INITIALIZER;
      barberCond[i] = PTHREAD_COND_INITIALIZER;
      barberCustomerPair[i] = -1;
      haircutDone[i] = false;
   }
}

Shop::Shop( ) {
   barbers = DEFAULT_BARBERS;
   chairs = DEFAULT_CHAIRS;
   availableChairs = chairs;
   customerCond = new pthread_cond_t[barbers];
   barberCond = new pthread_cond_t[barbers];
   barberCustomerPair = new int [barbers];
   haircutDone = new bool [barbers];
   for ( int i = 0; i < barbers; i++ ) {
      customerCond[i] = PTHREAD_COND_INITIALIZER;
      barberCond[i] = PTHREAD_COND_INITIALIZER;
      barberCustomerPair[i] = -1;
      haircutDone[i] = false;
   }
}

Shop::~Shop( ) {
   delete [] haircutDone;
   delete [] barberCustomerPair;
   for (int i = 0; i < barbers; i++) {
      pthread_cond_destroy( &barberCond[i] );
      pthread_cond_destroy( &customerCond[i] );
   }
   pthread_mutex_destroy( &mutex );
}

int Shop::visitShop( int id ) {
   pthread_mutex_lock( &mutex );
   if ( availableChairs == 0 && availableBarbers.empty( ) ) {
      cout << id << " leaves the shop because of no available waiting chairs"
         << endl;
      nDropsOff++;
      pthread_mutex_unlock( &mutex );
      return -1;
   } else if ( availableBarbers.empty( ) || !customers.empty( ) ) {
      pthread_cond_t *cond = new pthread_cond_t;
      *cond = PTHREAD_COND_INITIALIZER;
      customers.push( cond );
      cout << id << " takes a waiting chair. # waiting chairs available = " <<
         --availableChairs << endl;
      while ( availableBarbers.empty( ) || customers.front( ) != cond ) {
         pthread_cond_wait( cond, &mutex );
      }
      delete customers.front( );
      customers.front( ) = NULL;
      customers.pop( );
      if ( !customers.empty( ) ) {
         pthread_cond_signal( customers.front( ) );
      }
   }
   int currentBarber = availableBarbers.front( );
   barberCustomerPair[currentBarber] = id;
   availableBarbers.pop( );
   cout << id << " moves to service chair[" << currentBarber << 
      "], # waiting seats available = " << availableChairs << endl;
   pthread_cond_signal( &barberCond[currentBarber] );
   pthread_mutex_unlock( &mutex );
   return currentBarber; 
}

void Shop::leaveShop( int customerId, int barberId ) {
   pthread_mutex_lock( &mutex );
   cout << customerId << " wait for barber[" << barberId <<
      "] to be done with hair-cut" << endl;
   while ( !haircutDone[barberId] ) {
      pthread_cond_wait( &customerCond[barberId], &mutex );
   }
   cout << customerId << " says good-bye to barber[]" << endl;
   haircutDone[barberId] = false;
   pthread_cond_signal( &barberCond[barberId] );
   pthread_mutex_unlock( &mutex );
}

void Shop::helloCustomer( int id ) {
   pthread_mutex_lock( &mutex );
   if ( customers.empty( ) && barberCustomerPair[id] == -1 ) {
      availableBarbers.push( id );
      cout << "-" << id << " sleeps because of no customers." << endl;
      while ( barberCustomerPair[id] == -1 ) {
         pthread_cond_wait( &barberCond[id], &mutex );
      }
   } else if ( barberCustomerPair[id] == -1 ) {
      pthread_cond_signal( customers.front( ) );
      while ( barberCustomerPair[id] == -1 ) {
         pthread_cond_wait( &barberCond[id], &mutex );
      }
   } else if ( barberCustomerPair[id] == 0 ) {
      while ( barberCustomerPair[id] == 0 ) {
         pthread_cond_wait( &barberCond[id], &mutex );
      }
   }
   cout << "-" << id << " starts a hair-cut service for customer[" <<
      barberCustomerPair[id] << "]." << endl;
   pthread_mutex_unlock( &mutex );
}

void Shop::byeCustomer( int id ) {
   pthread_mutex_lock( &mutex );
   cout << "-" << id << " says he's done with a hair-cut service for customer ["
      << barberCustomerPair[id] << "]." << endl;
   haircutDone[id] = true;
   pthread_cond_signal( &customerCond[id] );
   while ( haircutDone[id] ) {
      pthread_cond_wait( &barberCond[id], &mutex );
   }
   barberCustomerPair[id] = -1;
   cout << "-" << id << " calls in another customer" << endl;
   if ( !customers.empty( ) ) {
      barberCustomerPair[id] = 0;
      availableBarbers.push( id );
      availableChairs++;
      pthread_cond_signal ( customers.front( ) );
   }
   pthread_mutex_unlock( &mutex );
}