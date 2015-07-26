# node-linuxcam
Capturing from webcam and converting to jpeg. Linux only (v4l2)

Installation:

```
npm install linuxcam
```

Usage:

```
var cam = require('linuxcam');
cam.start("/dev/video0", 160, 120);
var frame = cam.frame(); // ArrayBuffer
```

Simple SocketIO server example:

```
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var cam = require('linuxcam');

cam.start("/dev/video0", 160, 120);

function update(socket) {
  socket.emit("frame", cam.frame());
  setTimeout(function() {
    update(socket);
  }, 0);
}

io.on('connection', function(socket){
  socket.on('error', function(err){
    console.log("ERROR: "+err);
  });
  update(socket);
});

http.listen(9639, function(){
  console.log('listening on *:9639');
});

```
Client:
```
socket = io("http://localhost:9639");
var canvas = document.getElementById("canvas");
var ctx = canvas.getContext("2d");

var image = new Image();
image.onload = function() {
  ctx.drawImage(image, 0, 0, 160, 120);
};

socket.on('frame', function (frame) {
  image.src = "data:image/jpeg;base64," + arrayBufferToBase64(frame);
});

// ---- HTML ----
<canvas id="canvas" width="160" height="120"></canvas>
```
