<script>
    import { getContext } from 'svelte';
    // @ts-ignore
    const { jsonData, updateJsonData } = getContext(jsonContext);
    import { onMount } from "svelte";
    import { setContext } from "svelte";
    /**
     * @type {{ name: any; id: any; minTemp: any; temp: any[]; humidity: any[]; mac: any; }}
     */
     export let roomData;
    

    

</script>

<div class="room">
    <h2>{roomData.name}</h2>
    <!-- Generated HTML for the room goes here, using the roomData parameter -->
    <div class="chart">
   
    </div>
</div>

<div class="room_element sha_temp_body">
    <div  id="encoder-{roomData.id}" class="enc">
      <input class="dial noselect" value="0" min-value="12" max-value="36">
      <div class="backx"><div class="heat_value">{roomData.minTemp}</div></div>
    </div>
    <div class="row top-buffer ">
        <div class="col-12">
                  <span class="sha_temp white_back">
                <span>
                    <span class="temp-data">{roomData.temp[0]}<span class="small_01">.{roomData.temp[1]}</span> <sup>°C</sup></span>
                        <hr class="line_">
                    <span class="hum-data">{roomData.humidity[0]}<span class="small_01">.{roomData.humidity[1]}</span>
                    <span class="sup">%</span>
                    <span class="hidden_span mac_device">
                        {roomData.mac}</span>
            </span>
            </span>
            </span>
    </div>
    <div class="menu_room">
        <h1 class="roomName">
{roomData.name}
</h1>

<div class="chart-container" id="chart-container-{roomData.id}">
<div class="chart-container-header">Wykresy ogrzewania</div>
<canvas class="chart" id="chart-{roomData.id}"></canvas>

<div class="buttons-container">
<button class="year-scale">Rok</button>
<button class="month-scale">Miesiąc</button>
<button class="day-scale">Dzień</button>
</div>


</div>

    </div>
    <div class="height_auto">
    </div>
    </div>
</div>`;