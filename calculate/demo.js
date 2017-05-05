var sum = require('./build/Release/sum.node');

console.time('c++ sum');
for( var i = 0 ; i < 20000; i ++ ) 
{
	sum.sum(100,210);
}
console.timeEnd('c++ sum');



console.time('js sum');
var c;
for( var i = 0 ; i < 20000; i ++ ) 
{
	c = 100 + 200;
}
console.timeEnd('js sum');