function getVLC(name)   
{   
        if (window.document[name])      
        {
                return window.document[name];   
        }   
        if (navigator.appName.indexOf("Microsoft Internet")==-1)   
        {
                if (document.embeds && document.embeds[name])   
                        return document.embeds[name];      
        }   
        else   
        {   
                return document.getElementById(name);   
        }  
		// return document.getElementById(name);   
} 

function PluginChanggeSize(width,height)
{
	console.log(width);
	console.log(height);
	$("#vlc").width(width);
	$("#vlc").height(height);	
}

function PluginStreamStop()   
{   
     getVLC("vlc").playlist.stop();   
}
 
function GetIp()
{
	var url = document.location.href;
	var ipAddr;
	url.replace(/[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}/,function(value){ipAddr = value;});
	if(null == ipAddr){
		//alert("Not Found IP");
		return  "192.168.6.30";

	}
	//alert(ipAddr);
	return ipAddr;
												
}

function PluginStreamPlay(StreamTye)   
{   
	var mrl;
	var ip = GetIp();
	if(ip == null){
		return ;
	}
	var port = 554;
	var mrl = 'rtsp://'+ip+':' +port+'type=0&amp;id='+StreamTye;
	console.log(mrl);
	var vlc = getVLC("vlc");
	var options = new Array("rtsp-tcp")
	console.log(vlc);
	itemId= vlc.playlist.add(mrl, "fancy name", options);	
	//lc.playlist.add(mrl);   
	vlc.playlist.playItem(itemId);   
}  

