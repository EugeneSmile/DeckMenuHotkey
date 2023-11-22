#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "Manager.h"

std::shared_ptr<Manager> manager;

static void signal_handler(int signal)
{
   printf("[deckmenuhotkey] Got signal\n");
   if (manager)
      manager->stop();
}

int main(void)
{
   signal(SIGQUIT, signal_handler);

   manager = Manager::create();

   int error = 0;

   manager->process();

   std::this_thread::sleep_for(std::chrono::seconds(1));
   fprintf(stderr, "[deckmenuhotkey] Stopping\n");

   return EXIT_SUCCESS;
}