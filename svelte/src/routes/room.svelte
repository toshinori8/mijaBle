<script>
  // @ts-ignore
  const jq = window.$;
 
  /**
     * @type {(arg0: any) => void}
     */
   export let updateRoom;
  /**
   * @type {string | number}
   */
  export let roomID;

  import { getContext, onMount } from "svelte";
    import Battery from "./battery.svelte";
  // @ts-ignore
  // /**
  //  * @type {{ name: any; id: any; minTemp: any; temp: any[]; hum: any[]; mac: any; }}
  //  */
  // export let roomID;
  // let roomData = getContext("jsonRooms").find(
  //   (/** @type {{ id: any; }} */ room) => room.id == roomID
  // );

  function focus(node) {
      addDial(node.id.split("-")[1]); 
  }

  

  const name = getContext("name");
  const initial = getContext("initial");
  const rooms = getContext("rooms");

  /**
   * @param {string | number} element_id
   */
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
          release: function (/** @type {any} */ v) {
            $rooms[roomID].minTemp = v.toFixed(1).toString();
            updateRoom($rooms[roomID].id);
          },
          change: function (/** @type {any} */ v) {
            $rooms[roomID].minTemp = v.toFixed(1).toString();
          },
        });

   
  }

  onMount(() => {
   


  });
  
</script>

{#if $rooms[roomID].temp}
  <div class="room_element sha_temp_body">
    <div id="encoder-{$rooms[roomID].id}" class="enc" use:focus>
      <input
        class="dial noselect"
        data-min="12"
        data-max="30"
        data-step="0.1"
        value="{$rooms[roomID].minTemp}"
      />
      <div class="backx">
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
              <span class="temp-data">{$rooms[roomID].temp}<sup>°C</sup></span>
            {/if}

            <hr class="line_" />
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
