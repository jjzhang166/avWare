function SetSelected(id,value)
{	
	//alert(id+"  "+value);
	for(var i=0; i<document.getElementById(id).options.length; i++)
	{
		if(document.getElementById(id).options[i].value==value)
		{
			document.getElementById(id).options[i].selected=true;
			break;
		}
	}
}

function GetSelectedValue(id)
{
	for(var i=0; i<document.getElementById(id).options.length; i++)
	{
		if(document.getElementById(id).options[i].selected==true)
		{
			return {obj:document.getElementById(id).options[i],value:document.getElementById(id).options[i].value};
		}
	}
}