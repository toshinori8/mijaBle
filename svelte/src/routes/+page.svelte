<script>
  import { onMount } from "svelte";
  import { setContext } from "svelte";
  import Room from "./room.svelte";
  import { writable } from "svelte/store";
  import Loader from "./loader.svelte";

  let errorMessage = "";
  let loadingDataState = false;

  let jsonRoomsData = []; 
  let jsonDevicesData = [];

  let roomsWithoutDevices = 0;
  let retry = true;
  


  let rooms = "rooms data";
  let roomsStore = writable(rooms);
  $: roomsStore.set(rooms);
  setContext("rooms", roomsStore);


  function updateRoomStore(dataIn) {
    roomsStore.set(dataIn);
    dataIn = dataIn;
  }

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
      updateRoomStore(jsonRoomsData);
      
      document.getElementById("loading_dot").classList.add("hidden");
    // });
  }

  function random(min, max) {
    return Math.floor(Math.random() * (max - min + 1) + min);
  }


  let connectWebsocket = () => {

    loadingDataState = true;

    console.log("connecting to websocket");
    const ws = new WebSocket("ws://cleargrasstermostat.local/data/ws");
    ws.addEventListener("message", (response) => {
      let type = response.data.split("*")[0];
      let data = response.data.split("*")[1];

      if (data != null && type == "devices") {
        let incomingData = JSON.parse(data);

        incomingData.forEach((device) => {

          roomsWithoutDevices = jsonRoomsData.filter((room) => room.mac != device.mac).length;
          
          jsonRoomsData.find((room) => room.mac == device.mac).temp       = device.temp;
          jsonRoomsData.find((room) => room.mac == device.mac).hum        = device.hum;
          jsonRoomsData.find((room) => room.mac == device.mac).bat        = device.bat;
          jsonRoomsData.find((room) => room.mac == device.mac).lastUpdate = device.lastUpdate;

          loadingDataState = false;

        });
        updateRoomStore(jsonRoomsData);
      
      }
      if (data != null && type == "rooms") {
        jsonRoomsData = JSON.parse(data);
      }
    });
  };

  let url = ``;

  onMount(() => url = window.location.href);


  onMount(async () => {



    await connectWebsocket();
  });
</script>

<svelte:head>
  <title>CGG1 admin</title>
  <meta name="description" content="CGG1 admin panel" />
</svelte:head>

<p>{errorMessage}</p>

<!-- <button on:click={(e) => updateData()}>zmień dane2</button> -->
<section>
  {#if roomsWithoutDevices > 0}
    <div class="errorMessage">{errorMessage}</div>
    <div id="alert-modal-body" class="modal-body">
      {roomsWithoutDevices} pokojów bez przyporządkowanych urzadzeń, sprawdź
      <a href="/settings">ustawienia</a>
    </div>
  {/if}

  {#if loadingDataState === true}
    <Loader />
  {/if}

  {#await jsonRoomsData then devices}
    {#each devices as device}
      {#each jsonRoomsData as room}
        {#if device.mac === room.mac}
          <Room roomID={room.id} {updateRoom} />
        {/if}
      {/each}
    {/each}
  {/await}
</section>

<style>
</style>
