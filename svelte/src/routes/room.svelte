<script>
  // @ts-ignore

  const jq = window.$;



 /**
     * @type {(arg0: any) => (arg0: any) => void}
     */
  export let updateRoom;

  import { getContext, onMount } from "svelte";
  // @ts-ignore
  /**
   * @type {{ name: any; id: any; minTemp: any; temp: any[]; humidity: any[]; mac: any; }}
   */
  export let roomData;
  // get writable

  onMount(() => {
    jq("#encoder-" + roomData.id)
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
          roomData.minTemp = v.toFixed(1).toString();
          updateRoom(roomData.id);
        },
        change: function (/** @type {any} */ v) {
          roomData.minTemp = v.toFixed(1).toString();
        },
      });
  });
</script>





<div class="room_element sha_temp_body">
  <div id="encoder-{roomData.id}" class="enc">
    <input
      class="dial noselect"
      value="0"
      data-min="12"
      data-max="30"
      data-step="0.1"
    />
    <div class="backx"><div class="heat_value">{roomData.minTemp}</div></div>
  </div>
  <div class="row top-buffer ">
    <div class="col-12">
      <span class="sha_temp white_back">
        <span>
          <span class="temp-data"
            >{roomData.temp[0]}<span class="small_01">.{roomData.temp[1]}</span>
            <sup>°C</sup></span
          >
          <hr class="line_" />
          <span class="hum-data"
            >{roomData.humidity[0]}<span class="small_01"
              >.{roomData.humidity[1]}</span
            >
            <span class="sup">%</span>
            <span class="hidden_span mac_device"> {roomData.mac}</span>
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
        <canvas class="chart" id="chart-{roomData.id}" />

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
