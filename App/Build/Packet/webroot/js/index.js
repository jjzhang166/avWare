function GetImageProfile(chn,slave)
{
	var msg={
		Channel:chn,
		Slave:slave
	};
	var resp = $.sendMsg("GetImageProfile",msg);
	if(false == resp.success){
		alert("获取图像参数失败");
		return ;
	}
	var data = resp.data;	
	
	$( "#ID_BrightValue" ).val(data.Brightness);
	$( "#ID_ChromaValue" ).val(data.Hue );
	$( "#ID_ContrastValue" ).val(data.Contrast );
	$( "#ID_SaturationValue" ).val(data.Saturation );
	$( "#ID_BrightSlibar" ).slider( "value", data.Brightness);
	$( "#ID_ChromaSlibar" ).slider( "value", data.Hue );
	$( "#ID_ContrastSlibar" ).slider( "value", data.Contrast );
	$( "#ID_SaturationSlibar" ).slider( "value", data.Saturation );
	return ;
}
function SetImageProfile(chn,slave)
{
	var	msg={
		Channel:0,
		Slave:0,
		Brightness:parseInt($("#ID_BrightValue")[0].value),
		Contrast:parseInt($("#ID_ContrastValue")[0].value),
		Saturation:parseInt($("#ID_SaturationValue")[0].value),
		Hue:parseInt($("#ID_ChromaValue")[0].value),
		};
	var resp = $.sendMsg("SetImageProfile",msg);
	if(false == resp.success){
		alert("设置参数失败");
		return ;
	}
}

function InitImageSliderBar()
{
	GetImageProfile(0,0);
	
}

function PtzDriction(cmd)
{
	var req;
	var	msg={
		Channel:0,
		Slave:0,
		Cmd:cmd,
		HSpeed:10,
		VSpeed:10,
	};
	if(cmd >= 2 && cmd <= 17){
		req = "CallPtzPt";
	}else if(cmd >= 108 && cmd <= 111){
		req = "CallPtzZoom";
	}else if(cmd >= 104 && cmd <= 107){
		req = "CallPtzFocus";
	}else if(cmd >= 100 && cmd <= 103){
		req = "CallPtzIris";
	}
	var resp = $.sendMsg(req,msg);
	if(false == resp.success){
		alert("设置云台失败");
		return ;
	}
}