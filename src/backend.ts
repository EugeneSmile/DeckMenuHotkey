const PORT: number = 10336;
const HOST: string = `http://localhost:${PORT}`;

export async function getKeyboards(): Promise<[string]> {
    let res = await fetch(`${HOST}/getKeyboards`);
    return await res.json();
}

export async function getActiveKeyboards(): Promise<string[]> {
    let res = await fetch(`${HOST}/getActiveKeyboards`);
    return await res.json();
}

export async function getKeyboardHotkeys(): Promise<string[]> {
    let res = await fetch(`${HOST}/getKeyboardHotkeys`);
    return await res.json();
}

export async function logRequest(logType: string, text: string): Promise<string> {
    let res = await fetch(`${HOST}/logRequest?` + logType + `=` + text);
    return await res.json();
}