function GetVedioEncodeCab(channel,slave)
{
	var resp;
	var id_Compress = "ID_Chn"+channel+"Stream"+slave+"Compress";
	var id_Reslution = "ID_Chn"+channel+"Stream"+slave+"Reslution";
	var id_Fps = "ID_Chn"+channel+"Stream"+slave+"Fps";
	var id_Gop = "ID_Chn"+channel+"Stream"+slave+"Gop";
	var id_BitCtrlMode = "ID_Chn"+channel+"Stream"+slave+"BitCtrlMode";
	var id_Quility = "ID_Chn"+channel+"Stream"+slave+"Quility";
	var id_BitRate = "ID_Chn"+channel+"Stream"+slave+"BitRate";
	
	var addStr; 
	var msg={
		Channel:channel,
		Slave:slave
	};
	resp = $.sendMsg("GetVideoEncodeCaps",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;	
	$("#"+id_Compress).empty();
	for(var i = 0; i < data.Compress.length;i++){
		addStr  = '<option' +" "+'value='+'"'+ data.Compress[i]+'"'+ '>'+ data.Compress[i] + '<\/option>';
		$("#"+id_Compress).append(addStr);
	}
	
	$("#"+id_Reslution).empty();
	for(var i = 0; i < data.Resolution.length;i++){
		addStr  = '<option' +" "+'value='+'"'+ data.Resolution[i]+'"'+ '>'+ data.Resolution[i] + '<\/option>';	
		$("#"+id_Reslution).append(addStr);
	}
	
	$("#"+id_BitCtrlMode).empty();
	for(var i = 0; i < data.Bitctrl.length;i++){
		addStr  = '<option' +" "+'value='+'"'+ data.Bitctrl[i]+'"'+ '>'+ data.Bitctrl[i] + '<\/option>';
		$("#"+id_BitCtrlMode).append(addStr);
	}
	
	$("#"+id_Fps).empty();
	for(var i = data.Minframerate; i <= data.Maxframerate;i++){
		addStr  = '<option' +" "+'value='+'"'+i+'"'+'>'+i+ '<\/option>';
		$("#"+id_Fps).append(addStr);
	}
	$("#"+id_Quility).empty();
	for(var i = data.MinQLevel; i <= data.MaxQLevel;i++){
		addStr  = '<option' +" "+'value='+'"'+i+'"'+'>'+i+ '<\/option>';
		$("#"+id_Quility).append(addStr);
	}
	$("#"+id_Gop).empty();
	for(var i = data.MinGop; i <= data.MaxGop;i++){
		addStr  = '<option' +" "+'value='+'"'+i+'"'+'>'+i+ '<\/option>';
		$("#"+id_Gop).append(addStr);
	}
}

function GetVedioEncdeConfig(channel,slave)
{
	var resp;
	var id_Compress = "ID_Chn"+channel+"Stream"+slave+"Compress";
	var id_Reslution = "ID_Chn"+channel+"Stream"+slave+"Reslution";
	var id_Fps = "ID_Chn"+channel+"Stream"+slave+"Fps";
	var id_Gop = "ID_Chn"+channel+"Stream"+slave+"Gop";
	var id_BitCtrlMode = "ID_Chn"+channel+"Stream"+slave+"BitCtrlMode";
	var id_Quility = "ID_Chn"+channel+"Stream"+slave+"Quility";
	var id_BitRate = "ID_Chn"+channel+"Stream"+slave+"BitRate";
	var msg={
		Channel:channel,
		Slave:slave
	};
	resp = $.sendMsg("GetVideoEncodeProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;
	SetSelected(id_BitCtrlMode,data.Bitctrl);
	SetSelected(id_Compress,data.Compress);
	SetSelected(id_Fps,data.FrameRate);
	SetSelected(id_Gop,data.GOP);
	SetSelected(id_Reslution,data.Resolution);
	SetSelected(id_Quility,data.QLevel);
	$("#"+id_BitRate)[0].value = data.BitctrlCbrValue;
}

function SetVedioEncodeConfig(channel,slave)
{
	var id_Compress = "ID_Chn"+channel+"Stream"+slave+"Compress";
	var id_Reslution = "ID_Chn"+channel+"Stream"+slave+"Reslution";
	var id_Fps = "ID_Chn"+channel+"Stream"+slave+"Fps";
	var id_Gop = "ID_Chn"+channel+"Stream"+slave+"Gop";
	var id_BitCtrlMode = "ID_Chn"+channel+"Stream"+slave+"BitCtrlMode";
	var id_Quility = "ID_Chn"+channel+"Stream"+slave+"Quility";
	var id_BitRate = "ID_Chn"+channel+"Stream"+slave+"BitRate";
	var msg;
	if(GetSelectedValue(id_BitCtrlMode).value == "VBR"){
		msg={
		Channel:channel,
		Slave:slave,
		Bitctrl:GetSelectedValue(id_BitCtrlMode).value,
		Compress:GetSelectedValue(id_Compress).value,
		FrameRate:parseInt(GetSelectedValue(id_Fps).value),
		GOP:parseInt(GetSelectedValue(id_Gop).value),
		QLevel:parseInt(GetSelectedValue(id_Quility).value),
		Resolution:GetSelectedValue(id_Reslution).value
		}
	}else if(GetSelectedValue(id_BitCtrlMode).value == "CBR"){
		msg={
		Channel:channel,
		Slave:slave,
		Bitctrl:GetSelectedValue(id_BitCtrlMode).value,
		Compress:GetSelectedValue(id_Compress).value,
		FrameRate:parseInt(GetSelectedValue(id_Fps).value),
		GOP:parseInt(GetSelectedValue(id_Gop).value),
		BitctrlCbrValue:parseInt($("#"+id_BitRate)[0].value),
		Resolution:GetSelectedValue(id_Reslution).value
		}
	}
	var resp = $.sendMsg("SetVideoEncodeProfile",msg);
	if(false == resp.success){
		alert("设置参数失败");
		return ;
	}
	GetVedioEncodeCab(channel,slave);
	GetVedioEncdeConfig(channel,slave);
}

function GetVedioCaptureCaps(chn,slave)
{
	var addStr; 
	var msg={
		Channel:chn,
		Slave:slave
	};
	var resp = $.sendMsg("GetVideoCaptureCaps",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;	
	$("#ID_PowerFrequency").empty();
	for(var i = 0; i < data.Antiflicker.length;i++){
		addStr  = '<option' +" "+'value='+'"'+ data.Antiflicker[i]+'"'+ '>'+ data.Antiflicker[i] + '<\/option>';
		$("#ID_PowerFrequency").append(addStr);
	}
	$("#ID_IRcut").empty();
	for(var i = 0; i < data.IrCut.length;i++){
		addStr  = '<option' +" "+'value='+'"'+ data.IrCut[i]+'"'+ '>'+ data.IrCut[i] + '<\/option>';
		$("#ID_IRcut").append(addStr);
	}
	
	/*for(var i = 0; i < data.WhiteBalance.length;i++){
		addStr  = '<option' +" "+'value='+'"'+ data.WhiteBalance[i]+'"'+ '>'+ data.WhiteBalance[i] + '<\/option>';
		$("#ID_WhiteBlance").append(addStr);
	}*/
}

function GetVedioCapture(channel,slave)
{
	var msg={
		Channel:channel,
		Slave:slave
	};
	var resp = $.sendMsg("GetVideoCaptureProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;
	$("#ID_VedioCaptureHorreverse")[0].checked = data.Horreverse;
	$("#ID_VedioCaptureVerreverse")[0].checked = data.Verreverse;
	SetSelected("ID_PowerFrequency",data.Antiflicker);
	SetSelected("ID_IRcut",data.IrCut);
}

function  SetVedioCapture(channel,slave)
{
	//alert($("#ID_VedioCaptureHorreverse")[0].checked);
	//alert($("#ID_VedioCaptureVerreverse")[0].checked);
	var msg={
		Channel:channel,
		Slave:slave,
		Antiflicker:GetSelectedValue("ID_PowerFrequency").value,
		IrCut:GetSelectedValue("ID_IRcut").value,
		Horreverse:Number($("#ID_VedioCaptureHorreverse")[0].checked),
		Verreverse:Number($("#ID_VedioCaptureVerreverse")[0].checked)
	};
	var resp = $.sendMsg("SetVideoCaptureProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	GetVedioCapture(0,0);
}

function InitVedioEncodeUi()
{
	GetVedioEncodeCab(0,0);
	GetVedioEncodeCab(0,1);
	GetVedioEncdeConfig(0,0);
	GetVedioEncdeConfig(0,1);
}

function InitVedioCaptureUi()
{
	GetVedioCaptureCaps(0,0);
	GetVedioCapture(0,0);
}

