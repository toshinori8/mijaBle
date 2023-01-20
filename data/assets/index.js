class Room {
  constructor(id, name, temperature, humidity, minTemp, mac) {
    this.id = id;
    this.name = name;
    this.temperature = temperature;
    this.humidity = humidity;
    this.minTemp = minTemp;
    this.mac = mac;
  }
}

class Termostat {
  constructor() {
    this.rooms = [];
    this.devices = [];
  }

  addRoom(id, name, temperature, humidity, minTemp, macAddress) {
    this.rooms.push(
      new Room(id, name, temperature, humidity, minTemp, macAddress)
    );
    this.createRoom(id);
  }

  updateRoomData(id, temperature, humidity, minTemp) {
    let room = this.rooms.find((room) => room.id == id);
    if (room) {
      room.temperature = temperature;
      room.humidity = humidity;
      room.minTemp = minTemp;
    }
  }

  updateRoomData_HTML(id) {
    // get room data from HTML
    let room = this.rooms.find((room) => room.id == id);
    let temp = document.querySelector("#room-" + id + " .temp-data").firstChild
      .data;
    let hum = document.querySelector("#room-" + id + " .hum-data").firstChild
      .data;
    let minTemp = document.querySelector("#room-" + id + " .heat_value")
      .firstChild.data;
    // update room data
    room.temperature = temp;
    room.humidity = hum;
    room.minTemp = minTemp;
    console.log(`update room ID: ${id} complete`);
    termostat.logRoomData(id);
  }

  logRoomData(id) {
    let room = this.rooms.find((room) => room.id == id);

    if (room) {
      console.log(
        "………………………………………………………………………………………………………………………………………………………………………………………………………"
      );
      console.log(
        "|room id : " +
          id +
          "| |name :" +
          room.name +
          "| |minTemp : " +
          room.minTemp +
          "|"
      );
      console.log(
        "|macAddress : " +
          room.mac +
          "| |temperature : " +
          room.temperature +
          "| |humidity : " +
          room.humidity +
          "|"
      );
    }
  }
  createGraph(id) {
    let ctx = document.getElementById(`chart-${id}`).getContext("2d");

    // let buttonYear= document.querySelector(`#chart-container-${id} .year-scale`);
    let buttonMonth = document.querySelector(
      `#chart-container-${id} .month-scale`
    );
    // let buttonWeek= document.queryquerySelector(`#chart-container-${id} .week-scale`);
    let buttonDay = document.querySelector(
      `#chart-container-${id} .day-scale`
    );

    buttonDay.addEventListener("click", function () {
      var chartDay = new Chart(ctx, {
        type: "line",
        data: sensorDataDay,
        options: {
          scales: {
            yAxes: [
              {
                ticks: {
                  beginAtZero: true,
                },
              },
            ],
          },
        },
      });
    });

    buttonMonth.addEventListener("click", function () {
      var chartMonth = new Chart(ctx, {
        type: "line",
        data: genMonthData(),
        options: {
          scales: {
            yAxes: [
              {
                ticks: {
                  beginAtZero: true,
                },
              },
            ],
          },
        },
      });
    });
  
  
  }


  createEncoder(id) {
    let element = document.querySelector(`#room-${id} #encoder-${id}`);
    element.addEventListener("mouseleave", function () {
      $(this).removeClass("visible");
    });
    element.addEventListener("mouseenter", function () {
      $(this).addClass("visible");

      // find room with id
      let room = termostat.rooms.find((room) => room.id == id);

      let elementUpdate = $(this).find(".heat_value");
      $(this)
        .find(".dial")
        .knob({
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
            // set minTemp in rooms array
            room.minTemp = v;

            termostat.updateRoomData_HTML(id);

            //termostat.updateRoomData(id,device.temp, device.hum, device.minTemp);
          },
        });
    });
  }
  alertModal(body) {
    $("#alert-modal-body").html(body);
    $("#alertModal").modal("show");
  }
  updateTemperature(id, newTemperature) {
    // find room with id
    let room = this.rooms.find((room) => room.id == id);
    room.temperature = newTemperature;

    let temp = newTemperature.toString().split(".");
    document.querySelector("#room-" + id + " .temp-data").firstChild.data =
      temp[0];
    if (temp[1] == undefined) {
      temp[1] = "0";
    }
    document.querySelector("#room-" + id + " .temp-data span").firstChild.data =
      "." + temp[1];
  }
  updateHumidity(id, newHumidity) {
    let room = this.rooms.find((room) => room.id == id);
    room.humidity = newHumidity;

    let hum = newHumidity.toString().split(".");
    // if(typeof newHumidity == "string"){
    //   let hum = newHumidity.split(".");
    // }else{
    //
    // }

    document.querySelector("#room-" + id + " .hum-data").firstChild.data =
      hum[0];
    if (hum[1] == undefined) {
      hum[1] = "0";
    }
    document.querySelector("#room-" + id + " .hum-data span").firstChild.data =
      "." + hum[1];
  }

  createRoom(id) {
    let room = this.rooms.find((room) => room.id == id);
    let container = document.querySelector("#roomContainer");
    let newRoom = document.createElement("div");

    // get room device data
    let device = this.devices.find((device) => device.mac == room.mac);
    if (device == undefined) {
    } else {
    }

    // if room.minTemp is set
    let minTemp = room.minTemp;
    if (minTemp == undefined) {
      minTemp = 16;
    }

    let temp = [1, 1];
    if (device.temp == undefined) {
      console.log("no temp for room ");
    } else {
      temp = device.temp.toString().split(".");
      if (temp[1] == undefined) {
        temp[1] = "0";
      }
      if (temp[0] == undefined) {
        temp[0] = 0;
      }
    }

    let humidity = [1, 1];
    if (device.hum == undefined) {
      console.log("no humidity for room ");
    } else {
      humidity = device.hum.toString().split(".");
      if (humidity[1] == undefined) {
        humidity[1] = "0";
      }
      if (humidity[0] == undefined) {
        humidity[0] = 0;
      }
    }

    newRoom.id = `room-${room.id}`;
    newRoom.innerHTML =
      `
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
                            <span class="hum-data">${humidity[0]}<span class="small_01">.${humidity[1]}</span>
                            <span class="sup">%</span>
                            <span class="hidden_span mac_device">` +
      room.mac +
      `</span>
                    </span>
                    </span>
                    </span>
            </div>
            <div class="menu_room">
                <h1 class="roomName">` +
      room.name +
      `</h1>

      <div class="chart-container" id="chart-container-${room.id}">
      <div class="chart-container-header">Wykresy ogrzewania</div>
      <canvas class="chart" id="chart-${room.id}"></canvas>
     
      <div class="buttons-container">
      <button class="year-scale">Rok</button>
      <button class="month-scale">Miesiąc</button>
      <button class="day-scale">Dzień</button>
    </div>

     
      </div>
    
            </div>
            <div class="height_auto">
            </div>
            </div>
        </div>`;

    container.appendChild(newRoom);

    termostat.updateRoomData(id, device.temp, device.hum, device.minTemp);

    this.createEncoder(room.id);
    this.createGraph(room.id);
  }
} // end class Termostat

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

