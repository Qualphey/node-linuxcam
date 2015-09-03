// hello.js
var cam = require('./build/Release/v4l2.node');

module.exports.start = function(dev, width, height) {
  cam.start(dev, width, height);
};

module.exports.frame = function() {
  var iframe = cam.frame();
  var frame = {
    data: to_buffer(iframe[0]),
    width: iframe[1],
    height: iframe[2],
  }
  return frame;
};

function to_buffer(ab) {
  var buffer = new Buffer(ab.byteLength);
  var view = new Uint8Array(ab);
  for (var i = 0; i < buffer.length; ++i) {
    buffer.writeUInt8(view[i], i);
  }
  return buffer;
}
