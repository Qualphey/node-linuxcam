# node-linuxcam
Capturing images from webcam and converting to rgb24. Linux only (v4l2)

#Installation
```
npm install linuxcam
```

#Usage

```
var cam = require('linuxcam');
cam.start("/dev/video0", 320, 240);
var frame = cam.frame(); // Buffer
```

The buffer contains rgb24 data and you can convert it using jpeg-fresh module.

```
var Jpeg = require('jpeg-fresh').Jpeg;
var jpeg = new Jpeg(frame.data, frame.width, frame.height, 'rgb');
var jpeg_image = jpeg.encodeSync(); // Buffer
```

#Simple SocketIO server example

```
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var cam = require('linuxcam');
var Jpeg = require('jpeg-fresh').Jpeg;

cam.start("/dev/video0", 160, 120);

function update(socket) {
  var frame = cam.frame();
  var jpeg = new Jpeg(frame.data, frame.width, frame.height, 'rgb');
  var jpeg_frame = jpeg.encodeSync();
  socket.emit("frame", jpeg_frame.toString('base64'));
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
#Client
```
socket = io("http://localhost:9639");
var canvas = document.getElementById("canvas");
var ctx = canvas.getContext("2d");

var image = new Image();
image.onload = function() {
  ctx.drawImage(image, 0, 0, 160, 120);
};

socket.on('frame', function (frame) {
  image.src = "data:image/jpeg;base64," + frame;
});

// ---- HTML ----
<canvas id="canvas" width="160" height="120"></canvas>
```
