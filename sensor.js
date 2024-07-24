const mqtt = require('mqtt');

const readline = require('readline'); // for reading keypresses
readline.emitKeypressEvents(process.stdin);

const client = mqtt.connect('mqtt://localhost:1883');
var topic = '/sensors/' + require('crypto').randomUUID(); // generate random UUID

client.on('connect', () => {
    console.log('MQTT connected (topic: ' + topic + ')');

    if(process.stdin.isTTY) process.stdin.setRawMode(true);
    console.log('Press SPACE to toggle alarm conditions, or press Q to exit');

    var alarm = false;
    process.stdin.on('keypress', (chunk, key) => {
        if(key) {
            // console.log('Key pressed: ' + key.name);
            switch(key.name) {
                case 'q': process.exit(); break;
                case 'space': alarm = !alarm; console.log('Alarm set to ' + alarm); break;
                default: break;
            }
        } 
    });

    setInterval(() => {
        var message = (alarm) ? '{"temperature":62.50,"smoke":64}' : '{"temperature":25.00,"smoke":30}';
        client.publish(topic, message);
        console.log('Published message: ' + message);
    }, 2000);
});