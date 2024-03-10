const snap7 = require('../');
const s7server = new snap7.S7Server();
const s7client = new snap7.S7Client();


// Set up event listener
s7server.on("event", (event) => {
    console.log(s7server.EventText(event));
});

// Create a new Buffer and register it to the server as DB1
var db1 = Buffer.alloc(100, 'ÿ');
s7server.RegisterArea(s7server.srvAreaDB, 1, db1);
console.log('RegisterArea');

// Start the server
(async () => {
    try {
        await s7server.StartTo('127.0.0.1');
        console.log('Server started');

        // Close the server after 20s in this example
        setTimeout(async function() {
            await s7server.Stop();
            console.log('Server stopped')
            s7server.UnregisterArea(s7server.srvAreaDB, 1);
            console.log('UnregisterArea')
            s7server.removeAllListeners("event");
        }, 10000);
    } catch (error) {
        console.log(` >> Error Code #${error} - ${s7server.ErrorText(error)}`)
    }

    try {
        await s7client.ConnectTo('127.0.0.1', 0, 1);
        console.log('Connected')
        console.log(await s7client.DBRead(1, 0, 8));
        console.log(await s7client.ListBlocksOfType(s7client.Block_DB));
        console.log(await s7client.GetAgBlockInfo(s7client.Block_DB, 1));
    } catch (error) {
        console.log(` >> Error Code #${error} - ${s7client.ErrorText(error)}`)
    }
})();
