#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "Manager.h"

bool run = true;
Manager manager;

static void signal_handler(int signal)
{
   printf("Get signal\n");
   run = false;
}

int main(void)
{
   signal(SIGQUIT, signal_handler);

   int error = 0;
   while (run)
   {
      manager.process();
   }

   std::this_thread::sleep_for(std::chrono::seconds(1));
   fprintf(stderr, "Stopping\n");

   return EXIT_SUCCESS;
}