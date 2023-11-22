#!/usr/bin/env bash
echo "Building plugin in $(pwd)"
pnpm run build
scp dist/index.js deck@tsh3w:/home/deck/homebrew/plugins/DeckMenuHotkey/dist
