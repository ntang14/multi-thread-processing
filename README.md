# multi-thread-processing
A project utilizing pthreads with mutex locks.

This project tackles the sleeping barbers problem. The main program simulates a barber shop with n number of barbers and m number of customers. Each barber and each customer runs on its on pthread. If there are no customers in the shop, the barber(s) sleep until they are awaken by a customer. There are k number of waiting chairs available for customers to wait in if all the barbers are busy. If all the barbers are busy and all of the waiting chairs are taken, the customer leaves. Once the program is done running all of the customers, the program stops and displays the number of customers who were not served.

NOTE: The main program was already written for the assignment. I coded almost all of the shop.h file and all of the shop.cpp file.
