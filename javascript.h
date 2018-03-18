const char javascript_file[] PROGMEM = R"=====(
 
	var pins_Mode = [1,1,1,1,1,1,9,9,9,9,9,9,1,1,1,1,1]; // 1 for INPUT , 0 for OUTPUT
	var pins_State =[1,0,1,0,1,1,9,9,9,9,9,9,1,1,1,1,1];
	xmlHttp=new XMLHttpRequest();
	var global_pin_number;
	var global_pin_number_2;
	var global_pin_number_3;
	var interval;
	var auto_read = 0; 
	var string_auto_read_01;
	var auto_read_speed = 2000;
	function drawChip(){
		var konstW = 230;
		var konstH = 400;
		var c = document.getElementById("esp");
		var ctx = c.getContext("2d");
		ctx.fillStyle = "#0066FF"; 
		ctx.fillRect(0,0,konstW,konstH); // board
		ctx.fillStyle = "#999999";
		ctx.fillRect(konstW/10, konstH/3.3, konstW-konstW/5, konstH*(1-1/3.3-0.03)); //chip
		ctx.fillStyle = "#CC9900";
		for(var a=0;a < 8;a++) // left pins
		{
			var sy= konstH/3.3 + a*konstH*(1-1/3.3)/8+ konstH/40;
			ctx.fillRect(0,sy,konstW/20,konstW/20);
		}
		for(var a=0;a < 8;a++) // right pins
		{
			var sy= konstH/3.3 + a*konstH*(1-1/3.3)/8+ konstH/40;
			ctx.fillRect(konstW*(1-1/20),sy,konstW/20,konstW/20);
		}
		ctx.fillRect(konstW/5.75,konstH/13.3,konstW/17.7,konstH/4.44); //antenna
		ctx.fillRect(konstW/5.75,konstH/13.3,konstW/5.75,konstH/30.75);
		ctx.fillRect(2*konstW/5.75,konstH/13.3,konstW/17.7,konstH/5);
		ctx.fillRect(2*konstW/5.75,konstH/13.3+konstH/5-konstH/30.75,konstW/5.75,konstH/30.75);
		ctx.fillRect(3*konstW/5.75,konstH/13.3,konstW/17.7,konstH/5);
		ctx.fillRect(3*konstW/5.75,konstH/13.3,konstW/3.8,konstH/30.75);
	}
	
	function drawIOstate(pin_number){
		var pinState = pins_Mode[pin_number];
		var pinName = "pin" + pin_number.toString();
		if( pinState== 1){
			// if input change to output
			document.getElementById(pinName).innerHTML = "INPUT";
			document.getElementById(pinName).style.background="#003366";
			document.getElementById(pinName).style.color = "#FFFFFF"
			
		}
		else if( pinState== 0){
			// if output change to input
			document.getElementById(pinName).innerHTML = "OUTPUT";
			document.getElementById(pinName).style.background="#33CCCC";
			document.getElementById(pinName).style.color = "#000000"		
		}
	}
	
	function setPins(pin_number){ 
		//var pinName = "pin" + pin_number.toString();			 
		if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
			var state; // change to "state"
			if(pins_Mode[pin_number] == 1){ state="OUTPUT";} // reversed logic ker sele kasnej spremenim 
			else {state="INPUT";}
			var url = "setPins?pin="+pin_number+"&state="+state;
			console.log("Sending setPins req with url: "+url);
			xmlHttp.open('PUT',url,true);
			xmlHttp.onreadystatechange=handleServerResponse_setPins;
			xmlHttp.send(null);
			console.log("setPins req sent ok?");
			global_pin_number = pin_number; // to be used in server response (cant get argument to work)
			
		}
	}
	function handleServerResponse_setPins(){
		if(xmlHttp.readyState==4 && xmlHttp.status==200){
			console.log("serverResponse_setPins")
			xmlResponse=xmlHttp.responseXML;
			xmldoc = xmlResponse.getElementsByTagName('response');
			message = xmldoc[0].firstChild.nodeValue;
			console.log("received from server (setPins): "+message);
			
			pins_Mode[global_pin_number]=parseInt(message); // save I/O to array
			drawIOstate(global_pin_number); // update table with IO states
			console.log(pins_Mode);
			global_pin_number =0;
			console.log("\n");
		}
	}
	
		
	
	function toggle_read_Pin(pin_number){
		//var pinName = "pin_io_"+ pin_number.toString();
		if(pins_Mode[pin_number] == 0) //if pin is OUTPUT
		{
			// send request to esp to change state of pin
			togglePin(pin_number);
		}
		else if(pins_Mode[pin_number] == 1) // if pin is INPUT
		{
			// send req to esp to read pin
			readPin(pin_number);
		}
	}
	
	function togglePin(pin_number){
		if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
			var state = !pins_State[pin_number];
			var url = "/togglePins?pin="+pin_number+"&state="+state;
			console.log("Sending togglePins req with url: "+url);
			xmlHttp.open('PUT',url,true);
			xmlHttp.onreadystatechange=handleServerResponse_togglePins;
			xmlHttp.send(null);
			console.log("togglePins req sent ok");
			global_pin_number_2 = pin_number; // set it to be used in response
			//pins_State[pin_number] = !pins_State[pin_number]; // mogoce v response
		}
	}
	function handleServerResponse_togglePins(){
		if(xmlHttp.readyState==4 && xmlHttp.status==200){
			xmlResponse=xmlHttp.responseXML;
			xmldoc = xmlResponse.getElementsByTagName('response');
			message = xmldoc[0].firstChild.nodeValue;
			console.log("received from server (togglePins): "+message);
			pins_State[global_pin_number_2] = parseInt(message); // change the state of pin
			drawPinState(global_pin_number_2); // update pin state on diagram
			global_pin_number_2 = 0;
			console.log(pins_State);
			console.log("\n");
		}
	}
	
	function readPin(pin_number){
		if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
			
			var url = "/readPins?pin="+pin_number;
			console.log("Sending readPins req with url: "+url);
			xmlHttp.open('PUT',url,true);
			xmlHttp.onreadystatechange=handleServerResponse_readPins;
			xmlHttp.send(null);
			console.log("readPins req sent ok");
			global_pin_number_3 = pin_number; // set it to be used in response
		}
	}
	function handleServerResponse_readPins(){
		if(xmlHttp.readyState==4 && xmlHttp.status==200){
			xmlResponse=xmlHttp.responseXML;
			xmldoc = xmlResponse.getElementsByTagName('response');
			message = xmldoc[0].firstChild.nodeValue;
			console.log("received from server (readPins): "+message);
			pins_State[global_pin_number_3] = parseInt(message); // change the state of pin to rec msg
			drawPinState(global_pin_number_3); // update pin state on diagram
			global_pin_number_3 = 0;
			console.log(pins_State);
			console.log("\n");
		}
	}
	
	function drawPinState(pin){
		var pinName = "pin_io_"+ pin.toString();
		//console.log(pinName);
		if(pins_State[pin]==1){
			document.getElementById(pinName).style.background = "#FF0000";
			document.getElementById(pinName).style.color = "#000000"
			document.getElementById(pinName).innerHTML = "1";
		}
		else if(pins_State[pin]==0){
			document.getElementById(pinName).style.background = "#000000";
			document.getElementById(pinName).style.color = "#FFFFFF"
			document.getElementById(pinName).innerHTML = "0";
		}
	}
		
	function toggleAutoRead(){
			console.log("toggling auto read \n");
			if(auto_read == 0){
				// enable auto read
				interval = setInterval(autoRead, auto_read_speed);
				document.getElementById("auto_read_button").innerHTML = "ON";
				document.getElementById("auto_read_button").style.background="#00ff00";
				
				auto_read = 1;
			}
			else{
				// disable auto read
				clearInterval(interval);
				document.getElementById("auto_read_button").innerHTML = "OFF";
				document.getElementById("auto_read_button").style.background="#ff0000";
				
				auto_read = 0;
			}
			
		}
		
		function autoRead(){
			console.log("auto reading pins now");
			var text="";
			for(var i=0;i<18;i++){
				if(pins_Mode[i]==0){text+="0";}
				else if(pins_Mode[i]==1){text+="1";}
			}
			console.log(text);
			
			if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
				
				var url = "/readMultiPins?pin="+text;
				console.log("Sending readMultiPins req with url: "+url);
				xmlHttp.open('PUT',url,true);
				xmlHttp.onreadystatechange=handleServerResponse_readMultiPins;
				xmlHttp.send(null);
				console.log("readMultiPins req sent ok");
				string_auto_read_01=text; // set it to be used in response
			}
		}
		function handleServerResponse_readMultiPins(){
			if(xmlHttp.readyState==4 && xmlHttp.status==200){
				xmlResponse=xmlHttp.responseXML;
				var xmldoc_response = xmlResponse.getElementsByTagName('response');
				console.log(xmldoc_response[0]);
				/* console.log(xmldoc_response[0].firstChild.nodeValue);
				console.log(xmldoc_response[0].lastChild.nodeValue); */
				console.log("received from server (readMultiPins): ");
				
				
				for(var i=4;i<10;i++){
					var message = xmldoc_response[0].childNodes[i].innerHTML;
					console.log(message);
				}
				var message = xmldoc_response[0].childNodes[4].innerHTML;
				if(message==1){pins_State[4]= 1;}
				else if(message==0){pins_State[4]=0;}
				
				var message = xmldoc_response[0].childNodes[5].innerHTML;
				if(message==1){pins_State[5]= 1;}
				else if(message==0){pins_State[5]=0;}
				
				var message = xmldoc_response[0].childNodes[6].innerHTML;
				if(message==1){pins_State[12]= 1;}
				else if(message==0){pins_State[12]=0;}
				
				var message = xmldoc_response[0].childNodes[7].innerHTML;
				if(message==1){pins_State[13]= 1;}
				else if(message==0){pins_State[13]=0;}
				
				var message = xmldoc_response[0].childNodes[8].innerHTML;
				if(message==1){pins_State[14]= 1;}
				else if(message==0){pins_State[14]=0;}
				
				var message = xmldoc_response[0].childNodes[9].innerHTML;
				if(message==1){pins_State[16]= 1;}
				else if(message==0){pins_State[16]=0;}
				
				drawPinState(4);
				drawPinState(5);
				drawPinState(12);
				drawPinState(13);
				drawPinState(14);
				drawPinState(16);
				
				console.log("\n");
			}
		}
	
	drawChip();
	// set all pins to INPUT
	drawIOstate(0);
	drawIOstate(1);
	drawIOstate(3);
	drawIOstate(4);
	drawIOstate(5);
	drawIOstate(12);
	drawIOstate(13);
	drawIOstate(14);
	drawIOstate(16);			
	
	drawPinState(0);
	drawPinState(1);
	drawPinState(3);
	drawPinState(4);
	drawPinState(5);
	drawPinState(12);
	drawPinState(13);
	drawPinState(14);
	drawPinState(16);
	
	console.log(pins_Mode);
	//sendTestReq()
	

 )=====";