<script>
	import Battery from './battery.svelte';
  // @ts-nocheck
  // let jq = window.$;
  // import Counter from './Counter.svelte';
  import { onMount } from "svelte";
  import { setContext } from "svelte";
  import Room from "./room.svelte";
  import { writable } from "svelte/store";
  import Loader from "./loader.svelte";
  import Layout from "./+layout.svelte";

  let retryInterval = 3000;
  let errorMessage = "";
  let loadingDataState = false;

  /**
     * @type {any[]}
     */
  let jsonRoomsData = [];
  /**
     * @type {any[]}
     */
  let jsonDevicesData = [];

  let roomsWithoutDevices = 0;
  let retry = true;

  let name = "world";
  let nameStore = writable(name);

  $: nameStore.set(name);

  setContext("initial", name);
  setContext("name", nameStore);

  // let rooms = jsonRoomsData;
  let rooms = "some data";
  let roomsStore = writable(rooms);
  $: roomsStore.set(rooms);
  setContext("rooms", roomsStore);


  // @ts-ignore
  function updateRoomStore(dataIn) {
    console.log("updating room store");

    roomsStore.set(dataIn);
  }

  /**
     * @param {any} id
     */
   export async function updateRoom(id) {
    let room = jsonRoomsData.find(
      (/** @type {{ id: any; }} */ room) => room.id == id
    );

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
  }

  function updateData() {
    //// updates data on page & roomsData
    // @ts-ignore
    document.getElementById("loading_dot").classList.remove("hidden");

    fetchData().then(() => {
      // updateRoomStore(jsonRoomsData);
      // @ts-ignore
      document.getElementById("loading_dot").classList.add("hidden");
    });
  }

  async function changeData() {
 
    //// updates data on page & roomsData
    // @ts-ignore
    jsonRoomsData.forEach((room) => {
      
        room.bat = 30;
      
     
    });

   
    updateRoomStore(jsonRoomsData);

  }

  async function fetchData() {
  
    while (retry) {
      try {
        loadingDataState = true;
        const response = await fetch(
          "http://cleargrasstermostat.local/data/JSONrooms"
        );

        if (!response.ok) {
          throw new Error(response.statusText);
        }
        jsonRoomsData = await response.json();

        const responseDevices = await fetch(
          "http://cleargrasstermostat.local/data/JSONdevices"
        );
        if (!responseDevices.ok) {
          throw new Error(responseDevices.statusText);
        }
        jsonDevicesData = await responseDevices.json();
        // jsonDevices.set(jsonDevicesData);

        loadingDataState = false;
        jsonRoomsData.forEach((room) => {
          jsonDevicesData.forEach((device) => {
            if (device.mac == room.mac) {
              if (device.temp != null) {
                room.temp = device.temp;
               
              } else {
                room.temp = 0;
              }
              
              if (device.hum != null) {
                room.humidity = device.hum;
              } else {
                room.humidity = 0;
              }
              if (device.bat != null) {
                room.bat = device.bat;
              } else {
                room.bat = 0;
              }
            } else {
              roomsWithoutDevices++;
            }
          });
        });
        errorMessage = "Dane pobrane";
        updateRoomStore(jsonRoomsData);
        retry = false;
      } catch (error) {
        errorMessage =
          "An error occurred while fetching rooms data. Retrying in " +
          retryInterval / 100 +
          " seconds.";
        setTimeout(() => {
          retry = true;
        }, retryInterval);
      }
    }
  }

  const connectWebsocket = () => {
    console.log("connecting to websocket");
    const ws = new WebSocket("ws://cleargrasstermostat.local/data/ws");
    ws.addEventListener("message", (response) => {
      let type = response.data.split("*")[0];
      let data = response.data.split("*")[1];


      if (data != null) {
        let incomingData = JSON.parse(data);
        console.log(incomingData);
        incomingData.forEach((/** @type {{ mac: any; temp: number; humidity: number; bat: number; }} */ room) => {
          jsonDevicesData.forEach((device) => {
            if (device.mac == room.mac) {
              if (device.temp != null) {
                room.temp = device.temp;
              } else {
                room.temp = 0;
              }
              if (device.hum != null) {
                room.humidity = device.hum;
              } else {
                room.humidity = 0;
              }
              // if(device.bat != null){
              //   room.bat = device.bat;
              // } else {
              //   room.bat = 0;
              // }
              room.bat=30;
            } else {
              roomsWithoutDevices++;
            }
            updateRoomStore(jsonRoomsData);
          });
        });
        
        updateRoomStore(jsonRoomsData);
      
      }

      if (type == "rooms") {
      }
    });
  };

  onMount(async () => {
    await fetchData();
    await connectWebsocket();
  });
</script>

<svelte:head>
  <title>CGG1 admin</title>
  <meta name="description" content="CGG1 admin panel" />
</svelte:head>

<p>{errorMessage}</p>

<!-- <button on:click={updateData}>pobierz dane</button> -->
<button on:click={(e) => changeData(e)}>zmień dane2</button>
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
