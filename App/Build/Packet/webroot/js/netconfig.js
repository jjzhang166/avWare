//RTMP
function  SetRtmp(channel,slave)
{
	var msg={
		Channel:channel,
		Slave:slave,
		Enable:Number($("#ID_RtmpEnable")[0].checked),
		RtmpAddr:$("#ID_RtmpAddr")[0].value,
		RtmpString:$("#ID_RtmpString")[0].value,
		RtmpAudio:Number($("#ID_RtmpAudio")[0].checked)
	};
	var resp = $.sendMsg("SetRtmpProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
}

function GetRtmp(channel,slave)
{
	var resp;
	var msg={
		Channel:channel,
		Slave:slave
	};
	resp = $.sendMsg("GetRtmpProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;
	$("#ID_RtmpAddr")[0].value = data.RtmpAddr;
	$("#ID_RtmpString")[0].value = data.RtmpString;
	$("#ID_RtmpAudio")[0].checked = data.RtmpAudio;
	$("#ID_RtmpEnable")[0].checked = data.Enable;
}

function RtmpSubmit()
{
	var chn;
	var streamType;
	chn = parseInt(GetSelectedValue("ID_RtmpChnnel").value);
	streamType = parseInt(GetSelectedValue("ID_RtmpStreamType").value);
	SetRtmp(chn,streamType);
	return 0;
}

function InitRtmpUi()
{
	var chn;
	var streamType;
	chn = parseInt(GetSelectedValue("ID_RtmpChnnel").value);
	streamType = parseInt(GetSelectedValue("ID_RtmpStreamType").value);
	GetRtmp(chn,streamType);
	return ;
}

//RTMP
function  SetRtmp(channel,slave)
{
	var msg={
		Channel:channel,
		Slave:slave,
		Enable:Number($("#ID_RtmpEnable")[0].checked),
		RtmpAddr:$("#ID_RtmpAddr")[0].value,
		RtmpString:$("#ID_RtmpString")[0].value,
		RtmpAudio:Number($("#ID_RtmpAudio")[0].checked)
	};
	var resp = $.sendMsg("SetRtmpProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
}

function GetRtmp(channel,slave)
{
	var resp;
	var msg={
		Channel:channel,
		Slave:slave
	};
	resp = $.sendMsg("GetRtmpProfile",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;
	$("#ID_RtmpAddr")[0].value = data.RtmpAddr;
	$("#ID_RtmpString")[0].value = data.RtmpString;
	$("#ID_RtmpAudio")[0].checked = data.RtmpAudio;
	$("#ID_RtmpEnable")[0].checked = data.Enable;
}

function RtmpSubmit()
{
	var chn;
	var streamType;
	chn = parseInt(GetSelectedValue("ID_RtmpChnnel").value);
	streamType = parseInt(GetSelectedValue("ID_RtmpStreamType").value);
	SetRtmp(chn,streamType);
	return 0;
}
//FTP
function  SetFtpConfig()
{
	var msg={
		FtpEnable:Number($("#ID_FTPEnable")[0].checked),
		FtpSeverAddr:$("#ID_FTPServerAddr")[0].value,
		FtpServerPort:Number($("#ID_FTPServerPort")[0].value),
		UsrName:$("#ID_FTPUsrName")[0].value,
		PassWord:$("#ID_FTPPassWord")[0].value,	
		FtpDir:$("#ID_FTPDir")[0].value	
	};
	var resp = $.sendMsg("SetFtpConfig",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
}

function GetFtpConfig()
{
	var resp;
	var msg={
	};
	resp = $.sendMsg("GetFtpConfig",msg);
	if(false == resp.success){
		alert("获取参数失败");
		return ;
	}
	var data = resp.data;
	$("#ID_FTPServerAddr")[0].value = data.FtpSeverAddr;
	$("#ID_FTPServerPort")[0].value = data.FtpServerPort;
	$("#ID_FTPUsrName")[0].value = data.UsrName;
	$("#ID_FTPPassWord")[0].value = data.PassWord;
	$("#ID_FTPDir")[0].value = data.FtpDir;
	
	$("#ID_FTPEnable")[0].checked = data.FtpEnable;
}

function FTPSubmit()
{
	SetFtpConfig();
	return 0;
}

function InitFTPUi()
{
	GetFtpConfig();
	return 0;
}


function GetWifiConfig()
{
	var resp;
	var msg={
	};
	resp = $.sendMsg("GetWifiConfig",msg);
	if(false == resp.success){
		alert("获取WIFI参数失败");
		return ;
	}
	var data = resp.data;
	//var data = {"WIFIPW":"32131dsa","WIFISSID":"test_json","WIFIState":"0","WifiDhcp":true,"WifiEnable":1,"WifiGateWay":"192.168.2.1","WifiIPAddr":"192.168.2.36","WifiMask":"255.255.255.0"};
	
	$("#ID_WifiEnable")[0].checked = data.WifiEnable;
	$("#ID_WifiDhcp")[0].checked = data.WifiDhcp;
	//$("#ID_WIFIState")[0].value = data.WIFIState;
	SetSelected("ID_WIFIState",Number(data.WIFIState));
	$("#ID_WIFISSID")[0].value = data.WIFISSID;
	$("#ID_WIFIPW")[0].value = data.WIFIPW;
	$("#ID_WifiIPAddr")[0].value = data.WifiIPAddr;
	$("#ID_WifiGetWay")[0].value = data.WifiGateWay;
	$("#ID_WifiMask")[0].value = data.WifiMask;
	$("#ID_APSSID")[0].value = data.APSSID;
	$("#ID_APPW")[0].value = data.APPW;
}

function  SetWifiConfig()
{
	var msg;
	if($("#ID_WIFIState")[0].value == 1){
		if($("#ID_WifiDhcp")[0].checked == false){
			msg = {
				WifiEnable:Number($("#ID_WifiEnable")[0].checked),
				WIFIState:$("#ID_WIFIState")[0].value,
				WIFISSID:$("#ID_WIFISSID")[0].value,
				WIFIPW:$("#ID_WIFIPW")[0].value,
				WifiDhcp:$("#ID_WifiDhcp")[0].checked,	
				WifiIPAddr:$("#ID_WifiIPAddr")[0].value,
				WifiGateWay :$("#ID_WifiGetWay")[0].value,
				WifiMask:$("#ID_WifiMask")[0].value,
				APSSID:$("#ID_APSSID")[0].value,
				APPW:$("#ID_APPW")[0].value
			};
		}else{
				msg = {
				WifiEnable:Number($("#ID_WifiEnable")[0].checked),
				WIFIState:$("#ID_WIFIState")[0].value,
				WIFISSID:$("#ID_WIFISSID")[0].value,
				WIFIPW:$("#ID_WIFIPW")[0].value,
				WifiDhcp:$("#ID_WifiDhcp")[0].checked,
				APSSID:$("#ID_APSSID")[0].value,
				APPW:$("#ID_APPW")[0].value
			};
			
		}
	}else{
		msg = {
			WifiEnable:Number($("#ID_WifiEnable")[0].checked),
			WIFIState:$("#ID_WIFIState")[0].value,
			WIFISSID:$("#ID_WIFISSID")[0].value,
			WIFIPW:$("#ID_WIFIPW")[0].value,	
			APSSID:$("#ID_APSSID")[0].value,
			APPW:$("#ID_APPW")[0].value
		};
	}
	var resp = $.sendMsg("SetWifiConfig",msg);
	if(false == resp.success){
		alert("设置WIFI参数失败");
		return ;
	}
}

function WifiModeOnchange()
	{
		var obj = document.getElementById("ID_WIFIState");
		if(0 == obj.value){
			document.getElementById("ID_WifiStationInput").style.display = "none";
			document.getElementById("ID_WIFISSID").style.display = "none";
			document.getElementById("ID_WIFIPW").style.display = "none";
			document.getElementById("ID_APSSID").style.display = "block";
			document.getElementById("ID_APPW").style.display = "block";	
		}			
		else{
			document.getElementById("ID_WifiStationInput").style.display = "block";
			document.getElementById("ID_WIFISSID").style.display = "block";
			document.getElementById("ID_WIFIPW").style.display = "block";	
			document.getElementById("ID_APSSID").style.display = "none";
			document.getElementById("ID_APPW").style.display = "none";			
		}
	}
	
function WifiDhcpOnchange()
{
	var obj = document.getElementById("ID_WifiDhcp");
	if(true == obj.checked){
		document.getElementById("ID_WifiIPAddr").disabled = "disabled";
		document.getElementById("ID_WifiGetWay").disabled = "disabled";
		document.getElementById("ID_WifiMask").disabled = "disabled";
	}else{
		$('#ID_WifiIPAddr').attr("disabled",false); 
		$('#ID_WifiGetWay').attr("disabled",false); 
		$('#ID_WifiMask').attr("disabled",false); 
	}
}

function InitWifiUi()
{
	GetWifiConfig();
	WifiModeOnchange();
	WifiDhcpOnchange();
	return 0;
}

function WifiSubmit()
{
	SetWifiConfig();
	return 0;
}