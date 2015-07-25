// hello.js
var cam = require('./build/Release/v4l2.node');

module.exports.start = function(dev, width, height) {
  cam.start(dev, width, height);
};

module.exports.frame = function() {
  return cam.frame();
};
