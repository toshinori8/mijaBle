

class Room {
  constructor(id, name, temperature, humidity, minTemp, mac) {
    this.id           = id;
    this.name         = name;
    this.temperature  = temperature;
    this.humidity     = humidity;
    this.minTemp      = minTemp;
    this.mac          = mac;
  }
}

class Termostat {
  constructor() {
   
    this.rooms    = [];
    this.devices  = [];

  }

  addRoom(id, name, temperature, humidity, minTemp, macAddress) {
  
    this.rooms.push(new Room(id, name, temperature, humidity, minTemp, macAddress));
    this.createRoom(id);
  
  }

  createEncoder(id){
   let  element = document.querySelector(`#room-${id} #encoder-${id}`);  
      element.addEventListener("mouseleave", function(){
        $(this).removeClass("visible");

    });
    element.addEventListener("mouseenter", function(){

    $(this).addClass("visible");
     let elementUpdate= $(this).find(".heat_value");
        $(this).find(".dial").knob({
              readOnly: false,
              height: 220,
              width: 220,
              min: 12,
              max: 36,
              step: 0.1,
              thickness: 0.15,
              displayInput: false,
              dynamicDraw: true,
              fgColor: "#7ba8c9",
              bgColor: "none",
              change: function (v) {
                $(elementUpdate).html(v.toString().slice(0, 4));
              }
        });

    });

   



  }
  alertModal(body) {

    $("#alert-modal-body").html(body);
    $("#alertModal").modal("show");
  
  }
  updateTemp(id, newTemperature) {    

    // find room with id
    let room = this.rooms.find((room) => room.id == id);
    room.temperature = newTemperature;

    let temp = newTemperature.toString().split(".");
    document.querySelector("#room-"+id+" .temp-data").firstChild.data=temp[0];
    if(temp[1] == undefined){temp[1]="0"};
    document.querySelector("#room-"+id+" .temp-data span").firstChild.data="."+temp[1];  
  
  }
  updateHum(id, newHumidity) {
  
    let room = this.rooms.find((room) => room.id == id);
    room.humidity = newHumidity;

    let hum = newHumidity.toString().split(".");
    document.querySelector("#room-"+id+" .hum-data").firstChild.data=hum[0];
    if(hum[1] == undefined){hum[1]="0"};
    document.querySelector("#room-"+id+" .hum-data span").firstChild.data="."+hum[1];  
  
  }

 
  
  createRoom(id) {
       
        let room = this.rooms.find((room) => room.id == id);
        let container = document.querySelector("#roomContainer");
        let newRoom = document.createElement("div");
        
        
        // get room device data 
        let device = this.devices.find((device) => device.mac == room.mac);
        if(device == undefined){
          console.log('no device for room ' + room.id + ' with mac ' + room.mac + ' found');
        }else{
        console.log('device for room ' + room.id + ' with mac ' + room.mac + ' found')};

       console.log(device);


        // if room.minTemp is set
        let minTemp = room.minTemp; if(minTemp==undefined){minTemp=16};
        
        let temp = [1,1];
        if(device.temp==undefined){
         console.log('no temp for room ');
        }else{
          temp =device.temp.toString().split(".");
          if(temp[1] == undefined){temp[1]="0"};
          if(temp[0] == undefined){temp[0]=0};
        };

        
        // if (device.temp == undefined){
        //   device.temp=16;
        // }else{
        // };
        // // 
        
        newRoom.id = `room-${room.id}`;
        newRoom.innerHTML = `
        <div class="room_element sha_temp_body">
            <div  id="encoder-${room.id}" class="enc">
              <input class="dial noselect" value="0" min-value="12" max-value="36">
              <div class="backx"><div class="heat_value">${minTemp}</div></div>
            </div>
            <div class="row top-buffer ">
                <div class="col-12">
                    <span class="sha_temp white_back">
                        <span>
                            <span class="temp-data">${temp[0]}<span class="small_01">.${temp[1]}</span> <sup>°C</sup></span>
                                <hr class="line_">
                            <span class="hum-data">0<span class="small_01">.0</span>
                            <span class="sup">%</span>
                            <span class="hidden_span mac_device">`+room.mac+`</span>
                    </span>
                    </span>
                    </span>
            </div>
            <div class="menu_room">
                <h1 class="roomName">`+room.name+`</h1>
            </div>
            <div class="height_auto">
            </div>
            </div>
        </div>`;

        container.appendChild(newRoom);
        this.createEncoder(room.id);
  }
  

}// end class Termostat


function interaction() {
  $("#sha_button_list li").click(function () {
    $(this).toggleClass("on off");
  });

  $(".sha_switch").click(function () {
    $(this).toggleClass("on off");
  });

  $(".swithTheme").click(function () {
    // $("body").toggleClass("light dark");
  });
}

// pobranie danych JSON z serwera
function getJSON(url) {
  return fetch(url).then((response) => response.json());
}

// server = "http://192.168.8.94";
server = "http://" + window.location.hostname + "";

// function run some code in the background every n minutes
function runEvery(minutes, callback) {
  callback();
  setInterval(callback, minutes * 60 * 1000);
}

function getForecast() {
  $.ajax({
    type: "GET",
    url:
      "https://api.openweathermap.org/data/2.5/weather?lat=49.880870&lon=19.563030&appid=f055d509de51700a688e61d5f8e3da76&units=metric",
    dataType: "json",
    success: function (json) {
      //  json.main.temp, json.main.humidity
      $(".forecast_app .temp_data").html(json.main.temp + "°C");
      $(".forecast_app .hum_data").html(json.main.humidity + "%");
    },
  });
}

let termostat = new Termostat();
document.addEventListener("DOMContentLoaded", function () {
  
  // get devices from JSON file
  getJSON("http://cleargrasstermostat.local/JSONdevices").then((data) => {


   
    termostat.devices = data;

    fetch("http://cleargrasstermostat.local/JSONrooms")
  .then(response => response.json())
  .then(data => {

    
   

      // create rooms
      // if room has device in devices array, then create room
      for (let roomData of data) {
        //console.log(termostat.devices);
        console.log(roomData);
        if(termostat.devices.find((device) => device.mac == roomData.mac.toString())){
          console.log('device for room ' + roomData.id + ' with mac ' + roomData.mac + ' found');
          document.querySelector(".loader").remove();
          
          termostat.addRoom(roomData.id, roomData.name, roomData.temperature, roomData.humidity, roomData.minTemp, roomData.mac);
        }
        else{
          termostat.alertModal('Są pokoje bez przypisanych urządzeń. Sprawdź  <a href="http://cleargrasstermostat.local/settings">ustawienia</a>');
          console.log('no device for room ' + roomData.id + ' with mac ' + roomData.mac + ' found');
        } 


     
        
      }
     
    })
    .catch((error) => {
      console.log(error + "   error creating Rooms");
    });


  });
  
  // interaction();

  runEvery(10, getForecast);



  
 
    




});

