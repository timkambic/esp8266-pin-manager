const char html_page[] PROGMEM = R"=====(
  
<!DOCTYPE html>
<html>
<head>
	<title>esp8266_12</title>
	
	<link rel="icon" href="data:;base64,iVBORwOKGO=" />  <!-- fake favicon.ico -->
</head>
<body>
	<style>
		h1 {
			text-align:center;
			background-color:#00CCFF;
		}
		td{
			font-weight:bold;
		}
		
	
		.io_spacer{
			width: 6em;
		}
		#espBoard {
			text-align:center;
		}
		#espBoard table{
			
			text-align: center;
			margin:auto;
		}
		#espBoard td{
			/*border: 1px solid black;*/
			height:1.85em;
		}
		#espBoard div{			
			border-radius: 4px;	
			cursor:pointer;
		}
		#espBoard div:hover{			
			border-radius: 6px;
			font-size: 120%;
		}
		#auto_read{
			font-weight:bold;
		}
		#auto_read_button {
			display: inline-block;
			border: 1px solid;
			border-radius:4px;
			width:4em;
			text-align:center;
			background:#ff0000;
			cursor:pointer;
		}
	</style>
	<h1>ESP8266-12 </h1>
	<div>Pins D1 and D3 are also used for SERIAL COMS !!! </div>
	<div id="auto_read">Auto read inputs <div id="auto_read_button" onclick="toggleAutoRead()"> OFF </div>  (0.5Hz)</div>
	<br>
	<div id="espBoard">
		
		<table>
			<tr> 
				<td class="io_spacer">I/O</td>
				<td>High/Low</td>
				<td style="height:121px"></td>
				<td rowspan="9"><canvas id="esp" width="230" height="400" ></canvas>  <!-- style="border:1px solid;" --></td>
				<td style="height:121px"></td>
				<td>High/Low</td>
				<td class="io_spacer">I/O</td>
			</tr>
			<tr> 
				<td></td>
				<td><div id="pin_io_rst"></div></td>
				<td>RESET</td>
				<td>D_1</td>
				<td><div id="pin_io_1" onclick="toggle_read_Pin(1)">a</div></td>
				<td><div id="pin1" > INPUT</div></td>
			</tr>
			<tr> 
				<td></td>
				<td><div id="pin_io_adc"></div></td>
				<td>ADC</td>
				<td>D_3</td>
				<td><div id="pin_io_3" onclick="toggle_read_Pin(3)">a</div></td>
				<td><div id="pin3" > INPUT</div></td>
			</tr>
			<tr> 
				
				<td colspan="2"><span id="pin_io_chpd">connect to Vcc</span></td>
				<td>CH_PD</td>
				<td>D_5</td>
				<td><div id="pin_io_5" onclick="toggle_read_Pin(5)"></div></td>
				<td><div id="pin5" onclick="setPins(5)"> INPUT</div></td>
			</tr>
			<tr> 
				<td><div id="pin16" onclick="setPins(16)"> INPUT</div></td>
				<td><div id="pin_io_16" onclick="toggle_read_Pin(16)"></div></td>
				<td>D_16</td>
				<td>D_4</td>
				<td><div id="pin_io_4" onclick="toggle_read_Pin(4)"></div></td>
				<td><div id="pin4" onclick="setPins(4)"> INPUT</div></td>
			</tr>
			<tr> 
				<td><div id="pin14" onclick="setPins(14)"> INPUT</div></td>
				<td><div id="pin_io_14" onclick="toggle_read_Pin(14)"></div></td>
				<td>D_14</td>
				<td>D_0</td>
				<td><div id="pin_io_0"></div></td>
				<td><div id="pin0" onclick="setPins(0)"> INPUT</div></td>
				<td>GND to Flash</td>
			</tr>
			<tr> 
				<td><div id="pin12" onclick="setPins(12)"> INPUT</div></td>
				<td><div id="pin_io_12" onclick="toggle_read_Pin(12)"></div></td>
				<td>D_12</td>
				<td>D_2</td>
				<td colspan="2"><span id="pin_io_2">connect to Vcc</span></td>
				
			</tr>
			<tr> 
				<td><div id="pin13" onclick="setPins(13)"> INPUT</div></td>
				<td><div id="pin_io_13" onclick="toggle_read_Pin(13)"></div></td>
				<td>D_13</td>
				<td>D_15</td>
				<td colspan="2"><span id="pin_io_15">connect to GND</span></td>
				
			</tr>
			<tr> 
				<td></td>
				<td><span id="pin_io_vcc">3.3V</span></td>
				<td>Vcc</td>
				<td>GND</td>
				<td><span id="pin_io_gnd">0V</span></td>
				<td></td>
			</tr>
		</table>
	</div>
	
	<script src="script.js"></script>
</body>
</html>

)=====";


