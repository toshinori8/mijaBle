import { c as create_ssr_component, e as escape, i as is_promise, n as noop, f as each, v as validate_component } from "../../chunks/index.js";
/* empty css                                                  */const Room = create_ssr_component(($$result, $$props, $$bindings, slots) => {
  let { updateRoom } = $$props;
  let { roomData } = $$props;
  if ($$props.updateRoom === void 0 && $$bindings.updateRoom && updateRoom !== void 0)
    $$bindings.updateRoom(updateRoom);
  if ($$props.roomData === void 0 && $$bindings.roomData && roomData !== void 0)
    $$bindings.roomData(roomData);
  return `<div class="${"room_element sha_temp_body"}"><div id="${"encoder-" + escape(roomData.id, true)}" class="${"enc"}"><input class="${"dial noselect"}" value="${"0"}" data-min="${"12"}" data-max="${"30"}" data-step="${"0.1"}">
    <div class="${"backx"}"><div class="${"heat_value"}">${escape(roomData.minTemp)}</div></div></div>
  <div class="${"row top-buffer "}"><div class="${"col-12"}"><span class="${"sha_temp white_back"}"><span><span class="${"temp-data"}">${escape(roomData.temp[0])}<span class="${"small_01"}">.${escape(roomData.temp[1])}</span>
            <sup>°C</sup></span>
          <hr class="${"line_"}">
          <span class="${"hum-data"}">${escape(roomData.humidity[0])}<span class="${"small_01"}">.${escape(roomData.humidity[1])}</span>
            <span class="${"sup"}">%</span>
            <span class="${"hidden_span mac_device"}">${escape(roomData.mac)}</span></span></span></span></div>
    <div class="${"menu_room"}"><h1 class="${"roomName"}">${escape(roomData.name)}</h1>

      <div class="${"chart-container"}" id="${"chart-container-" + escape(roomData.id, true)}"><div class="${"chart-container-header"}">Wykresy ogrzewania</div>
        <canvas class="${"chart"}" id="${"chart-" + escape(roomData.id, true)}"></canvas>

        <div class="${"buttons-container"}"><button class="${"year-scale"}">Rok</button>
          <button class="${"month-scale"}">Miesiąc</button>
          <button class="${"day-scale"}">Dzień</button></div></div></div>
    <div class="${"height_auto"}"></div></div></div>`;
});
const Page = create_ssr_component(($$result, $$props, $$bindings, slots) => {
  let errorMessage = "";
  let jsonRoomsData = [];
  let jsonDevicesData = [];
  async function updateRoom(id) {
    let room = jsonRoomsData.find(
      /** @type {{ id: any; }} */
      (room2) => room2.id == id
    );
    try {
      const response = await fetch("http://cleargrasstermostat.local/updateRoom", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(room)
      });
      if (!response.ok) {
        throw new Error(response.statusText);
      }
      console.log(response);
    } catch (e) {
      console.log(e);
    }
  }
  if ($$props.updateRoom === void 0 && $$bindings.updateRoom && updateRoom !== void 0)
    $$bindings.updateRoom(updateRoom);
  return `${$$result.head += `<!-- HEAD_svelte-13s7ykz_START -->${$$result.title = `<title>CGG1 admin</title>`, ""}<meta name="${"description"}" content="${"CGG1 admin panel"}"><!-- HEAD_svelte-13s7ykz_END -->`, ""}

<p>${escape(errorMessage)}</p>

<button>pobierz dane</button>

<section>

  ${``}

  ${function(__value) {
    if (is_promise(__value)) {
      __value.then(null, noop);
      return ``;
    }
    return function(devices) {
      return `
    ${each(devices, (device) => {
        return `${each(jsonRoomsData, (room) => {
          return `${device.mac === room.mac ? `<p>room data ID - ${escape(room.id)}</p>
          ${validate_component(Room, "Room").$$render($$result, { roomData: room, updateRoom }, {}, {})}` : ``}`;
        })}`;
      })}
  `;
    }(__value);
  }(jsonDevicesData)}
</section>`;
});
export {
  Page as default
};
