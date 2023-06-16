
const PORT: number = 10336;
const HOST: string = `http://localhost:${PORT}`;

interface Inputs {
    inputs: string[];
}

export async function getKeyboards(): Promise<[Inputs]> {
    let res = await fetch(`${HOST}/getKeyboards`);
    return await res.json();
}

export async function getActiveKeyboards(): Promise<[Inputs]> {
    let res = await fetch(`${HOST}/getActiveKeyboards`);
    return await res.json();
}

export async function getKeyboardHotkeys(): Promise<[Inputs]> {
    let res = await fetch(`${HOST}/getKeyboardHotkeys`);
    return await res.json();
}
