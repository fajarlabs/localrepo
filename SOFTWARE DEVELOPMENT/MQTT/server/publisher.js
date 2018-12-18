var mqtt = require('mqtt');
var client  = mqtt.connect('mqtt://127.0.0.1',{
	clientId:"CLIENTA",
    rejectUnauthorized: false
});

client.on('connect', function () {
	//client.subscribe('myTopic')
	setInterval(function() {
		client.publish('myTopic', 'Hello from publisher');
		console.log('Message Sent');
	}, 500);
});

client.on('message', function (topic, message) {
	context = message.toString();
	console.log(context)
})