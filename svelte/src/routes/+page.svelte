<script>
  import {onMount } from "svelte";
  import Room from "../lib/components/roomWidget.svelte";
  // import Loader from "../lib/components/loader.svelte";
  let errorMessage = "";
  let loadingDataState = false;

  let jsonRoomsData = {};
  let jsonDevicesData = {};
  let roomsWithoutDevices = 0;

  import fetchStore from "../lib/stores/fetchData.js";
  import { fade } from "svelte/transition";
    import WeatherWidget from "$lib/components/weatherWidget.svelte";

  let [rooms, devices, loading, error] = fetchStore();
  

  export async function updateRoom(id) {
    let room = jsonRoomsData.find((room) => room.id == id);

    try {
      const response = await fetch(
        "http://cleargrasstermostat.local/data/updateRoom",
        {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(room),
        }
      );

      if (!response.ok) {
        throw new Error(response.statusText);
      }
      // console.log(response);
    } catch (e) {
      // console.log(e);
    }
    return id;
  }

  function updateData() {
    //// updates data on page & roomsData
    document.getElementById("loading_dot").classList.remove("hidden");

    // fetchData().then(() => {
    jsonRoomsData.forEach((room) => {
      room.temp = random(20, 30);
    });
    // roomsStore.updateRoomStore(jsonRoomsData);

    document.getElementById("loading_dot").classList.add("hidden");
    // });
  }

  function random(min, max) {
    return Math.floor(Math.random() * (max - min + 1) + min);
  }

  let connectWebsocket = () => {
    loadingDataState = true;

    const ws = new WebSocket("ws://cleargrasstermostat.local/data/ws");
    ws.addEventListener("message", (response) => {
      let type = response.data.split("*")[0];
      let data = response.data.split("*")[1];

      if (data != null && type == "devices") {

        let incomingData = JSON.parse(data);
        console.log("incoming data DEVICES");
        incomingData.forEach((device) => {
          roomsWithoutDevices = jsonRoomsData.filter(
            (room) => room.mac != device.mac
          ).length;

          jsonRoomsData.find((room) => room.mac == device.mac).temp =
            device.temp;
          jsonRoomsData.find((room) => room.mac == device.mac).hum = device.hum;
          jsonRoomsData.find((room) => room.mac == device.mac).bat = device.bat;
          jsonRoomsData.find((room) => room.mac == device.mac).lastUpdate =
            device.lastUpdate;
          loadingDataState = false;
         });

         $rooms.set(jsonRoomsData);

        // updateRoomStore(jsonRoomsData);

      }
      if (data != null && type == "rooms") {
        console.log("incoming data ROOMS");
         jsonRoomsData = JSON.parse(data);
        // updateRoomStore(jsonRoomsData);

      }
    });
  };

  onMount(async () => {
    await connectWebsocket();

    rooms.subscribe((value) => {
      jsonRoomsData = value;
    });
    devices.subscribe((value) => {
      jsonDevicesData = value;
    });
  });
</script>

<svelte:head>
  <title>CGG1 admin</title>
  <meta name="description" content="CGG1 admin panel" />
</svelte:head>

<p>{errorMessage}</p>

<div class="  pt-20">
  <div
    class="mx-auto max-w-11/12 justify-center px-6 md:flex md:space-x-6 xl:px-0"
  >
    <div class="rounded-lg md:w-2/3">
      {#if roomsWithoutDevices > 0}
        <div class="errorMessage">{errorMessage}</div>
        <div id="alert-modal-body" class="modal-body">
          {roomsWithoutDevices} pokoi bez przyporządkowanych urzadzeń, sprawdź
          <a href="/setup/rooms">ustawienia</a>
        </div>
      {/if}

      <!-- {#if loadingDataState === true}
        <Loader />
      {/if} -->


      {#if !jsonRoomsData[0] || !jsonDevicesData[0]}
        <!-- <div transition:fade><Loader /></div> -->
      {:else}
        {#each jsonDevicesData as device}
          {#each jsonRoomsData as room}
            {#if device.mac === room.mac}
            
            <!-- <div  transition:fade={{ delay: 500, duration: 500 }} style="display:flex"> -->
              
              <!-- <p>{room.name}  {room.id} :device: {room.mac}</p> -->

            <!-- </div> -->
            
              <Room roomID={room.id} {updateRoom} /> 
            {/if}
          {/each}
        {/each}
      {/if}

      <WeatherWidget></WeatherWidget>
   
    </div>
  </div>
</div>
