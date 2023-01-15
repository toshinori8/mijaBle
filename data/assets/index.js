

class Room {
  constructor(id, name, temperature, minTemp, sensorMac) {
    this.id           = id;
    this.name         = name;
    this.temperature  = temperature;
    this.minTemp      = minTemp;
    this.sensorMac    = sensorMac;
  }
}

class Termostat {
  constructor() {
    this.rooms    = [];
    this.devices  = [];
  }

  addRoom(id, name, temperature, minTemp, sensorMac) {
    this.rooms.push(new Room(id, name, temperature, minTemp, sensorMac));


    $("#roomContainer").append(`<div class="room_element room_element_`+id+` sha_temp_body">
    
    <div class="switch">
          <span class="sha_switch pull-right off swithTheme x2 enable_heat"></span>
        </div>
        <div class="encoder">
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
                        <span class="hidden_span mac_device">`+sensorMac+`</span>
                </span>
                </span>
                </span>
        </div>
        <div class="menu_room">
            <h1 class="roomName">`+name+`</h1>
        </div>
        <div class="height_auto">
        </div>
        </div>

    </div>`);


    
  }

  createEncoders(){
    console.log(this.rooms);
    this.rooms.forEach((room) => {
        // show encoder on mouseenter
        // document.querySelector(".room_element_"+room.id +" .encoder").addEventListener("mouseenter", function(){
        //   console.log("mouseenter");
        //   console.log(this);
        //   $(this).fadeIn(200);

        // });


    });

    // // create encoders for every room
    // let encoders = document.querySelectorAll(".dial");
    // encoders.forEach((encoder) => {
    //   $(encoder).knob({
    //     change: function (value) {
    //       //console.log("change : " + value);
    //     },
    //     release: function (value) {
    //       //console.log(this.$.attr('value'));
    //       //console.log("release : " + value);
    //     },
    //     cancel: function () {
    //       //console.log("cancel : ", this);
    //     },
    //   });
    // });


  }

  updateTemp(id, newTemperature) {    
    let temp = newTemperature.toString().split(".");
    document.querySelector(".room_element_"+id+" .temp-data").firstChild.data=temp[0];
    document.querySelector(".room_element_"+id+" .temp-data span").firstChild.data="."+temp[1];  
  }
  updateHum(id, newHumidity) {
    let hum = newHumidity.toString().split(".");
    document.querySelector(".room_element_"+id+" .hum-data").firstChild.data=hum[0];
    document.querySelector(".room_element_"+id+" .hum-data span").firstChild.data="."+hum[1];  
  }

  
  
  createRooms() {

    let container = document.querySelector("#roomContainer");
    
    if (this.rooms.length > 0) {
      
      // create ID for every room
      let id = 0;

      this.rooms.forEach((room) => {
        
        let newRoom = document.createElement("div");
        container.appendChild(newRoom);

      });
    } else {
      let message = document.createElement("div");
      message.innerHTML = '</br></br><div class="alert alert-secondary"><strong>Brak pomieszczeń</strong></br>Być może wystąpił problem z połączeniem.</div>';
      container.appendChild(message);
    }
   // this.createEncoders()
    return true;
  }

}

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
        termostat.addRoom(roomData.id, roomData.name, roomData.temperature, roomData.minTemp, roomData.sensorMac);
      }
    //termostat.createRooms();
    //termostat.updateTemp(2,1.13);
    //termostat.updateHum(2,45.4);
    })
    .catch((error) => {
      console.log(error + "error getting Rooms");
    });

  // interaction();

  runEvery(10, getForecast);



  
  // $(".room_element_0 .encoder .dial").knob({
  //   readOnly: false,
  //   height: 220,
  //   width: 220,
  //   min: 12,
  //   max: 36,
  //   step: 0.1,
  //   thickness: 0.15,
  //   displayInput: false,
  //   dynamicDraw: true,
  //   fgColor: "#7ba8c9",
  //   bgColor: "none",
  //   change: function (v) {
  //     val = v.toString().slice(0, 4);
  //     $(".room_element_1 .encoder .heat_value").html(val);
  //   }
  // });
    




});

