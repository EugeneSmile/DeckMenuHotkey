#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <linux/input-event-codes.h>
#include <stdbool.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "Manager.h"
#include "WebInterface.h"

bool run = true;
Manager manager;
// WebInterface interface;

static void signal_handler(int signal)
{
   printf("Get signal\n");
   run = false;
}

int main(void)
{
   signal(SIGINT, signal_handler);

   int error = 0;
   while (run)
   {
      manager.process();
   }

   std::this_thread::sleep_for(std::chrono::seconds(1));
   fprintf(stderr, "Stopping\n");

   return EXIT_SUCCESS;
}