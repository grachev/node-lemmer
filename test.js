var lemmer = require('./');

var lem = new lemmer.Lemmer();
lem.lemmatize(process.argv[2]);
