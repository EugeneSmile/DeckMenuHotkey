import {
  definePlugin,
  PanelSection,
  PanelSectionRow,
  ServerAPI,
  staticClasses,
  ToggleField,
  ButtonItem,
  DialogButton,
  Dropdown,
  SingleDropdownOption
} from "decky-frontend-lib";
import { VFC, useState, useEffect, Fragment } from "react";
import { FaWrench, FaKeyboard } from "react-icons/fa";
import * as backend from "./backend";
import { Keyboard, HotkeyGroups, Hotkey } from "./types";
import { Keys } from "./keys";

async function delayPromise<T>(value: T): Promise<T> {
  return new Promise<T>(resolve => {
    setTimeout(() => resolve(value), 275);
  });
}

const Content: VFC<{ serverAPI: ServerAPI }> = ({ }) => {

  const [isEnabled, setIsEnabled] = useState<boolean>(false);
  const [keyboards, setKeyboards] = useState<Keyboard[]>([]);
  const [dropdownKeys, setDropdownKeys] = useState<SingleDropdownOption[]>([]);
  const [keyboardEditIndex, setKeyboardEditIndex] = useState<number>(-1);

  useEffect(() => {
    createKeysOptions();
    backend.getEnabled()
      .then((enabled) => { setIsEnabled(enabled); });
    backend.getKeyboards()
      .then((keyboards) => { setKeyboards(keyboards); });
  }, []);

  const toggleEnable = (
    <PanelSectionRow>
      <ToggleField label="Enable" checked={isEnabled} onChange={async (checked) => {
        await backend.setEnabled(checked);
        setIsEnabled(checked);
      }} />
    </PanelSectionRow>
  )

  async function updateKeyboards() {
    await backend.setKeyboards(keyboards);
    await backend.getKeyboards()
      .then((keyboards) => { setKeyboards(keyboards); });
  }

  function setKeyboardEnabled(keyboard_name: string, enabled: boolean) {
    setKeyboards(keyboards.map((keyboard) => {
      if (keyboard.name === keyboard_name) {
        let new_keyboard = keyboard;
        new_keyboard.enabled = enabled;
        return new_keyboard;
      } else {
        return keyboard;
      }
    }));
  }

  function setKeyboardHotkeys(keyboard_name: string, hotkey_group: keyof HotkeyGroups, hotkey_name: keyof Hotkey, hotkey_value: boolean | number) {
    setKeyboards(keyboards.map((keyboard) => {
      if (keyboard.name === keyboard_name) {
        let new_keyboard = keyboard;
        new_keyboard.hotkeys[hotkey_group][hotkey_name] = hotkey_value;
        return new_keyboard;
      } else {
        return keyboard;
      }
    }));
  }

  function getModifierDiv(keyboard: Keyboard, hotkey_group: keyof HotkeyGroups, hotkey: keyof Hotkey) {
    return (
      <div style={{ flexGrow: 1 }}>
        <ToggleField label={hotkey} checked={keyboard.hotkeys[hotkey_group][hotkey]} onChange={async (checked) => { setKeyboardHotkeys(keyboard.name, hotkey_group, hotkey, checked); await updateKeyboards(); }}>
        </ToggleField>
      </div>
    )
  }

  function getHotkeyPanel(keyboard: Keyboard, hotkey_group: keyof HotkeyGroups) {
    return (
      <Fragment>
        <PanelSection>
          <div style={{ fontSize: 14 }}>{hotkey_group}</div>
          <div flow-children="horizontal" style={{ display: "flex", justifyContent: "space-between", padding: 0, gap: "8px" }}>
            {getModifierDiv(keyboard, hotkey_group, "alt")}
            {getModifierDiv(keyboard, hotkey_group, "ctrl")}
          </div>
          <div flow-children="horizontal" style={{ display: "flex", justifyContent: "space-between", padding: 0, gap: "8px" }}>
            {getModifierDiv(keyboard, hotkey_group, "shift")}
            {getModifierDiv(keyboard, hotkey_group, "meta")}
          </div>
          <Dropdown
            rgOptions={dropdownKeys}
            selectedOption={keyboard.hotkeys[hotkey_group]["key"]}
            onChange={async (e: SingleDropdownOption) => {
              setKeyboardHotkeys(keyboard.name, hotkey_group, "key", e.data);
              await updateKeyboards();
            }}></Dropdown>
        </PanelSection>
      </Fragment>
    )
  }

  function createKeysOptions() {
    let keysDropdownOptions: SingleDropdownOption[] = [];
    for (const key in Keys) {
      keysDropdownOptions.push({
        data: Keys[key].code,
        label: Keys[key].name
      });
      setDropdownKeys(keysDropdownOptions);
    }
  }

  if (!isEnabled) {
    return (
      <PanelSection title="General">
        {toggleEnable}
      </PanelSection>
    )
  }

  if (keyboardEditIndex >= 0 && keyboardEditIndex < keyboards.length) {
    return (
      <PanelSection>
        <div style={{ fontSize: 16 }}>{keyboards[keyboardEditIndex].name}</div>
        {getHotkeyPanel(keyboards[keyboardEditIndex], "steam")}
        {getHotkeyPanel(keyboards[keyboardEditIndex], "quickmenu")}
        <ButtonItem layout="below" onClick={async () => {
          setKeyboardEditIndex(-1);
        }}>Exit</ButtonItem>
      </PanelSection>
    )
  }

  return (
    <Fragment>
      <PanelSection title="General">
        {toggleEnable}
      </PanelSection>
      <PanelSection title="Keyboards">
        {keyboards.map((keyboard) => (
          <PanelSectionRow>
            <div flow-children="horizontal" style={{ display: "flex", justifyContent: "space-between", padding: 0, gap: "24px" }}>
              <DialogButton style={{ flexGrow: 1, display: "flex", justifyContent: "space-between", padding: 0, gap: 0, alignItems: "center", margin: 3 }} onClick={() => { setKeyboardEditIndex(keyboards.indexOf(keyboard)); }}>
                <FaWrench style={{ flexGrow: 0, minWidth: 0, width: "25%", padding: 0 }} />
                <div style={{ flexGrow: 1, padding: 0, fontSize: 12 }}>{keyboard.name}</div>
              </DialogButton>
              <div style={{ flexGrow: 1 }}>
                <ToggleField checked={keyboard.enabled} onChange={async (checked) => {
                  setKeyboardEnabled(keyboard.name, checked);
                  await updateKeyboards();
                }} />
              </div>
            </div>
          </PanelSectionRow>))
        }
        <PanelSectionRow>
          <ButtonItem
            layout="below"
            onClick={async () => {
              setKeyboards([]);
              setKeyboards(await delayPromise(backend.reloadKeyboards()));
            }}>
            Refresh
          </ButtonItem>
          <ButtonItem layout="below" onClick={async () => { await backend.printConfig(); }}>Backend: printConfig</ButtonItem>
          <ButtonItem layout="below" onClick={async () => { await backend.stopBackend(); }}>Backend: stopBackend</ButtonItem>
        </PanelSectionRow>
      </PanelSection>
    </Fragment>
  );
};


export default definePlugin((serverApi: ServerAPI) => {
  return {
    title: <div className={staticClasses.Title}>DeckMenuHotkey Plugin</div>,
    content: <Content serverAPI={serverApi} />,
    icon: <FaKeyboard />,
    onDismount() { },
  };
});
