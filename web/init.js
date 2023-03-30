(async ()=>{
	const fs = require("fs");

	if(!fs.existsSync("./lib")) 
		fs.mkdirSync("./lib");

	fetch("https://cdnjs.cloudflare.com/ajax/libs/vue/3.2.47/vue.esm-browser.js")
	.then(res => res.text())
	.then(res => fs.writeFileSync("./lib/vue.esm-browser.js", res));
})();
