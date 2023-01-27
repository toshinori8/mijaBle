<script>
  // @ts-ignore

  const jq = window.$;



 /**
     * @type {(arg0: any) => (arg0: any) => void}
     */
  export let updateRoom;
  /**
     * @type {string | number}
     */
   export let roomID;

  import { getContext, onMount } from "svelte";
  // @ts-ignore
  // /**
  //  * @type {{ name: any; id: any; minTemp: any; temp: any[]; humidity: any[]; mac: any; }}
  //  */
  // export let roomID;
  // let roomData = getContext("jsonRooms").find(
  //   (/** @type {{ id: any; }} */ room) => room.id == roomID
  // );
	
	const name = getContext('name')
	const initial = getContext('initial')
  const rooms = getContext("rooms");
  
  onMount(() => {
    
   

    jq("#encoder-" + $rooms[roomID].id)
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
  });
</script>


<!-- <div>
  <h1>roomID {roomID}!</h1>
  <label for="inpuT">Name change</label>
  <input bind:value={$name} id="inpuT"/>
  <hr>
  <p>Rooms value: {$rooms[roomID].name}</p>

      
</div> -->
<p>{console.log($rooms[roomID])}</p>

{#if $rooms[roomID].temp}
<div class="room_element sha_temp_body">
  <div id="encoder-{$rooms[roomID].id}" class="enc">
    <input
      class="dial noselect"
      value="0"
      data-min="12"
      data-max="30"
      data-step="0.1"
    />
    <div class="backx"><div class="heat_value">{$rooms[roomID].minTemp}</div></div>
  </div>
  <div class="row top-buffer ">
    <div class="col-12">
      <span class="sha_temp white_back">
        <span>

              
            
             
              {#if $rooms[roomID].temp.toString().includes('.')}
                <span class="temp-data">{$rooms[roomID].temp.toString().split('.')[0]}
                  <span class="small_01">.{$rooms[roomID].temp.toString().split('.')[1]}</span>
                  <sup>°C</sup>
                </span>
              {:else}
                <span class="temp-data">{$rooms[roomID].temp}<sup>°C</sup></span>
              {/if}
             
            <hr class="line_" />
            {#if $rooms[roomID].humidity} 
              {#if $rooms[roomID].humidity.toString().includes('.')}
                  <span class="hum-data">{$rooms[roomID].humidity.toString().split('.')[0]}
                    <span class="sup">%</span></span>
              {:else}
                  <span class="hum-data">{$rooms[roomID].humidity.toString()}
                    <span class="sup">%</span></span>
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