server = "http://" + window.location.hostname + "";

// function run some code in the background every n minutes
function runEvery(minutes, callback) {
  callback();
  setInterval(callback, minutes * 60 * 1000);
}

function getForecast() {
  $.ajax({
    type: "GET",
    url: "https://api.openweathermap.org/data/2.5/weather?lat=49.880870&lon=19.563030&appid=f055d509de51700a688e61d5f8e3da76&units=metric",
    dataType: "json",
    success: function (json) {
      //  json.main.temp, json.main.humidity
      $(".forecast_app .temp_data").html(json.main.temp + "°C");
      $(".forecast_app .hum_data").html(json.main.humidity + "%");
    },
  });
}

function getDevicesActualData() {
  getJSON("http://cleargrasstermostat.local/JSONdevices").then((data) => {
    // update devices with new data
    for (let device of data) {
      termostat.devices.find((d) => d.mac == device.mac).temp = device.temp;
      termostat.devices.find((d) => d.mac == device.mac).hum = device.hum;
      termostat.devices.find((d) => d.mac == device.mac).name = device.name;
    }

    // update rooms with new data
    // for (let room of termostat.rooms) {
    //   let device = termostat.devices.find((device) => device.mac == room.mac);
    //   if (device == undefined) {
    //   } else {

    //     console.log(device);

    //   }

    // }
    for (let device of termostat.devices) {
      let room = termostat.rooms.find((room) => room.mac == device.mac);
      if (room == undefined) {
      } else {
        console.log(room);
        console.log(typeof device.hum);
        if (device.hum != undefined) {
          termostat.updateHumidity(room.id, device.hum);
        }
        if (device.temp != undefined) {
          termostat.updateTemperature(room.id, device.temp);
        }

        // termostat.updateHumidity(room.id, device.hum);
        // termostat.updateTemperature(room.id, device.temp);
      }
    }

    //termostat.updateRoomData(room.id, device.temp, device.hum, device.minTemp);
    // termostat.updateHumidity(room.id, device.hum.asFloat());
    // termostat.updateTemperature(room.id, device.temp.asFloat());

    //
  });
}

let termostat = new Termostat();
document.addEventListener("DOMContentLoaded", function () {
  // get devices from JSON file
  getJSON("http://cleargrasstermostat.local/JSONdevices").then((data) => {
    termostat.devices = data;

    fetch("http://cleargrasstermostat.local/JSONrooms")
      .then((response) => response.json())
      .then((data) => {
        // create rooms
        // if room has device in devices array, then create room
        for (let roomData of data) {
          find = termostat.devices.find(
            (device) => device.mac == roomData.mac.toString()
          );
          loader = document.querySelector(".loader");
          if (find != undefined) {
            if (loader) {
              loader.remove();
            }
            termostat.addRoom(
              roomData.id,
              roomData.name,
              roomData.temperature,
              roomData.humidity,
              roomData.minTemp,
              roomData.mac
            );
          } else {
            termostat.alertModal(
              'Są pokoje bez przypisanych urządzeń, </br>sprawdź  <a href="http://cleargrasstermostat.local/settings">ustawienia.</a>'
            );
          }
        }
      })
      .catch((error) => {
        console.log(error + "   error creating Rooms");
      });

    runEvery(0.1, getDevicesActualData); // 0.1 min
  });

  // interaction();

  runEvery(10, getForecast);
});
