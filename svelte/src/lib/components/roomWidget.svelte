<script>
  // @ts-ignore
  const jq = window.$;
  export let updateRoom;
  export let roomID;

  import { getContext, onMount } from "svelte";
  import { fade } from "svelte/transition";
  import Battery from "./battery.svelte";

  let animate = false;

  function focus(node) {
    addDial(node.id.split("-")[1]);
  }

  const name = getContext("name");
  const initial = getContext("initial");
  const rooms = getContext("rooms");

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
    // try to connect to server



    let url = "http://cleargrasstermostat.local/data/relay/" + roomID + "/" + state;
    fetch(url, {
      method: "GET",
      headers: {
        "Content-Type": "application/json",
      },
    })
      .then((response) => response.json())
      .then((data) => {
        console.log("Success:", data);
      })
      .catch((error) => {
        console.error("Error:", error);
      });

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

<!-- svelte-ignore missing-declaration -->
{#if $rooms[roomID].temp}
  <div
    class="room_element sha_temp_body {heatState ? 'heat' : 'noheat'}"
    transition:fade={{ delay: random(0, 1000), duration: 500 }}
  >
    <div
      id="encoder-{$rooms[roomID].id}"
      class="enc"
      use:focus
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

            <hr class="line_" />
            <Battery level={$rooms[roomID].bat} />
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
      <div class="menu_room">
        <h1 class="roomName">
          {$rooms[roomID].name}
        </h1>

        <div class="chart-container" id="chart-container-{$rooms[roomID].id}">
          <div class="chart-container-header">Wykresy ogrzewania</div>
          <canvas class="chart" id="chart-{$rooms[roomID].id}" />

          <div class="buttons-container">
            <button class="year-scale">Rok</button>
            <button class="month-scale">Miesiąc</button>
            <button class="day-scale">Dzień</button>
          </div>
        </div>
      </div>
      <div class="height_auto" />
    </div>
  </div>
{/if}
