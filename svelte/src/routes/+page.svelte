<script>
  // @ts-nocheck

  // import Counter from './Counter.svelte';
  // import welcome from '$lib/images/svelte-welcome.webp';
  // import welcome_fallback from '$lib/images/svelte-welcome.png';
  import { onMount } from "svelte";
  import { setContext } from "svelte";
  import Room from "./room.svelte";
  import { writable } from "svelte/store";
  import Loader from "./loader.svelte";

  // const jsonRooms = writable({});
  // setContext("jsonRooms", jsonRooms);
  // const jsonDevices = writable({});
  // setContext("jsonDevices", jsonDevices);

  // let jsonRoomsDataVisible = [];

  // let jsonRoomsData = [];
  // let jsonDevicesData = [];

  // let roomsWithoutDevices = 0;
  let retryInterval = 300;
  let timeLeft = 30; // seconds
  let errorMessage = "";
  let loadingDataState = false;

  let jsonRoomsData = [];
  const jsonRooms = writable(jsonRoomsData);
  let jsonDevicesData = [];
  const jsonDevices = writable(jsonDevicesData);
  let roomsWithoutDevices = 0;
  let retry = true;

  onMount(async () => {
    await fetchData();
  });

  // async function fetchData() {
  //   loadingDataState=true;

  //   try {

  //         const responseRooms = await fetch("http://cleargrasstermostat.local/JSONrooms");
  //         if (!responseRooms.ok) {
  //             throw new Error(responseRooms.statusText);
  //         }
  //         jsonRoomsData = await responseRooms.json();
  //         jsonRooms.set(jsonRoomsData);

  //         const responseDevices = await fetch("http://cleargrasstermostat.local/JSONdevices");
  //         if (!responseDevices.ok) {
  //             throw new Error(responseDevices.statusText);
  //         }
  //         console.log("responseDevices success");
  //         jsonDevicesData = await responseDevices.json();
  //         jsonDevices.set(jsonDevicesData);
  //         loadingDataState=false;
  //         jsonRoomsData.forEach((room) => {
  //             jsonDevicesData.forEach((device) => {
  //                 let tempArray = [];
  //                 if (device.mac == room.mac) {
  //                     if (device.temp != null) {
  //                         room.temp = device.temp.split(".");
  //                     } else {
  //                         room.temp = [0, 0];
  //                     }
  //                     if (device.humidity != null) {
  //                         room.humidity = device.humidity.split(".");
  //                     } else {
  //                         room.humidity = [0, 0];
  //                     }
  //                 } else {
  //                     roomsWithoutDevices++;
  //                 }
  //             });
  //         });
  //     } catch (error) {
  //       loadingDataState=true;

  //         console.error(error);
  //         console.log("error fetching data for romms and devices");
  //         setTimeout(fetchData, 3000); // retry after 30 seconds
  //     }
  // }

  async function fetchData() {
    while (retry) {
      try {
        const response = await fetch(
          "http://cleargrasstermostat.local/JSONrooms"
        );
        if (!response.ok) {
          throw new Error(response.statusText);
        }
        jsonRoomsData = await response.json();
        jsonRooms.set(jsonRoomsData);

          const responseDevices = await fetch("http://cleargrasstermostat.local/JSONdevices");
          if (!responseDevices.ok) {
              throw new Error(responseDevices.statusText);
          }
          console.log("responseDevices success");
          jsonDevicesData = await responseDevices.json();
          jsonDevices.set(jsonDevicesData);
          loadingDataState=false;
          jsonRoomsData.forEach((room) => {
              jsonDevicesData.forEach((device) => {
                  let tempArray = [];
                  if (device.mac == room.mac) {
                      if (device.temp != null) {
                          room.temp = device.temp.split(".");
                      } else {
                          room.temp = [0, 0];
                      }
                      if (device.humidity != null) {
                          room.humidity = device.humidity.split(".");
                      } else {
                          room.humidity = [0, 0];
                      }
                  } else {
                      roomsWithoutDevices++;
                  }
              });
          });



        errorMessage = "Dane pobrane";
        retry = false;
      } catch (error) {
        //console.log(error);
        //console.log("responseRooms failed - regeneratig fetch");
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
</script>

<svelte:head>
  <title>CGG1 admin</title>
  <meta name="description" content="CGG1 admin panel" />
</svelte:head>

<p>{errorMessage}</p>

<button on:click={fetchData}>pobierz dane</button>

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
          <Room roomData={room} />
        {/if}
      {/each}
    {/each}
  {/await}
</section>

<style>
</style>
