var mosca = require('mosca');

var SECURE_KEY = __dirname + '/ryans-key.pem';
var SECURE_CERT = __dirname + '/ryans-cert.pem';

var settings = {
	host: '127.0.0.1',
  	port: 1883,
	// logger: {
	// 	name: "secureExample",
	// 	level: 40,
	// },
	// secure : {
	// 	port: 8443,
	// 	keyPath: SECURE_KEY,
	// 	certPath: SECURE_CERT,
	// }
};

var server = new mosca.Server(settings);

server.on('clientConnected', function(client) {
    console.log('client connected', client.id);
});

server.on('clientDisconnected', function(client) {
	console.log('client disconnected', client.id);
});

// fired when a message is received
server.on('published', function(packet, client) {
	console.log('Published', packet.payload);
});

server.on('subscribed', function(topic, client) {
	console.log('subscribed: ' + client.id);
});

server.on('unsubscribed', function(topic, client) {
	console.log('unsubscribed: ' + client.id);    
});

server.on('ready', setup);

// fired when the mqtt server is ready
function setup() {
  console.log('Mosca server is up and running');
}