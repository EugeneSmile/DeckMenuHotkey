import {
  definePlugin,
  Dropdown,
  DropdownOption,
  SingleDropdownOption,
  PanelSection,
  PanelSectionRow,
  ServerAPI,
  staticClasses,
  TextField,
} from "decky-frontend-lib";
import { VFC, useState } from "react";
import { FaShip } from "react-icons/fa";
import * as backend from "./backend";

// interface AddMethodArgs {
//   left: number;
//   right: number;
// }

const Content: VFC<{ serverAPI: ServerAPI }> = ({ }) => {
  // const [result, setResult] = useState<number | undefined>();

  // const onClick = async () => {
  //   const result = await serverAPI.callPluginMethod<AddMethodArgs, number>(
  //     "add",
  //     {
  //       left: 2,
  //       right: 2,
  //     }
  //   );
  //   if (result.success) {
  //     setResult(result.result);
  //   }
  // };

  const [dropdownOptions, setDropdownOptions] = useState<DropdownOption[]>([]);
  const [selectedApp, setSelectedApp] = useState<number | null>(null);

  return (
    <PanelSection title="Panel Section">
      <PanelSectionRow>
        <TextField value="Hello!" />
        <Dropdown
          strDefaultLabel="Select Keyboard Input"
          rgOptions={dropdownOptions}
          selectedOption={selectedApp}
          onChange={(e: SingleDropdownOption) => {
          }}
        />
      </PanelSectionRow>
    </PanelSection>
  );
};

export default definePlugin((serverApi: ServerAPI) => {
  return {
    title: <div className={staticClasses.Title}>DeckMenuHotkey Plugin</div>,
    content: <Content serverAPI={serverApi} />,
    icon: <FaShip />,
    onDismount() { },
  };
});
