import pathlib
import subprocess
import asyncio
import os
import signal
import socket

import decky_plugin

HOME_DIR = str(pathlib.Path(os.getcwd()).parent.parent.resolve())
PARENT_DIR = str(pathlib.Path(__file__).parent.resolve())


class Plugin:
    backend_proc = None

    def portIsUsed():
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        result = bool(sock.connect_ex(("127.0.0.1", 10336)))
        sock.close()
        return not result

    # Asyncio-compatible long-running code, executed in a task when the plugin is loaded
    async def _main(self):
        # startup with my_env
        decky_plugin.logger.info("Starting DeckMenuHotkey backend instance")
        tries = 3
        while self.portIsUsed():
            decky_plugin.logger.info(
                "Port is used by (probably) another backend instance"
            )
            if not bool(tries):
                decky_plugin.logger.error("Port is still used, giving up")
                return
            else:
                decky_plugin.logger.info("Will try to wait")
                await asyncio.sleep(5)
                tries -= 1
        self.backend_proc = subprocess.Popen(
            decky_plugin.DECKY_PLUGIN_DIR + "/bin/deckmenuhotkey",
            cwd=decky_plugin.DECKY_PLUGIN_SETTINGS_DIR,
        )
        while True:
            await asyncio.sleep(1)

    async def _unload(self):
        decky_plugin.logger.info("Stopping DeckMenuHotkey backend instance")
        if self.backend_proc is not None:
            self.backend_proc.send_signal(signal.SIGQUIT)
            try:
                self.backend_proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                self.backend_proc.kill()
        else:
            decky_plugin.logger.info("No running DeckMenuHotkey instance found")
