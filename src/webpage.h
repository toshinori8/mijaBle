#include <Arduino.h>



const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8" />
    <meta
      name="viewport"
      content="width=device-width, initial-scale=1, shrink-to-fit=no"
    />
    <!-- Bootstrap CSS -->
    <link
      rel="stylesheet"
      href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css"
      integrity="sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z"
      crossorigin="anonymous"
    />

    <title>Pokoje</title>
  </head>
  <body>
    <style>

.modal-blur {
  -webkit-backdrop-filter: blur(10px);
  backdrop-filter: blur(10px);
}

.modal-fade {
  -webkit-animation: fadein 0.5s;
  animation: fadein 0.5s;
}

@-webkit-keyframes fadein {
  from { opacity: 0; }
  to { opacity: 1; }
}

@keyframes fadein {
  from { opacity: 0; }
  to { opacity: 1; }
}


      .slide-left {
        /* Przesunięcie elementu w lewo o 100px */
        transform: translateX(-100px);
        /* Animacja przesunięcia trwająca 0.5s */
        transition: transform 0.5s;
      }

      .slide-right {
        /* Przesunięcie elementu w prawo o 100px */
        transform: translateX(100px);
        /* Animacja przesunięcia trwająca 0.5s */
        transition: transform 0.5s;
      }

      .room.slide-left {
        transform: translateX(-100px);
        transition: transform 0.5s;
      }

      .room.slide-right {
        transform: translateX(100px);
        transition: transform 0.5s;
      }
    </style>

    <!-- główny ekran-->
    <div class="container mt-5">
      <div class="row">
        <div class="col-12">
          <h1>Lista pokojów</h1>
          <p class="lead">
            Przeciągnij w prawo lub w lewo, aby potwierdzić usunięcie pokoju lub
            wejść w ustawienia
          </p>
          <button class="btn btn-primary float-right" id="addRoomBtn">
            Dodaj pokój
          </button>
        </div>
      </div>
      <div class="row mt-3">
        <div class="col-12">
          <div class="list-group" id="roomList">
            <!-- Tutaj będą wyświetlane pokoje pobieane z JSON-->
            
            

            
          </div>
        </div>
      </div>
    </div>

    <!-- Modal z formularzem dodawania pokoju -->
    <div class="modal modal-blur modal-fade" tabindex="-1" id="addRoomModal">
      <div class="modal-dialog">
        <div class="modal-content">
          <div class="modal-header">
            <h5 class="modal-title">Dodaj pokój</h5>
            <button
              type="button"
              class="close"
              data-dismiss="modal"
              aria-label="Close"
            >
              <span aria-hidden="true">&times;</span>
            </button>
          </div>
          <div class="modal-body">
            <form>
              <div class="form-group">
                <label for="roomNameInput">Nazwa pokoju</label>
                <input type="text" class="form-control" id="roomNameInput" />
              </div>
              <div class="form-group">
                <label for="roomMacInput">Adres MAC</label>
                <select class="form-control" id="roomMacInput"></select>
              </div>
            </form>
          </div>
          <div class="modal-footer">
            <button
              type="button"
              class="btn btn-secondary"
              data-dismiss="modal"
            >
              Anuluj
            </button>
            <button type="button" class="btn btn-primary" id="saveRoomBtn">
              Zapisz
            </button>
          </div>
        </div>
      </div>
    </div>




    <!-- Modal potwierdzenie usunięcia  -->
    <div
      class="confirm d-flex justify-content-between align-items-center bg-light p-2 modal-blur modal-fade"
      style="display: none !important"
    >
      <span>Czy na pewno chcesz usunąć ten pokój?</span>
      <div>
        <button type="button" class="btn btn-secondary mr-2">Anuluj</button>
        <button type="button" class="btn btn-danger">Usuń</button>
      </div>
    </div>










