var nativeLemmer = require('../build/Release/node_lemmer.node');
var path = require('path');

function Lemmer() {
	var dir = path.join(__dirname, '..', 'dict');
	this.lemmer = new nativeLemmer.Lemmer(path.join(dir, 'dict_russian.auto'),
		path.join(dir, 'paradigms_russian.bin'), path.join(dir, 'prediction_russian.auto'));
}

Lemmer.prototype.lemmatize = function(word) {
	return this.lemmer.lemmatize(word);
}

exports.Lemmer = Lemmer;
