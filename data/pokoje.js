
  let currentEditetdRoom = null;
// Przykładowy obiekt z danymi o pokojach
  let rooms = [];
  let devices = [];

  // pobranie danych JSON z serwera
  function getJSON(url) {
    return fetch(url).then((response) => response.json());

  }

  


      document.addEventListener("DOMContentLoaded", function() {
      

        // get initial data from server
        getJSON("/JSONdevices").then((data) => {
          
          devices = data;
          console.log(data);
          
          const select = document.querySelectorAll(".roomMacInput");
      devices.forEach((device) => {
        select.forEach((selectBox) => {
          let option = document.createElement("option");
          console.log(selectBox);
          option.value = device.mac;
        option.text = device.name;
          selectBox.appendChild(option);
        });
      });


          getJSON("/JSONpokoje").then((data) => {
            rooms = data;
            rooms.forEach((room) => {
              addRoom(room);
            });
          }).catch((error) => {
            console.log(error + "error getting Rooms");
          });

          

        }).catch((error) => {
          console.log(error+ "error getting Devices");
        });
        



      ////   THE REST OF THE CODE IS HERE

        



  function alertModal(body) {
    $('#alert-modal-body').html(body);
    $("#alertModal").modal("show");
  }


        document.getElementById("edit_RoomMacInput").addEventListener("change", function () {
        document.getElementById("edit_RoomMacInputCustom").value = this.value;

      }); 


      function deviceNameByMac(mac) {
        const device = devices.find((device) => device.mac === mac);
        if (device) {
          return device.name;
        } else {
          return "Brak nazwy";
        }  
      };
      
      
      
      function addTouchSwipeAction(element) {
       
        element.addEventListener("click", (event) => {
        
          document.getElementById('editRoomNameInput').value = element.querySelector('.room-name').textContent;           
          document.getElementById('edit_RoomMacInputCustom').value = element.querySelector('.room-mac').textContent;
            
          currentEditetdRoom = element;

          $("#editRoomModal").modal("show");
        });

      };
      

      const roomElements = document.querySelectorAll(".room");
      roomElements.forEach((room) => {
        addTouchSwipeAction(room);
      });


      $("#saveRoomData").click(function () {
        let roomName = $("#editRoomNameInput").val();
        let roomMac = $("#edit_RoomMacInput").val();

        
        currentEditetdRoom.querySelector(".room-name").textContent = roomName;
        currentEditetdRoom.querySelector(".room-mac").textContent = roomMac;
        currentEditetdRoom.querySelector(".room-name_small").textContent = deviceNameByMac(roomMac);
        $("#editRoomModal").modal("hide");


      });



      // Po kliknięciu przycisku "Dodaj pokój" pokaż modal z formularzem dodawania pokoju
      $("#addRoomBtn").click(function () {
        $("#addRoomModal").modal("show");
      });


     
      // Po kliknięciu przycisku "Zapisz" w formularzu dodawania pokoju dodaj pokój do listy
      $("#saveRoomBtn").click(function () {
        // Pobierz dane z formularza
        let roomName = $("#roomNameInput").val();
        let roomMac = $("#add_RoomMacInput").val();

        // Dodaj pokój do listy
        addRoom({ name: roomName, mac: roomMac });

        // Wyczyść formularz i zamknij modal
        $("#roomNameInput").val("");
        $("#add_RoomMacInput").val("");
        $("#addRoomModal").modal("hide");
      });

      


      


      function addRoom(room) {
        // Dodanie elementu do listy pokoi
        const roomList = document.querySelector("#roomList");
        const newRoomElement = document.createElement("div");
        
        newRoomElement.innerHTML = `<a href="#" class="room list-group-item list-group-item-action flex-column align-items-start">
                                    <div class="d-flex w-100 justify-content-between">
                                        <h5 class="mb-1 room-name">${room.name}</h5>
                                        <small class="text-muted room-name_small">${deviceNameByMac(room.mac)}</small>
                                        <small class="text-muted room-mac">${room.mac}</small>
                                    </div>
                                    </a>`;
        
        addTouchSwipeAction(newRoomElement);
        roomList.appendChild(newRoomElement);
      }






      document.getElementById("exitBtn").addEventListener("click", function () {
         
          // serialize the form data
          let rooms =document.querySelectorAll(".room");
          let roomsData = [];
            rooms.forEach((room) => {

            roomsData.push({
                name: room.querySelector(".room-name").textContent,
                mac: room.querySelector(".room-mac").textContent,
              });



            });
              let data = JSON.stringify(roomsData);
              
              // POST data to server /saveRooms 
              var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
              var theUrl = "/saveRooms";
              xmlhttp.open("POST", theUrl);
              xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
              xmlhttp.send( data );
                            
              xmlhttp.onreadystatechange = function() {
                // get json response from server
                  let res = JSON.parse(this.response);
                
                
                if (this.status == 200 && res.response == "dataSaved") {
                  // window.location.href = "/";
                   console.log("dataSaved");
                } else if(res.response != "dataSaved") {
                  alertModal("Nie udało się zapisać danych");
                }
                  
                
              };
              xmlhttp.onerror = function(error) {
                
                alertModal( "Nie udało się zapisać danych");
                console.log(error);
              };

      });
        
        
      
      });


      
     
      
     