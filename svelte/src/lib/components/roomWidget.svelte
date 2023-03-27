<script>
  // @ts-ignore
  const jq = window.$;

  export let updateRoom;
  export let roomID;

  import { getContext, onMount } from "svelte";
  import { fade } from "svelte/transition";
  import Battery from "./battery.svelte";
  import Modal from "$lib/components/modal.svelte";

  let sendingOn = false;
  let animate = false;

  let showModal = false;
  const handleToggleModal = () => {
    showModal = !showModal;
  };

  function addDialOnstart(node) {
    addDial(node.id.split("-")[1]);
  }

  // const name = getContext("name");
  // const initial = getContext("initial");
  const rooms = getContext("rooms");
  const devices = getContext("devices");
  
  function addDial(element_id) {
    jq("#encoder-" + element_id)
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
        displayPrevious: true,
        linecap: "round",
        cursor: 20,

        release: function (/** @type {any} */ v) {
          $rooms[roomID].minTemp = v.toFixed(1).toString();
          updateRoom($rooms[roomID].id);
          updHeatState();
        },
        change: function (/** @type {any} */ v) {
          $rooms[roomID].minTemp = v.toFixed(1).toString();
          updHeatState();
        },
      });
  }

  function random(min, max) {
    return Math.floor(Math.random() * (max - min + 1) + min);
  }

  // send heat on/off signal to relay
  function switchRealay(roomID, state) {
    if (sendingOn == true) {
      sendingOn = false;
      let url =
        "http://cleargrasstermostat.local/data/relay/" + roomID + "/" + state;
      fetch(url, {
        method: "GET",
        headers: {
          "Content-Type": "application/json",
        },
      })
        .then((response) => response.json())
        .then((data) => {
          console.log("Success:", data);
          sendingOn = false;
        })
        .catch((error) => {
          console.error("Error:", error);
        });
    }
  }

  // let heatState = $rooms[roomID].heatState;
  let heatState = () => {
    $rooms[roomID].heatState ? "heat" : "noheat" | "noheat";
  };

  $: updHeatState(
    $rooms[roomID].temp,
    $rooms[roomID].minTemp,
    $rooms[roomID].heatState
  );

  function updHeatState() {
    console.log("updHeatState");
    if ($rooms[roomID].temp < $rooms[roomID].minTemp) {
      heatState = true;
      switchRealay(roomID, true);
    } else {
      switchRealay(roomID, false);
      heatState = false;
    }
  }
  onMount(() => {});
