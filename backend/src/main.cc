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

#include "Controller_XBox.h"

bool run = true;
Controller_XBox cxb;

struct mod_keys
{
   bool meta;
   bool alt;
   bool ctrl;
   bool shift;
};

struct mods
{
   struct mod_keys left;
   struct mod_keys right;
} modifiers;

// const char *keyboard_dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
const char *keyboard_dev = "/dev/input/by-id/usb-Logitech_USB_Receiver-if02-event-mouse";

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"};

static void signal_handler(int signal)
{
   printf("Get signal\n");
   run = false;
}

void emit(int fd, int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(fd, &ie, sizeof(ie));
}

int init_keyboard(int *fd)
{
   *fd = open(keyboard_dev, O_RDONLY | O_NONBLOCK);
   if (*fd == -1)
   {
      fprintf(stderr, "Cannot open %s: %s.\n", keyboard_dev, strerror(errno));
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

int process(int fd_keyboard)
{
   struct input_event ev_keyboard;

   ssize_t n;
   n = read(fd_keyboard, &ev_keyboard, sizeof ev_keyboard);
   if (n == (ssize_t)-1)
   {
      if (errno == EAGAIN)
      {
         return EXIT_SUCCESS;
      }
      if (errno != EINTR)
      {
         fprintf(stderr, "Err %d\n", errno);
         return errno;
      }
   }
   else
   {
      if (n != sizeof ev_keyboard)
      {
         errno = EIO;
         return errno;
      }
   }

   if (ev_keyboard.type == EV_KEY && ev_keyboard.value >= 0 && ev_keyboard.value <= 2)
   {
      // Released
      if (ev_keyboard.value == 0)
      {
         if (ev_keyboard.code == KEY_LEFTMETA)
            modifiers.left.meta = false;
      }

      // Pressed
      if (ev_keyboard.value == 1)
      {
         if (ev_keyboard.code == KEY_LEFTMETA)
            modifiers.left.meta = true;
      }

      if (modifiers.left.meta)
      {
         if (ev_keyboard.code == KEY_HOMEPAGE && ev_keyboard.value == 0)
         {
            cxb.emit(EV_KEY, BTN_MODE, 1);
            cxb.emit(EV_SYN, SYN_REPORT, 0);
            cxb.emit(EV_KEY, BTN_MODE, 0);
            cxb.emit(EV_SYN, SYN_REPORT, 0);
         }
         if (ev_keyboard.code == KEY_BACK && ev_keyboard.value == 0)
         {
            cxb.emit(EV_KEY, BTN_MODE, 1);
            cxb.emit(EV_SYN, SYN_REPORT, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cxb.emit(EV_KEY, BTN_SOUTH, 1);
            cxb.emit(EV_SYN, SYN_REPORT, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cxb.emit(EV_KEY, BTN_MODE, 0);
            cxb.emit(EV_SYN, SYN_REPORT, 0);
            cxb.emit(EV_KEY, BTN_SOUTH, 0);
            cxb.emit(EV_SYN, SYN_REPORT, 0);
         }
      }
   }
   return EXIT_SUCCESS;
}

int main(void)
{
   signal(SIGINT, signal_handler);

   int fd_keyboard;

   if (init_keyboard(&fd_keyboard))
   {
      fprintf(stderr, "Error of keyboard init\n");
      return EXIT_FAILURE;
   }

   int error = 0;
   while (run)
   {
      if (error = process(fd_keyboard))
         fprintf(stderr, "Error of process: %d\n", error);
   }

   std::this_thread::sleep_for(std::chrono::seconds(1));
   fprintf(stderr, "Stopping\n");
   close(fd_keyboard);

   return EXIT_SUCCESS;
}