var lemmer = require('./');

var lem = new lemmer.Lemmer();
var lemmas = lem.lemmatize(process.argv[2]);
lemmas.forEach(function(lemma) {
	console.log('Lemma: ' + lemma);
});
