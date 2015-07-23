// hello.js
var addon = require('./build/Release/v4l2.node');

module.exports.start = function() {
  addon.start();
};

module.exports.frame = function() {
  return addon.update();
};
