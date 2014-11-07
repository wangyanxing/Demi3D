var srcPath = process.argv[2];

if (srcPath == undefined){
	console.log("Please specify the expression file.");
	process.exit(1);
}

var win = process.argv[3];

var fs = require('fs')

String.prototype.endWith=function(str){
if(str==null||str==""||this.length==0||str.length>this.length)
  return false;
if(this.substring(this.length-str.length)==str)
  return true;
else
  return false;
return true;
}

var iswin = false;
if(win == "win")
	iswin = true;

var walk = function(dir) {
    var results = []
    var list = fs.readdirSync(dir)
    list.forEach(function(file) {
    	//if(iswin)
        	file = dir + '/' + file;

        var stat = fs.statSync(file)
        if (stat && stat.isDirectory()) 
			results = results.concat(walk(file))
        else if((/\.(dds|png)$/i).test(file))
			results.push(file)
    })
    return results;
}


//var tool = "/Applications/Imagination/PowerVR/GraphicsSDK/PVRTexTool/CLI/OSX_x86/PVRTexToolCLI ";
var tool = "/Applications/Imagination/PowerVR_Graphics/PowerVR_Tools/PVRTexTool/CLI/OSX_x86/PVRTexToolCLI ";

var outbash = srcPath + "/";
outbash += iswin ? "conv.bat" : "conv.command";
var batFile = "";
if(!iswin){
	batFile += "#!/bin/bash\n";
}

var results = walk(srcPath)
console.log(results.length + " image files to process.")
for(var i=0; i < results.length; ++i){
	var file = results[i];
	if(file == undefined) continue;
	
	console.log(file);

	if(iswin) {
		if (file.endWith("_rxgb.dds")) {
			var basefile = file.replace(/_rxgb.dds/, "");
			var seecFile = basefile + "_s.dds";
			var changech = " -red " + file + ",a -green " + file + ",g -blue " + file + ",b -alpha " + seecFile + ",g";
			batFile += "pv -f BC2 -i " + file + changech + " -d " + basefile + ".png -o " + basefile + ".pvr\n";

		}else if (!file.endWith("_s.dds")) {
			// normal file
			var basefile = file.replace(/.dds/,"");
			batFile += "pv -f BC2 -i " + file + " -d " + basefile + ".png\n";
		}
	}else{
		batFile += tool + "-f PVRTC1_4 -i " + file + " -m\n";
	}
}

if (iswin) {
	batFile += "del " + srcPath + "\\*.dds" + " /f /s /q /a \n";
	batFile += "del " + srcPath + "\\*.pvr" + " /f /s /q /a \n";
}

fs.writeFile(outbash, batFile, function (err) {
 		if (err) throw err;
 			console.log('It\'s saved!');
 	});