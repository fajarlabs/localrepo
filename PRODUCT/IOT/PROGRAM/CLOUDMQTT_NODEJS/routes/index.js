var express = require('express');
var mqtt = require('mqtt');
var router = express.Router();
var url = require('url');

var mqtt_url = process.env.CLOUDMQTT_URL || 'mqtt://hhrblzkj:5fDMVfZA3RM3@m15.cloudmqtt.com:15944';
var topic = process.env.CLOUDMQTT_TOPIC || 'data-6e5c4b13-357f-4a56-abb9-a22a23f2e505';
var client = mqtt.connect(mqtt_url);

/* GET home page. */
router.get('/', function(req, res, next) {
  var config =  url.parse(mqtt_url);
  config.topic = topic;
  res.render('index', {
	connected: client.connected,
	config: config
  });
});

/* waiting Connect Socket MQTT */
client.on('connect', function() {

  /* method POST URI publish */
  router.post('/publish', function(req, res) {
	var msg = JSON.stringify({
	  date: new Date().toString(),
	  msg: req.body.msg
	});
    client.publish(topic, msg, function() {
      res.writeHead(204, { 'Connection': 'keep-alive' });
      res.end();
    });
  });

  /* method GET URI publish */
  router.get('/stream', function(req, res) {
    // send headers for event-stream connection
    // see spec for more information
    res.writeHead(200, {
      'Content-Type': 'text/event-stream',
      'Cache-Control': 'no-cache',
      'Connection': 'keep-alive'
    });
    res.write('\n');

    // Timeout timer, send a comment line every 20 sec
    var timer = setInterval(function() {
      res.write('event: ping' + '\n\n');
    }, 20000);

    client.subscribe(topic, function() {
      client.on('message', function(topic, msg, pkt) {
		    //res.write("New message\n");
        try {
		      var json = JSON.parse(msg);
          res.write("data: " + json.date + ": " + json.msg + "\n\n");
        } catch(err) {
          console.log(err);
        }
      });
    });
  });

});

module.exports = router;
