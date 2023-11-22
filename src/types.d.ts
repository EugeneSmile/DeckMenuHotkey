declare module "*.svg" {
  const content: string;
  export default content;
}

declare module "*.png" {
  const content: string;
  export default content;
}

declare module "*.jpg" {
  const content: string;
  export default content;
}

export interface Hotkey {
  key: number;
  meta: boolean;
  alt: boolean;
  ctrl: boolean;
  shift: boolean;
}

export interface HotkeyGroups {
  quickmenu: Hotkey;
  steam: Hotkey;
}

export interface Keyboard {
  hotkeys: Hotkeys;
  enabled: boolean;
  name: string;
}

export enum HotkeyGroupsEnum {
  STEAM = 0,
  QUICKMENU = 1,
  NUMBER_OF_STEAM_HOTKEYS
}

export interface Key {
  code: number;
  name: string;
}

export interface SortedKey {
  [index: number]: Key;
}

export const HotkeyGroupsNames = {
  steam: "Steam",
  quickmenu: "Quick Access Menu"
}
