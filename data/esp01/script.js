function onLoad(){
  // Add event listener to the LED checkbox (the function will be called on every change)
  let switchOnBoardLed = document.getElementById('toggle-OB-led');
  if(switchOnBoardLed != undefined){
   switchOnBoardLed.addEventListener('change', toggleLed ); 
  }else{
    console.log("DOM Element not found!");
  }
  let switchLeds = document.getElementById('toggle-leds');
  if(switchLeds != undefined){
   switchLeds.addEventListener('change', toggleLeds ); 
  }else{
    console.log("DOM Element not found!");
  }
  reflesh();
  setInterval(reflesh, 1000);
}

function toggleLed() {
  const pars = new URLSearchParams({
    val:  document.getElementById('toggle-OB-led').checked ? '1' : '0'
  });
  
  fetch('/led?' + pars )                // Do the request
  .then(response => response.text())    // Parse the response 
  .then(text => {                       // DO something with response
    console.log(text);
  });
}
   

function reflesh(){
  let rec = new XMLHttpRequest();
  rec.open('GET', "/mainPageEndpoint");
  rec.send();
  rec.onload = () => {
    if(rec.responseType == ""){
      //console.log(rec.responseText);
      let jsonObj = JSON.parse(rec.responseText);
      //console.log('rec.responseText', rec.responseText)
      let cpuTemp = jsonObj.cpuTemp;
      let temp = jsonObj.temp;
      //console.log('ledList', ledList)
      let cpuTempField = document.getElementById("cpu-temp");
      let tempField = document.getElementById("temp");
      cpuTempField.innerText = cpuTemp + " C°";
      tempField.innerText = temp + " C°";
    }else{
      console.error("error type");
      console.log("type:", rec.responseType);
      console.log("response:", rec.response);
      console.log("responseText:", rec.responseText);
    }
  };
}