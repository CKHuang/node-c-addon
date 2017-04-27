var share = require('./build/Release/share.node');

console.log(share.add(3,10,20,30,40));

share.exports(function(){
	console.log(arguments);
});