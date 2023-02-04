<script>
  import { onMount } from "svelte";
  import { setContext } from "svelte";
  import Room from "../lib/components/roomWidget.svelte";
  import { writable } from "svelte/store";
  import Loader from "../lib/components/loader.svelte";
  import WeatherWidget from "../lib/components/weatherWidget.svelte";
  import MorphingModal from "$lib/svelte-morphing-modal";

  let errorMessage = "";
  let loadingDataState = false;
  let url = ``;

  let jsonRoomsData = [];
  let jsonDevicesData = [];

  let roomsWithoutDevices = 0;

  let rooms = "rooms data";
  let roomsStore = writable(rooms);
  $: roomsStore.set(rooms);
  setContext("rooms", roomsStore);

  let open = false;

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
        updateRoomStore(jsonRoomsData);
      }
      if (data != null && type == "rooms") {
        jsonRoomsData = JSON.parse(data);
      }
    });
  };

  onMount(() => (url = window.location.href));

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
      {roomsWithoutDevices} pokoi bez przyporządkowanych urzadzeń, sprawdź
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

<WeatherWidget />
<button on:click={() => (open = true)}>Open</button>
<MorphingModal
  {open}
  on:open={() => (open = true)}
  on:close={() => (open = false)}
>
  <div slot="content" class="modal-content">
    
    <div class="modal z-50 fixed w-full h-full top-0 left-0 flex items-center justify-center p-8 lg:p-0">
      <div class="modal-overlay fixed w-full h-full bg-gray-900 opacity-50"></div>
      <div class="bg-white w-full lg:h-max lg:w-1/2  mx-auto rounded-lg shadow-xl z-50 overflow-y-auto">
        <div class="head bg-gray-100 py-5 px-8 text-2xl font-extrabold">
          <button class="p-2 bg-gray-200 hover:bg-gray-300 rounded-full ml-4" on:click={() => (open = false)}>
            <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 0 24 24" width="24px" fill="#000000"><path d="M0 0h24v24H0V0z" fill="none"/><path d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12 19 6.41z"/></svg>
          </button>
        </div>
        <div class="content p-8">
        </div>
      </div>
    </div>

    
    
  </div>
</MorphingModal>

