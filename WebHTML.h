const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0">
    <title>ESP Web Tool</title>
    <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>

    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      .content {padding: 8px;margin:0px 0px;}
      .card { background-color: white; 
        box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
        border-radius: 10px;
        width:95%;
        margin-left: auto;
        margin-right: auto;
      }
      
      input{
        background:#f1f1f1;
        border:0;
        padding: 10px 25px;
        margin: 4px 2px;
        width:50%;
      }
      
      .button {
        border: none;
        color: white;
          padding: 10px 5px;        
        margin: 4px 2px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        cursor: pointer;
        font-weight: bold;
        opacity: 0.8;
        
      }
      .button:hover {
        opacity: 1;
      }
      
      .button::after{
        clear:both;
      }
      
      .button1 {background-color: #4CAF50;width: 85px;} /* Green */
        .button2 {background-color: #FF0500;width: 120px;}
      
      .pclass{ background-color: #DCDCDC; black;color:black;height: 300px;margin:5px;border-radius: 5px;text-align: left;font-size: 1.0rem;overflow-y: scroll;}
    
      .hsection {
        background-color: #04456f;
        padding-top: 1px;
        color: white;
      }
      
      #file-input{background:#f1f1f1;border:0;padding: 10px 5px;margin: 4px 2px;}
      #bar,#prgbar{background-color:#f1f1f1;border-radius:10px;margin: 0px 5px;}
      #bar{background-color:#FF0500;width:0%;height:10px;}
  </style>
  </head>

<body style="margin:0px 0px;">
  <div class="content" >
    <div class="card" >   
      <div class="hsection">
        <h2>ESP Web Tool</h2>
        <hr />
      </div>
      
      <form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
        <div align="center" style="padding: 5px 5px;">
          <input type='file' name='update' id='file' onchange='sub(this)' style="display:none;"> </input>
          <label id='file-input' for='file' style="padding: 9px 5%;">Choose Firmware file</label>
          <input type='submit' class="button button2" value='Upload'>
        </div>
        
        <div id='prg'>
        
        </div>
        
        <div id='prgbar' >
          <div id='bar'></div>
        </div>        
      </form>
      
      <div class="pclass">
        <p id="edata" placeholder="Recived a message..."></p>
      </div>
      <div align="left" style="padding: 10px 5px;"> 
        <input type="text" id="message" placeholder="Type a message..." autocomplete="off" style="height: 21px;" />
        <button type="submit" class="button button1" onclick="onPress()">Send</button>   
      </div>
      <br>
    </div>
  </div>

<script language="javascript" type="text/javascript">

//var url = "ws://192.168.1.2:1337/";
var url = 'ws://'+location.hostname+':1337/'
var textinput;
var canvas;
var context;
var dis;
// This is called when the page finishes loading
function init() {

    textinput = document.getElementById("message");
    // Connect to WebSocket server
    wsConnect(url);
}

// Call this to connect to the WebSocket server
function wsConnect(url) {
    
  
    // Connect to WebSocket server
    websocket = new WebSocket(url);
    
    // Assign callbacks
    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}

// Called when a WebSocket connection is established with the server
function onOpen(evt) {
    // Log connection state
    console.log("Connected");
    if(dis){
      console.log("After Disconnect Reload page");  
      window.location.reload();
      dis=0;
    }
}

// Called when the WebSocket connection is closed
function onClose(evt) {

    // Log disconnection state
    console.log("Disconnected");
    dis=1;
    // Try to reconnect after a few seconds
    setTimeout(function() { wsConnect(url) }, 2000);
}

// Called when a message is received from the server
function onMessage(evt) {

    // Print out our received message
    var time = new Date().toLocaleTimeString('en-US',{ hour12: false });      
    console.log("Received: " + evt.data);
    document.getElementById("edata").innerHTML= time +" -> "+evt.data + "<br>" + document.getElementById("edata").innerHTML;
   
}

// Called when a WebSocket error occurs
function onError(evt) {
    console.log("ERROR: " + evt.data);
}

// Sends a message to the server (and prints it to the console)
function doSend(message) {
    
  console.log("Sending: " + message);
    websocket.send(message);
}

// Called whenever the HTML button is pressed
function onPress() {
    var mmssgg = document.getElementById("message").value;
  if(mmssgg.length>0){
    doSend(mmssgg);
    document.getElementById("message").value="";
  }
}

function sub(obj){
  var fileName = obj.value.split('\\\\');
  document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];
};
$('form').submit(function(e){
  e.preventDefault();
  var form = $('#upload_form')[0];
  var data = new FormData(form);
  $.ajax({
    url: '/update',
    type: 'POST',
    data: data,
    contentType: false,
    processData:false,
    xhr: function() {
      var xhr = new window.XMLHttpRequest();
      xhr.upload.addEventListener('progress', function(evt) {
        if (evt.lengthComputable) {
          var per = evt.loaded / evt.total;
          $('#prg').html('Progress: ' + Math.round(per*100) + '%');
          $('#bar').css('width',Math.round(per*100) + '%');
        }
      }, false);
      return xhr;
    },
    success:function(d, s) {
      console.log('success!')
    },
    error: function (a, b, c) {
    }
  });
});
// Call the init function as soon as the page loads
window.addEventListener("load", init, false);

</script>
</body>
</html>)rawliteral";
