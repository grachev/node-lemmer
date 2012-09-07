var lemmer = require('../build/Release/node_lemmer.node');

exports.hello = function(str) {
	return lemmer.hello(str);
}
