

class Room {
  constructor(id, name, temperature, humidity, minTemp, sensorMac) {
    this.id           = id;
    this.name         = name;
    this.temperature  = temperature;
    this.minTemp      = minTemp;
    this.humidity     = humidity;
    this.sensorMac    = sensorMac;
  }
}

class Termostat {
  constructor() {
   
    this.rooms    = [];
    this.devices  = [];

  }

  addRoom(id, name, temperature, humidity, minTemp, sensorMac) {
  
    this.rooms.push(new Room(id, name, temperature, humidity, minTemp, sensorMac));
    console.log(id);
    this.createRoom(id);
    
  
  }

  createEncoder(id){
   let  element = document.querySelector(`#room-${id} #encoder-${id}`);  
    console.log(element);

    // element.setAttribute("style", `
    // position: re;
    // top: 45px;
    // opacity:0.4;
    // left: 35px;
    // width: 230px;
    // height: 229px;
    // z-index: 40000;
    // `);
    // console.log(document.querySelector("#room-0"));
    element.addEventListener("mouseleave", function(){

      $(this).removeClass("visible");


    });
    element.addEventListener("mouseenter", function(){


//      console.log(`mouseenter $#room-${id} #encoder-${id}`);
    //   // console.log($("#room-0 #encoder-0"));
     // console.log(this);
    $(this).addClass("visible");
     let elementUpdate= $(this).find(".heat_value");
     console.log(elementUpdate);
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
                //val = v.toString().slice(0, 4);
                console.log(v.toString().slice(0, 4));
                $(elementUpdate).html(v.toString().slice(0, 4));
                // console.log();
              }
        });


    });

   



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
        
        newRoom.id = `room-${room.id}`;
        newRoom.innerHTML = `
        <div class="room_element sha_temp_body">
          
            <div  id="encoder-${room.id}" class="enc">
              <input class="dial noselect" value="0" min-value="12" max-value="36">
              <div class="backx"><div class="heat_value">80</div></div>
            </div>
            <div class="row top-buffer ">
                <div class="col-12">
                    <span class="sha_temp white_back">
                        <span>
                            <span class="temp-data">1<span class="small_01">.2</span> <sup>°C</sup></span>
                                <hr class="line_">
                            <span class="hum-data">0<span class="small_01">.0</span>
                            <span class="sup">%</span>
                            <span class="hidden_span mac_device">`+room.sensorMac+`</span>
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
  
  fetch("http://cleargrasstermostat.local/JSONrooms")
  .then(response => response.json())
  .then(data => {
  

    //termostat.rooms = data;
      
      for (let roomData of data) {
        console.log(roomData);
        termostat.addRoom(roomData.id, roomData.name, roomData.temperature, roomData.minTemp,roomData.sensorMac);
      }
    })
    .catch((error) => {
      console.log(error + "error creating Rooms");
    });

  // interaction();

  runEvery(10, getForecast);



  
 
    




});

