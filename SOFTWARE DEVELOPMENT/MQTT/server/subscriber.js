var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://127.0.0.1',{
	clientId:"CLIENTB",
    rejectUnauthorized: false
});


client.on('connect', function () {
    client.subscribe('myTopic')
})

client.on('message', function (topic, message) {
	context = message.toString();
	console.log(context)
	// setInterval(function() {
	// 	client.publish('myTopic', 'Hello from subscribe');
	// 	console.log('Message Sent');
	// }, 500);
})