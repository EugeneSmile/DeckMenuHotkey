import { Keyboard } from "./types"

const PORT: number = 10336;
const HOST: string = `localhost`;
let URL: string = `http://${HOST}:${PORT}`;

export async function reloadKeyboards(): Promise<Keyboard[]> {
    let res = await fetch(`${URL}/reloadKeyboards`);
    if (res.ok)
        return getKeyboards();
    return [];
}

export async function getKeyboards(): Promise<Keyboard[]> {
    let res = await fetch(`${URL}/getKeyboards`);
    return await res.json();
}

export async function setKeyboards(keyboards: Keyboard[]): Promise<Response> {
    let res = await fetch(`${URL}/setKeyboards`, {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(keyboards)
    })
    return res;
}

export async function logRequest(logType: string, text: string): Promise<string> {
    let res = await fetch(`${URL}/logRequest?` + logType + `=` + text);
    return await res.json();
}

export async function getEnabled(): Promise<boolean> {
    let res = await fetch(`${URL}/getEnabled`);
    return await res.json();
}

export async function setEnabled(enabled: boolean): Promise<Response> {
    let res = await fetch(`${URL}/setEnabled`, {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(enabled)
    })
    return res;
}

export async function printConfig(): Promise<Response> {
    let res = await fetch(`${URL}/printConfig`);
    return await res.json();
}

export async function stopBackend(): Promise<Response> {
    let res = await fetch(`${URL}/stopBackend`);
    return await res.json();
}