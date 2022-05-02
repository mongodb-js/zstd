const { compress, decompress } = require('../index');

async function run() {
    const buffer = Buffer.from('test');
    const compressed = await compress(buffer);
    const decompressed = await decompress(compressed);
    console.log(compressed, decompressed);
}

run();