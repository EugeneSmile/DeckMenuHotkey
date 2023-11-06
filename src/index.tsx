import {
  definePlugin,
  PanelSection,
  PanelSectionRow,
  ServerAPI,
  staticClasses,
  ButtonItem
} from "decky-frontend-lib";
import { VFC, useState, useEffect } from "react";
import { FaShip } from "react-icons/fa";
import * as backend from "./backend";

// interface AddMethodArgs {
//   left: number;
//   right: number;
// }

async function delayPromise<T>(value: T): Promise<T> {
  return new Promise<T>(resolve => {
    setTimeout(() => resolve(value), 275);
  });
}

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

  const [keyboards, setKeyboards] = useState<string[]>([]);
  const [buttonText, setButtonText] = useState("Reload");

  useEffect(() => {
    backend.getKeyboards()
      .then((keyboards) => { setKeyboards(keyboards); backend.logRequest("info", keyboards.length.toString()) });
  }, []);

  const refreshButton = (
    <PanelSectionRow>
      <ButtonItem
        layout="below"
        onClick={async () => {
          backend.logRequest("info", `Keyboards: ` + keyboards.length.toString());
          //setKeyboards(await delayPromise(backend.getKeyboards()));
        }}
      >
        {buttonText}
      </ButtonItem>
    </PanelSectionRow >
  );

  return (
    <PanelSection title="Keyboards">
      {keyboards.map((keyboard) => (
        <PanelSectionRow key={keyboard}>
          {keyboard}
        </PanelSectionRow>
      ))}
      {refreshButton}
    </PanelSection >
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
