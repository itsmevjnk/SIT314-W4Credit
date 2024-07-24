const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://localhost:1883');

var topic = '/alarm';

client.on('connect', () => {
    client.subscribe(topic);
    console.log('MQTT connected');
});

var alarm = false;
client.on('message', (topic, message) => {
    var newAlarm = (message == '1');
    if(newAlarm != alarm) {
        alarm = newAlarm;
        if(alarm) console.log('>>> Fire Alarm <<<');
        else console.log('All Clear');
    }
});