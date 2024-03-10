const snap7 = require('../');
const s7client = new snap7.S7Client();

(async () => {
    try {
        await s7client.ConnectTo('127.0.0.1', 0, 1);
        console.log('Connected')
        var DB1 = await s7client.DBRead(1, 0, 8);
        console.log(DB1);
    } catch (error) {
        console.log(` >> Error Code #${error} - ${s7client.ErrorText(error)}`)
    }
})()
