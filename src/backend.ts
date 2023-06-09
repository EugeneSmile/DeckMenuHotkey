
const PORT: number = 10336;
const HOST: string = `http://localhost:${PORT}`;

interface Inputs {
    inputs: string[];
}

export async function getInputs(): Promise<[Inputs]> {
    let res = await fetch(`${HOST}/inputs`);
    return await res.json();
}
