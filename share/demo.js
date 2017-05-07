var share = require('./build/Release/shm.node');
var cluster = require('cluster');

//console.log( share.sub([1,2,3,4,5,"203","AB"]) );

if ( cluster.isMaster ) {
	cluster.fork();
} else {
	share.write('{name:"CK",age:20,company:"tencent"}') 
}
