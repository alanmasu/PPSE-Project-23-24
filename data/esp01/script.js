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
  document.getElementById("ledsForm").addEventListener("submit", function (e) {
    e.preventDefault();
    setLeds();
  });
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

function toggleLeds() {
  const pars = new URLSearchParams({
    val:  document.getElementById('toggle-leds').checked ? '1' : '0'
  });
  
  fetch('/rgbLed?' + pars )                // Do the request
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

function setLeds(){
  let form = document.getElementById('ledsForm');
  let formData = new FormData(form);
  // console.log(Object.fromEntries(formData));
  let values = Object.fromEntries(formData);
  console.log(values)
  if (isNaN(values.count) || values.count == ""){
    alert("Numero di LED deve essere numerico");
    console.log("imput non numerico")
  }else{
    // console.log(formData);
    let toSend = "color=" + parseInt(values.color.substring(1), 16) + "&count=" + values.count + "\n";
    let rec = new XMLHttpRequest();
    rec.open('POST', "/setLeds", true);
    rec.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    rec.setRequestHeader("Content-length", toSend.length);
    rec.send(toSend);
    // rec.send(formData);
  }
  
}
// function buttonClick(element){
//     //Modifico me stesso
//     element.dataset.state = element.dataset.state == '1' ? '0': '1' ;
//     let rec = new XMLHttpRequest();
//     let list = [];
//     let msg = "";
//     let buttons = document.querySelectorAll(".led-button");
//     if(buttons !== undefined){
//         console.log("buttons", buttons);
//         if(buttons.length > 0){
//             buttons.forEach(function(btn){
//                 list.push({pin: btn.dataset.pin, state: btn.dataset.state});
//             });
//         }
//     }
//     console.log("list", list);
//     let jsonObj = {leds: list};
//     msg = JSON.stringify(jsonObj);
//     rec.open('POST', "/ledState", true);
//     rec.setRequestHeader('Content-type', 'application/json');
//     rec.send(msg);
// }

// function saveConfig(element){
//     let rec = new XMLHttpRequest();
//     rec.open('GET', "/saveConfig");
//     rec.send();
//     // window.location.href = "/saveConfig";
// }