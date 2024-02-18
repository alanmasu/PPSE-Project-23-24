function loadMap(){
    var map = L.map('map').setView([46.077146695918714, 11.12079652532639], 16);
    L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 19,
        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    }).addTo(map);
    var marker = L.marker([46.077146695918714, 11.12079652532639]).addTo(map);
}