</script>



  {#each $devices as device}
      {#if device.mac == $rooms[roomID].mac}
     <p style="display: none">
      {$rooms[roomID].temp = device.temp}
      {$rooms[roomID].hum = device.hum}
      {$rooms[roomID].bat = device.bat}
      </p>
      {/if}
  {/each}

    {#if $rooms[roomID].temp}
  <div class="justify-between mb-6 sm:flex sm:justify-start">
    <!-- <img src="http://127.0.0.1:5500/serve/wac.png" alt="product-image" class="w-full rounded-lg sm:w-56" /> -->

    <div
      class="room_element {heatState
        ? 'heat'
        : 'noheat'} w-full rounded-lg sm:w-56"
      transition:fade={{ delay: random(0, 1000), duration: 500 }}
    >
      <div
        id="encoder-{$rooms[roomID].id}"
        class="enc"
        use:addDialOnstart
        use:updHeatState
      >
        <input
          class="dial noselect"
          data-min="12"
          data-max="30"
          data-step="0.1"
          value={$rooms[roomID].minTemp}
        />
        <div class="backx" transition:fade>
          <div class="heat_value">{$rooms[roomID].minTemp}</div>
        </div>
      </div>
      <div class="row top-buffer ">
        <div class="col-12">
          <span class="sha_temp white_back">
            <span>
              {#if $rooms[roomID].temp.toString().includes(".")}
                <span class="temp-data"
                  >{$rooms[roomID].temp.toString().split(".")[0]}
                  <span class="small_01"
                    >.{$rooms[roomID].temp.toString().split(".")[1]}</span
                  >
                  <sup>°C</sup>
                </span>
              {:else}
                <span class="temp-data" on:change={updHeatState()}
                  >{$rooms[roomID].temp}<sup>°C</sup></span
                >
              {/if}

              <!-- <hr class="line_" /> -->
              <Battery level={$rooms[roomID].bat}/>
              {#if $rooms[roomID].hum}
                {#if $rooms[roomID].hum.toString().includes(".")}
                  <span class="hum-data"
                    >{$rooms[roomID].hum.toString().split(".")[0]}
                    <span class="sup">%</span></span
                  >
                {:else}
                  <span class="hum-data"
                    >{$rooms[roomID].hum.toString()}
                    <span class="sup">%</span></span
                  >
                {/if}
              {/if}
            </span>
          </span>
        </div>
      </div>
    </div>

    <div
      class="backMenu  sm:ml-4 sm:flex sm:w-full sm:justify-between rounded-lg  p-6  text-white al"
    >
      <div
        class="room_menu mt-5 sm:mt-0 align-middle text-right w-full md:text-right "
      >
        <p class="_title">{$rooms[roomID].name}</p>

        <button on:click={() => handleToggleModal()}>Open modal</button>

        <p class="text-black text-sm">_____________</p>

        <p class="text-black text-sm">_____________</p>
        <p class="text-black text-sm">_____________</p>
      </div>
      <div
        class="mt-4 flex justify-between sm:space-y-6 sm:mt-0 sm:block sm:space-x-6 "
      >
        <div class="flex items-center border-gray-100 ">
          <!-- <span class="cursor-pointer rounded-l bg-gray-100 py-1 px-3.5 duration-100 hover:bg-blue-500 hover:text-blue-50"> - </span>
        <input class="h-8 w-8 border bg-white text-center text-xs outline-none" type="number" value="2" min="1" />
        <span class="cursor-pointer rounded-r bg-gray-100 py-1 px-3 duration-100 hover:bg-blue-500 hover:text-blue-50"> + </span> -->
        <!-- <div class="chart-container" id="chart-container-{$rooms[roomID].id}">
            <div class="chart-container-header">Wykresy ogrzewania</div>
            <canvas class="chart" id="chart-{$rooms[roomID].id}" />
            <div class="buttons-container">
              <button class="year-scale">Rok</button>
              <button class="month-scale">Miesiąc</button>
              <button class="day-scale">Dzień</button>
            </div>
          </div> -->
        <!-- </div> -->
        <!-- <div class="height_auto" /> -->
        <!-- </div> -->
      </div>
        <div class="flex items-center space-x-4">
          <!-- <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" class="h-5 w-5 cursor-pointer duration-150 hover:text-red-500">
          <path stroke-linecap="round" stroke-linejoin="round" d="M6 18L18 6M6 6l12 12" />
        </svg>  -->
        </div>
      </div>
    </div>
  </div>
{/if}

<Modal title="Edit your details" open={showModal} on:close={() => handleToggleModal()}>
  <svelte:fragment slot="body">
      
  

  <form>
          <!-- <h6 class="text-blueGray-400 text-sm mt-3 mb-6 font-bold uppercase">
            User Information
          </h6> -->
          <div class="flex flex-wrap">
            <div class="w-full lg:w-6/12 px-4">
              <div class="relative w-full mb-3">
                  <br><br>
                <label class="block uppercase text-blueGray-600 text-xs font-bold mb-2" for="grid-password">
                  Username
                </label>
                <input type="text" class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150" value="lucky.jesse">
              </div>
            </div>
            <div class="w-full lg:w-6/12 px-4">
              <div class="relative w-full mb-3">
                <label class="block uppercase text-blueGray-600 text-xs font-bold mb-2" for="grid-password">
                  Email address
                </label>
                <input type="email" class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150" value="jesse@example.com">
              </div>
            </div>
            <div class="w-full lg:w-6/12 px-4">
              <div class="relative w-full mb-3">
                <label class="block uppercase text-blueGray-600 text-xs font-bold mb-2" for="grid-password">
                  First Name
                </label>
                <input type="text" class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150" value="Lucky">
              </div>
            </div>
            <div class="w-full lg:w-6/12 px-4">
              <div class="relative w-full mb-3">
                <label class="block uppercase text-blueGray-600 text-xs font-bold mb-2" for="grid-password">
                  Last Name
                </label>
                <input type="text" class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150" value="Jesse">
              </div>
            </div>
          </div>
        </form>
  
  

  
      </svelte:fragment>
  </Modal>

<style>

  
  .room_menu ._title {
    color: rgba(0, 0, 0, 0.5);
    padding: 12px;
    padding-right: 21px;
    border-radius: 114px;
    background-color: #fcfcfc;
    width: auto;
    background: linear-gradient(
      259deg,
      rgba(255, 255, 255, 0.1) 0%,
      rgba(255, 255, 255, 0.5) 10%,
      rgba(255, 255, 255, 0) 100%
    );
  }
  .top-buffer {
    margin-top: 20px;
  }

  .backMenu {
    box-shadow: 21px 37px 20px -48px rgb(1 1 1 / 60%);
    background: linear-gradient(
      270deg,
      rgba(255, 255, 255, 0.8) 0%,
      rgba(255, 255, 255, 0.8) 10%,
      rgba(255, 255, 255, 0) 100%
    );
  }

  .white_back {
    background-color: rgba(241, 241, 241, 1.3) !important;
  }
  .room_element .sha_tile .white_back {
    -webkit-user-select: none; /* Safari */
    -ms-user-select: none; /* IE 10 and IE 11 */
    user-select: none; /* Standard syntax */
    position: absolute;
    /* z-index: 120; */
  }

  .room_element .sha_temp {
    transition: all ease-out 0.5s;
  }
  .room_element.heat .sha_temp {
    box-shadow: 1px 4px 20px -4px rgb(252 119 16),
      3px 3px 5px -1px rgba(0, 0, 0, 0.15),
      -6px -6px 10px -1px rgba(255, 255, 255, 0.7);
  }

  .room_element .sha_temp {
    -webkit-user-select: none; /* Safari */
    -ms-user-select: none; /* IE 10 and IE 11 */
    user-select: none; /* Standard syntax */
    margin-top: 20px;
    /* z-index: 10000; */
    position: absolute;
    display: inline-block;
    width: 230px;
    height: 230px;
    border-radius: 100%;
    text-align: center;
    box-shadow: 30px 30px 30px -10px rgba(0, 0, 0, 0.1),
      -10px -10px 20px -10px rgba(255, 255, 255, 0.65),
      -0.5px -0.5px 0 rgba(255, 255, 255, 1), 0.5px 0.5px 0 rgba(0, 0, 0, 0.02);
  }

  .room_element .sha_temp > span {
    -ms-user-select: none;
    display: inline-block;
    transform: translateY(-60%);
    position: absolute;
    top: 111px;
    left: 17px;
    -ms-user-select: none;
    user-select: none;
  }

  .room_element .sha_temp > span > span.temp-data {
    user-select: none;
    font-size: 78px;
    font-weight: bolder;
    margin-top: -65px;
    margin-left: -78px;
    position: absolute;
    display: block;
    text-align: right;
    width: 200px;
    color: rgba(49, 69, 106, 1);
    font-family: "lcd", Fallback, sans-serif;
    text-shadow: 3px 3px 3px rgb(120 120 100 / 16%);
  }
  .room_element .small_01 {
    position: absolute;
    margin-top: 37px;
    -webkit-user-select: none;
    -ms-user-select: none;
    user-select: none;
    font-size: 44px;
    font-weight: bolder;
  }
  .room_element sup {
    -webkit-user-select: none; /* Safari */
    -ms-user-select: none; /* IE 10 and IE 11 */
    user-select: none; /* Standard syntax */
    position: relative;
    left: 120px;
    font-size: 70px;
  }

  .room_element .sha_temp > span > span.hum-data {
    -webkit-user-select: none;
    -ms-user-select: none;
    user-select: none;
    position: absolute;
    font-size: 45px;
    font-weight: normal;
    margin-top: 14px;
    margin-left: 65px;
    display: block;
    width: 55px;
    text-align: right;
    color: rgba(49, 69, 106, 1);
    font-family: "lcd", Fallback, sans-serif;
    text-shadow: 3px 3px 3px rgb(120 120 100 / 13%);
  }
  .room_element span.hum-data .small_01 {
    -webkit-user-select: none; /* Safari */
    -ms-user-select: none; /* IE 10 and IE 11 */
    user-select: none; /* Standard syntax */
    font-size: 30px;
    font-weight: bolder;
  }
  .room_element span.hum-data span.sup {
    -webkit-user-select: none;
    -ms-user-select: none;
    user-select: none;
    position: absolute;
    top: 31px;
    left: 56px;
    /* left: 120px; */
    font-size: 15px;
    font-weight: 400;
    text-shadow: none;
    letter-spacing: 15px;
    font-family: system-ui;
    opacity: 0.9;
  }

  .room_element .sha_temp > span > span.temp-data sup {
    -webkit-user-select: none;
    -ms-user-select: none;
    user-select: none;
    position: absolute;
    font-size: 15px;
    font-weight: 500;
    top: 30%;
    margin-left: 96px;
  }

  .room_element .sha_temp:before {
    position: absolute;
    content: "";
    display: block;
    width: 97%;
    height: 97%;
    border: rgba(55, 55, 55, 0.1) 4px black;
    border-radius: inherit;
    left: 50%;
    top: 50%;
    transform: translateX(-50%) translateY(-50%);
    box-shadow: inset 4px 4px 6px -1px rgba(0, 0, 0, 0.03),
      inset -4px -4px 6px -1px rgba(0, 0, 0, 0.05);

    opacity: 1;
  }

  .room_element {
    display: table-footer-group;
    width: 200px;
    height: 300px;
  }
  
  .room_element .sha_temp:after {
    position: absolute;
    content: "";
    display: block;
    width: 78%;
    height: 78%;
    border-radius: inherit;
    left: 50%;
    top: 50%;
    background-color: rgba(0, 0, 0, 0.025);
    border: 1px solid rgba(0, 0, 0, 0.01);
    transform: translateX(-50%) translateY(-50%);
    box-shadow: inset 4px 4px 6px -1px rgba(0, 0, 0, 0.02),
      inset -4px -4px 16px -1px rgba(5, 5, 5, 0.04);
  }

  .room_element .sha_tile > div span.tile_info span {
    display: block;
    font-weight: 400;
    color: rgba(49, 69, 106, 0.5);
  }

  /** End Section Body 2 Style **/

  @keyframes sheen {
    100% {
      transform: rotateZ(60deg) translate(1em, -9em);
    }
  }

  @media only screen and (max-width: 600px) {
  }

  .room_element .enc {
    background: transparent;
    opacity: 0;
    position: absolute;
    /* left: 35px; */
    margin-left: 5px;
    width: 230px;
    /* top: 51px; */
    height: 229px;
    z-index: 8;
    /* height: 229px; */
    margin-top: 45px;
    /* border-radius: 122px; */
    /* padding-top: 120px; */
    /* background-color: rgba(0,0,0,0.1); */
    transition: all ease 0.5s;
    -webkit-user-select: none; /* Safari */
    -ms-user-select: none; /* IE 10 and IE 11 */
    user-select: none; /* Standard syntax */
  }

  .room_element .enc .backx {
    position: absolute;
    width: 161px;
    background: rgba(235, 235, 235, 0.98);
    height: 166px;
    margin-top: -188px;
    margin-left: 33px;
    border-radius: 99px;
    transition: all ease 0.5s;
    opacity: 1;
    z-index: 90000000;
    -webkit-user-select: none;
    -moz-user-select: none;
    user-select: none;
  }

  .room_element .enc:hover {
    background: none;
    opacity: 1;
    transition: all ease 0.5s;
  }
  .room_element .enc .backx .heat_value {
    -webkit-user-select: none; /* Safari */
    -ms-user-select: none; /* IE 10 and IE 11 */
    user-select: none; /* Standard syntax */
    font-size: 63px;
    margin-left: 9px;
    font-weight: bold;
    backdrop-filter: 1px;
    margin-top: 28px;
    font-family: "lcd", Fallback, sans-serif;
    color: rgba(48, 67, 104, 1);
    background: transparent;
    text-shadow: 3px 3px 3px rgb(120 120 100 / 13%);
  }

  .room_element .enc .backx .heat_value:last-letter {
    font-size: 20px;
    font-weight: 500;
    color: rgba(48, 67, 104, 1);
    background: transparent;
    text-shadow: 3px 3px 3px rgb(120 120 100 / 13%);
    margin-left: 5px;
    margin-top: 10px;
    font-family: "lcd", Fallback, sans-serif;
  }
</style>
