function fetchJsonFile(path, func)
{
	var xhttp = new XMLHttpRequest();
	xhttp.onload = function()
	{
		func(JSON.parse(this.responseText).filesystem,"initpath");	
	}

	xhttp.open('GET', path);
	xhttp.send();

}

function useJsonData(jsonObject, id)
{
	let subdir = jsonObject.subdir;
	const dir = [];
	const dir2 = [];
	const reg = [];
	let k = 0;
	let j = 0;
	for(let i in subdir)
	{
		if( subdir[i].isdir == 0)
		{
			reg[k] = subdir[i].path;
			k += 1;
		}

		else if( subdir[i].isdir  == 1)
		{
			dir[j] = subdir[i].path;
			dir2[j] = subdir[i];
			j += 1;
		}
	}
	let text = "";

	for(let n in dir)
	{
		if( id == "initpath" )
		{
			text += "<details><summary>" + dir[n] + "</summary><ul id= \""+ dir[n] + "\"></ul></details>";
		}
		else
		{
			text += "<li><details><summary>" + dir[n] + "</summary><ul id= \"" + dir[n] + "\"></ul></details></li>";
		}
	}
	for(let m in reg)
	{
		if( id == "initpath")
		{
			text += "<a href= \"filesystem/" + reg[m] + "\">" + reg[m] + "</a>\n";
		}
		else
		{
			text += "<li><a href= \"" + id + "/" + reg[m] + "\">" + reg[m] + "</a></li>\n";
		}

	}

	document.getElementById(id).innerHTML = text;
	
	for(let l in dir2)
	{
			let iin =  dir2[l].path ;
			useJsonData(dir2[l], iin);
	}
}

