let rooms = [];
let devices = [];

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

document.addEventListener("DOMContentLoaded", function () {
  getJSON("JSONrooms")
    .then((data) => {
      // Fill roomContainer with rooms

      let roomContainer = document.querySelector("#roomContainer");
      console.log(roomContainer);
      data.forEach((room) => {
        let newRoomElement = document.createElement("div");
        newRoomElement.innerHTML = `
                                    <div id="sha_temp_body">
                                            <div class="row top-buffer ">
                                                <div class="col-12">
                                                    <span class="sha_temp white_back">
                                                        <span>
                                                            <span class="temp-data">16<span class="small_01">.7</span> <sup>°C</sup></span>
                                                                <hr class="line_">
                                                            <span class="hum-data">48<span class="small_01">.3</span>
                                                            <span class="sup">%</span>
                                                            <span class="hidden_span mac_device">xxx</span>
                                                    </span>
                                                    </span>
                                                    </span>
                                            </div>
                                            <div class="height_auto"> </div>
                                        </div>
                                    </div>`;

        roomContainer.appendChild(newRoomElement);
        return true;
      });
    })
    .catch((error) => {
      console.log(error + "error getting Devices");
    });

  interaction();

  runEvery(10, getForecast);
});

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

//   var rotationSnap = 5;

//   var encoders = document.querySelectorAll('.encoder');
    
//       for (var i = 0; i < encoders.length; i++) {
        
//         var encoder = encoders[i];

//         Draggable.create(encoder, {
//             type: "rotation",
        
//             minRotation: 80,
//             maxRotation: 180,
//             onDrag: function(endValue) {
//               $('#count').html(Math.round(this.rotation));
        
//             },
//             snap: function(endValue) {
//                 $('#count').html(Math.round(this.rotation));
//               return Math.round(endValue / rotationSnap) * rotationSnap;
//             }
            
//           });



//       };

object = document.querySelector(".room_element .encoder");
object.addEventListener("mouseenter", function (e) {
    console.log("mousedown");
    $(".room_element .encoder").fadeTo(300, 1);

});
object.addEventListener("mouseleave", function (e) {
    console.log("mousedown");
    $(".room_element .encoder").fadeTo(300, 0);

});

$(".room_element .encoder .dial").knob({
    'readOnly': false,
    'height': 220,
    'width': 220,
     'min':12,
      'max':36,
    'step':0.1,
    'thickness': 0.15,
    'displayInput': false,
    'dynamicDraw': true,
    'fgColor': '#7ba8c9',
    'bgColor': 'none',
    'change': function(v) {
        val = v.toString().slice(0, 4);
           $(".room_element .encoder .heat_value").html(val);
       // console.log(v);
        //   $('#dial2 .dial').val(100-v).trigger('change');
    }
  });
 




}
