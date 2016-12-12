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