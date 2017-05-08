(function($){ 
    $.extend({
        sendMsg: function(cmd, body){
			//alert("sendMsg"+1);
        	var message = $.buildMessage(cmd, body);
			//alert("sendMsg"+2);
			var resp;
			var str;
			message += "\r\n\r\n";
			//alert(message);
			console.log(message);
			$.ajax({
				url: "Login.cgi",
				async: false,
				type:"POST",
				timeout:1000,
				data: message
			}).done(function(response) {
				//console.log(response);
				//alert("response");
				var str = JSON.parse(response); 
				resp = {success:true,data:str.Param};
			}).fail(function(){
				//alert("fail");
				resp = {success:false,data:""};
			})		
			console.log(resp.data);
			return resp;
		},
		buildMessage: function(cmd, bodyMsg) {
        	var message = {
        		Header: {
				    Action: "Request",
        			Method: cmd,
	        		Session: ""
        		},
        		Param: bodyMsg
        	};

        	return JSON.stringify(message);
		},
		SetCookie: function (name,value){
			//alert("SetCookie 1");
			var str = name + "="+value+"&amp;";
			document.cookie = str;
			//alert("SetCookie 2");
			//console.log("setCookie:"+str);
			return ;
		},
		GetCookie: function(name){
			var arrstr = document.cookie.split("&amp; ");
			if(!arrstr)
			{
				return null;
			}
			//console.log("arrstr.length="+arrstr.length);
			for(var i = 0;i < arrstr.length;i++)
			{
				var temp = arrstr[i].split("=");
				//console.log(temp);
				if(temp[0] == name)
				{
					//console.log(temp[0]+"="+temp[1]);
					return $.SpliteCookie(temp[1]);
				}
					
			}
			return null;
		},
		SpliteCookie: function(value){
			//console.log("value="+value);
			len =  value.indexOf("&amp");
			//console.log("len="+len);
			if(len > 0){
				//console.log(">0");
				//console.log("str="+value.substr(value,len));
				return value.substr(value,len); 
			}
			//console.log("value");
			return value;
		}
	})
})(jQuery);





