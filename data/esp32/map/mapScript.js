//global objects for map
const redIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-red.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
  });
const yellowIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-gold.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
  });

let firstWaypoint = L.marker([46.0681230644734, 11.150550088742115]);
let firstWaypointCircle = L.circle([46.066757288599504, 11.149665173034434], {
    color: '#2b5cab',
    fillColor: '#1189d9',
    fillOpacity: 0.5,
    radius: 10
});
let waypointsList = [firstWaypoint]; //For future implementation!

let actualWaypoint = L.marker([46.06820112646287, 11.149819112494852], {icon: redIcon});
let map = L.map('map');

let actualWaypointAdded = false;
let firstWaypointAdded = false;

function loadMap(){
  // let actualPoint = getActualPosition();
  let actualPoint = {time: 1708361330, actualCoordinates: [46.06820112646287, 11.149819112494852], fix: 1, fixType: 3, sats: 6, hdop: 4.7, temp: 23.7};
  if(actualPoint === undefined){
    map.setView([46.075040938511776, 11.121374432172466], 12);
  }else{
    if(actualPoint.fix == true && actualPoint.hdop < 5){
      map.setView(actualPoint.actualCoordinates, 16);
      actualWaypoint.setLatLng(actualPoint.actualCoordinates);
      actualWaypoint.addTo(map);
      actualWaypoint.bindPopup("<b>You are Here!</b>").openPopup();
      actualWaypointAdded = true;
    }else{
      map.setView([46.075040938511776, 11.121374432172466], 12);
    }
  }
  
  L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
      maxZoom: 19,
      attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
  }).addTo(map);
  setInterval(reflashCurrentPosition, 2000);
  setInterval(reflashWaypoints, 2000);
}

function getActualPosition(){
  let rec = new XMLHttpRequest();
  rec.open('GET', "/actualPosition");
  rec.send();
  rec.onload = () => {
    if(rec.responseType == ""){
      //console.log(rec.responseText);
      let jsonObj = JSON.parse(rec.responseText);
      return jsonObject;
    }else{
      console.error("error type");
      console.log("type:", rec.responseType);
      console.log("response:", rec.response);
      console.log("responseText:", rec.responseText);
      return undefined;
    }
  };
}

function getWaypointList(){
  let rec = new XMLHttpRequest();
  rec.open('GET', "/waypointList");
  rec.send();
  rec.onload = () => {
    if(rec.responseType == ""){
      //console.log(rec.responseText);
      let jsonObj = JSON.parse(rec.responseText);
      return jsonObject;
    }else{
      console.error("error type");
      console.log("type:", rec.responseType);
      console.log("response:", rec.response);
      console.log("responseText:", rec.responseText);
      return undefined;
    }
  };
}

function randomNumber(min, max){
  return Math.random() * (max - min + 1.0) + min;
}

function reflashCurrentPosition(){
  // let actualPoint = getActualPosition();
  let actualPoint = {time: 1708361330, actualCoordinates: [46.06820112646287, 11.149819112494852], fix: true, fixType: 3, sats: 6, hdop: 5.76, temp: 23.7};
  // actualPoint.hdop = randomNumber(4.5, 5.5);
  // actualPoint.actualCoordinates[0] = randomNumber(46.06820112646287, 46.0685693929914);
  // actualPoint.actualCoordinates[1] = randomNumber(11.149819112494852, 11.149845609819108);
  // console.log(actualPoint.actualCoordinates);
  if(actualPoint !== undefined){
    if(actualPoint.fix == true && actualPoint.hdop < 5){
      map.panTo(actualPoint.actualCoordinates);
      actualWaypoint.setLatLng(actualPoint.actualCoordinates);
      if(!actualWaypointAdded){
        actualWaypoint.addTo(map);
        actualWaypoint.bindPopup("<b>You are Here!</b>");
        actualWaypointAdded = true;
      }
    }
  }
}

function reflashWaypoints(){
  //For now only one waypoint is added
  // let waypoints = getWaypointList();
  let waypoints = [{time: 1708361330, coordinates: [46.066757288599504, 11.149665173034434], fix: true, fixType: 3, sats: 6, hdop: 4.7, temp: 23.7}];
  if(waypoints !== undefined){
    let actualPoint = waypoints[0];
    if(actualPoint.fix == true && actualPoint.hdop < 5){
      // map.panTo(actualPoint.actualCoordinates);
      firstWaypoint.setLatLng(actualPoint.coordinates);
      firstWaypointCircle.setLatLng(actualPoint.coordinates);
      if(!firstWaypointAdded){
        firstWaypoint.addTo(map);
        firstWaypoint.bindPopup("<b>First way point</b>");
        firstWaypointCircle.addTo(map);
        firstWaypointAdded = true;
      }
    }
  }
}