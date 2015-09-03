# node-linuxcam
Capturing images from webcam and converting to rgb24. Linux only (v4l2)

## Installation
```javascripts
npm install linuxcam
```

## Usage

```javascript
var cam = require('linuxcam');
cam.start("/dev/video0", 320, 240);
var frame = cam.frame(); // Buffer
```

The buffer contains rgb24 data and you can convert it using jpeg-fresh module.

```javascript
var Jpeg = require('jpeg-fresh').Jpeg;
var jpeg = new Jpeg(frame.data, frame.width, frame.height, 'rgb');
var jpeg_image = jpeg.encodeSync(); // Buffer
```

## Simple live streaming example with SocketIO

```javascript
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var cam = require('linuxcam');
var Jpeg = require('jpeg-fresh').Jpeg;

cam.start("/dev/video0", 320, 240);

function update(socket) {
  var frame = cam.frame();
  var jpeg = new Jpeg(frame.data, frame.width, frame.height, 'rgb');
  var jpeg_frame = jpeg.encodeSync();
  socket.emit("frame", jpeg_frame.toString('base64'));
  setTimeout(function() {
    update(socket);
  }, 40);
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
## Client
```html
<!DOCTYPE html>
<html>
  <head>
    <script src="https://cdn.socket.io/socket.io-1.3.5.js"></script>
  <head>
  <body>
    <canvas id="canvas" width="320" height="240"></canvas>
    <script>
      socket = io("http://localhost:9639");
      var canvas = document.getElementById("canvas");
      var ctx = canvas.getContext("2d");

      var image = new Image();
      image.onload = function() {
        ctx.drawImage(image, 0, 0, 320, 240);
      };

      socket.on('frame', function (frame) {
        image.src = "data:image/jpeg;base64," + frame;
      });
    </script>
  </body>
</html>

```

## MIT license