<!-- Modal Edycja pokoju -->
<div class="modal modal-blur modal-fade" id="editRoomModal" tabindex="-1" aria-labelledby="editModalLabel" aria-hidden="true">
    <div class="modal-dialog">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="editModalLabel">Edytuj pokój</h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close">
            <span aria-hidden="true">&times;</span>
          </button>
        </div>
        <div class="modal-body">
            <form>
                <div class="form-group">
                  <label for="editRoomNameInput">Nazwa pokoju</label>
                  <input type="text" class="form-control" id="editRoomNameInput" placeholder="Nazwa pokoju">
                </div>
                <div class="form-group">
                  <label for="editRoomMacInput">Adres MAC</label>
                  <!-- Lista z adresami MAC -->
                  <select class="form-control" id="editRoomMacInput">
                   
                    <!-- Wypełnione z JSON -->


                  </select>
                </div>
                <div class="form-group">
                  <!-- Pole do edycji adresu MAC -->
                  <input type="text" class="form-control" id="editRoomMacInputCustom" placeholder="Wprowadź adres MAC">
                </div>
                <!-- Przycisk zapisania zmian -->
                <!-- <button type="submit" class="btn btn-primary">Zapisz</button> -->
              </form>
          


        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-secondary" data-dismiss="modal">Anuluj</button>
          <button type="button" class="btn btn-primary">Zapisz zmiany</button>
        </div>
      </div>
    </div>
  </div>


   
    <script
      src="https://code.jquery.com/jquery-3.5.1.slim.min.js"
      integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj"
      crossorigin="anonymous"
    ></script>
    <script
      src="https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js"
      integrity="sha384-9/reFTGAW83EW2RDu2S0VKaIzap3H66lZH81PoYlFhbGU+6BZp6G7niu735Sk7lN"
      crossorigin="anonymous"
    ></script>
    <script
      src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"
      integrity="sha384-B4gt1jrGC7Jh4AgTPSdUtOBvfO8shuf57BaghqFfPlYxofvL8/KUEfYiJOMMV+rV"
      crossorigin="anonymous"
    ></script>

    <script>
      // Inicjalizacja SWIPE

      function addTouchSwipeAction(element) {
        // Obsługa zdarzenia touchend
        element.addEventListener("touchend", (event) => {
          // Pobranie położenia zakończenia dotyku
          const touchEndX = event.changedTouches[0].clientX;

          // Pobranie położenia elementu
          const elementRect = element.getBoundingClientRect();
          const elementLeft = elementRect.left;
          const elementRight = elementRect.right;

          // Sprawdzenie, czy użytkownik chciał kliknąć element, czy przesunąć go
          if (touchEndX < elementLeft - 5) {
            // Przesunięcie w lewo
            element.classList.add("slide-left");
          } else if (touchEndX > elementRight + 5) {
            // Przesunięcie w prawo 
            console.log("Przesunięcie w prawo" + touchEndX > elementRight + 5);
            element.classList.add("slide-right");
          } else {
            // Kliknięcie zwykłe
            console.log("Kliknięcie zwykłe");

            $('#editRoomModal').modal('show');

            console.log("Kliknięcie zwykłe : edycja!");
           
    //     
            // const roomMac = element.querySelector('.room-mac').textContent;
            // const roomName = element.querySelector('.room-name').textContent;
            // document.getElementById('editRoomNameInput').value = roomName;
            // document.getElementById('editRoomMacInput').value = roomMac;

            document.getElementById('editRoomMacInput').value = element.querySelector('.room-mac').textContent;

            document.getElementById('editRoomNameInput').value = element.querySelector('.room-name').textContent;  

          }
        });

        // Obsługa zdarzenia transitionend
        element.addEventListener("transitionend", () => {
          // Usunięcie klas po zakończeniu animacji przesunięcia
          element.classList.remove("slide-left", "slide-right");
        });
      }

      const roomElements = document.querySelectorAll(".room");
      ///console.log("Liczba pokoi na liście: " + roomElements.length);

      roomElements.forEach((room) => {
        // console.log(room.classList());
        addTouchSwipeAction(room);
      });

      // Po kliknięciu przycisku "Dodaj pokój" pokaż modal z formularzem dodawania pokoju
      $("#addRoomBtn").click(function () {
        $("#addRoomModal").modal("show");
      });

      // Po kliknięciu przycisku "Zapisz" w formularzu dodawania pokoju dodaj pokój do listy
      $("#saveRoomBtn").click(function () {
        // Pobierz dane z formularza
        let roomName = $("#roomNameInput").val();
        let roomMac = $("#roomMacInput").val();

        // Dodaj pokój do listy
        addRoom({ name: roomName, mac: roomMac });

        // Wyczyść formularz i zamknij modal
        $("#roomNameInput").val("");
        $("#roomMacInput").val("");
        $("#addRoomModal").modal("hide");
      });

      function addRoom(room) {
        // Dodanie elementu do listy pokoi
        const roomList = document.querySelector("#roomList");
        const newRoomElement = document.createElement("div");
        newRoomElement.innerHTML = `<a href="#" class="room list-group-item list-group-item-action flex-column align-items-start">
                                    <div class="d-flex w-100 justify-content-between">
                                        <h5 class="mb-1 room-name">${room.name}</h5>
                                        <small class="text-muted room-mac">${room.mac}</small>
                                    </div>
                                    </a>`;
        
        addTouchSwipeAction(newRoomElement);
        roomList.appendChild(newRoomElement);
      }


      // Przykładowy obiekt z danymi o pokojach
      const rooms = [
        {
          name: "Pokój 1",
          mac: "00:11:22:33:44:55",
        },
        {
          name: "Pokój 2",
          mac: "66:77:88:99:AA:BB",
        },
        {
          name: "Pokój 3",
          mac: "CC:DD:EE:FF:00:11",
        },
      ];

    

        rooms.forEach((room) => {

            console.log(room);

               addRoom(room);
        });




        const devices = [
        {
          name: "Czujnik waleria",
          mac: "00:11:22:33:44:55",
        },
        {
          name: "Czujnik kuchnia",
          mac: "66:77:88:99:AA:BB",
        },
        {
          name: "Czujnik łazienka",
          mac: "CC:DD:EE:FF:00:11",
        },
      ];



      // Pobierz element listy rozwijanej
      const select = document.getElementById("roomMacInput");

      // Wypełnij listę opcjami z obiektu rooms
      devices.forEach((device) => {
        const option = document.createElement("option");
        option.value = device.mac;
        option.text = device.name;
        select.appendChild(option);
      });
    </script>
  </body>
</html>





    )rawliteral";