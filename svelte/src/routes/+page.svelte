<script>
  // @ts-nocheck
  // let jq = window.$;
  // import Counter from './Counter.svelte';
  // import welcome from '$lib/images/svelte-welcome.webp';
  // import welcome_fallback from '$lib/images/svelte-welcome.png';
  import { onMount } from "svelte";
  // import { setContext } from "svelte";
  import Room from "./room.svelte";
  import { writable } from "svelte/store";
  import Loader from "./loader.svelte";
  import Layout from "./+layout.svelte";

  let retryInterval = 3000;
  let errorMessage = "";
  let loadingDataState = false;

  let jsonRoomsData = [];
  const jsonRooms = writable(jsonRoomsData);
  let jsonDevicesData = [];
  const jsonDevices = writable(jsonDevicesData);
  let roomsWithoutDevices = 0;
  let retry = true;





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
      console.log(response);
    } catch (e) {
      console.log(e);
    }
  }

  function updateData() {
    //// updates data on page & roomsData
      document.getElementById("loading_dot").classList.remove("hidden");

      fetchData().then(() => {
        document.getElementById("loading_dot").classList.add("hidden");
      });

  }

  async function fetchData() {
    console.log("fetching data");
    while (retry) {
      try {
        loadingDataState = true;
        // const response = await axios.get({
        //   url: "http://cleargrasstermostat.local/JSONrooms",
        // });

        const response = await fetch(
          "http://cleargrasstermostat.local/data/JSONrooms"
        );

        if (!response.ok) {
          throw new Error(response.statusText);
        }
        jsonRoomsData = await response.json();
        jsonRooms.set(jsonRoomsData);

        const responseDevices = await fetch(
          "http://cleargrasstermostat.local/data/JSONdevices"
        );
        if (!responseDevices.ok) {
          throw new Error(responseDevices.statusText);
        }
        jsonDevicesData = await responseDevices.json();
        jsonDevices.set(jsonDevicesData);
        loadingDataState = false;
        jsonRoomsData.forEach((room) => {
          jsonDevicesData.forEach((device) => {
            if (device.mac == room.mac) {
              if (device.temp != null) {
                room.temp = device.temp;
              } else {
                room.temp = 0;
              }
              // console.log(device);
              if (device.hum != null) {
                room.humidity = device.hum;
              } else {
                room.humidity = 0;
              }
            } else {
              roomsWithoutDevices++;
            }
          });
        });
        errorMessage = "Dane pobrane";
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

  onMount(async () => {
    await fetchData();
  });
</script>

<svelte:head>
  <title>CGG1 admin</title>
  <meta name="description" content="CGG1 admin panel" />
</svelte:head>

<p>{errorMessage}</p>

<button on:click={updateData}>pobierz dane</button>

<section>
  <!-- {#if roomsWithoutDevices > 0}
    <div class="errorMessage">{errorMessage}</div>
    <div id="alert-modal-body" class="modal-body">
      {roomsWithoutDevices} pokojów bez przyporządkowanych urzadzeń, sprawdź
      <a href="/settings">ustawienia</a>
    </div>
  {/if} -->

  {#if loadingDataState === true}
    <Loader />
  {/if}

  {#await jsonDevicesData then devices}
    {#each devices as device}
      {#each jsonRoomsData as room}
        {#if device.mac === room.mac}
          <Room roomData={room} {updateRoom} />
        {/if}
      {/each}
    {/each}
  {/await}
</section>

<style>
</style>
