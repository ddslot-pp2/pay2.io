var WebSocket = require('ws');
var ws = new WebSocket('ws://127.0.0.1:8080/echo');

ws.on('open', function() {
  var packet = {sender: 1234, msg: '행 world'};
  var json_packet = JSON.stringify(packet);

  
  //console.log(jp);
  ws.send(json_packet);
});

ws.on('message', function(message) {
  
  console.log('received: %s', message);
});
