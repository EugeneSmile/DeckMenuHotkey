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

export enum SteamHotkeys {
  STEAM = 0,
  QUICKMENU = 1,
  NUMBER_OF_STEAM_HOTKEYS
}

export interface Hotkey {
  type: SteamHotkeys;
  meta: boolean;
  alt: boolean;
  ctrl: boolean;
  shift: boolean;
  key: number;
}
