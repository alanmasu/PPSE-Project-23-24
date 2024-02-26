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


function centerMap(actualPoint){
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
}

function loadMap(){
  L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
      maxZoom: 19,
      attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
  }).addTo(map);
  getActualPosition(true);

  setInterval(getActualPosition, 1000);
  setInterval(getWaypointList, 1000);
}

function getActualPosition(center = false){
  let rec = new XMLHttpRequest();
  rec.open('GET', "/actualPosition");
  rec.send();
  rec.onload = () => {
    if(rec.responseType == ""){
      let jsonObj = JSON.parse(rec.responseText);
      if(center){
        console.log("Centering map");
        centerMap(jsonObj);
      }else{
        reflashCurrentPosition(jsonObj);
      }
    }else{
      console.error("error type");
      console.log("type:", rec.responseType);
      console.log("response:", rec.response);
      console.log("responseText:", rec.responseText);
    }
  };
}

function getWaypointList(){
  let rec = new XMLHttpRequest();
  rec.open('GET', "/waypointList");
  rec.send();
  rec.onload = () => {
    if(rec.responseType == ""){
      let jsonObj = JSON.parse(rec.responseText);
      reflashWaypoints(jsonObj);
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

function reflashCurrentPosition(actualPoint){
  // actualPoint = {time: 1708361330, actualCoordinates: [46.06820112646287, 11.149819112494852], fix: true, fixType: 3, sats: 6, hdop: 4.7, temp: 23.7};
  if(actualPoint !== undefined){
    // console.log("actualPoint defined");
    if(actualPoint.fix == true && actualPoint.hdop < 5){
      // console.log("actualPoint has fix");
      map.panTo(actualPoint.actualCoordinates);
      actualWaypoint.setLatLng(actualPoint.actualCoordinates);
      actualWaypoint.bindPopup("\
        <b>You are Here!</b>\
        <br>\
        Lat: " + actualPoint.actualCoordinates[0]  + "<br>\
        Long: " + actualPoint.actualCoordinates[1] + "<br>\
        Temp: " + actualPoint.temp + "<br>\
        HDOP: " + actualPoint.hdop + "<br>\
        Sats: " + actualPoint.sats
      );
      if(!actualWaypointAdded){
        actualWaypoint.addTo(map);
        actualWaypointAdded = true;
      }
    }
  }
}

function reflashWaypoints(waypoints){
  // waypoints = [{time: 1708361330, actualCoordinates: [46.0688739974261, 11.149632306379223], fix: true, fixType: 3, sats: 6, hdop: 4.7, temp: 23.7, waypointAdded:true }];
  if(waypoints !== undefined && length in waypoints){
    if(waypoints.length > 0){
      let actualPoint = waypoints[0];
      if(actualPoint.fix == true && actualPoint.hdop < 5 && actualPoint.waypointAdded){
        firstWaypoint.setLatLng(actualPoint.actualCoordinates);
        firstWaypointCircle.setLatLng(actualPoint.actualCoordinates);
        firstWaypoint.bindPopup("\
          <b>First Waypoint!</b>\
          <br>\
          Lat: " + actualPoint.actualCoordinates[0]  + "<br>\
          Long: " + actualPoint.actualCoordinates[1] + "<br>\
          Temp: " + actualPoint.temp + "<br>\
          HDOP: " + actualPoint.hdop + "<br>\
          Sats: " + actualPoint.sats
        );
        if(!firstWaypointAdded){
          firstWaypoint.addTo(map);
          firstWaypointCircle.addTo(map);
          firstWaypointAdded = true;
        }
      }else if(actualPoint.waypointAdded == false){
        if(firstWaypointAdded){
          firstWaypoint.removeFrom(map);
          firstWaypointCircle.removeFrom(map);
          firstWaypointAdded = false;
        }
      }
    }
  }
}