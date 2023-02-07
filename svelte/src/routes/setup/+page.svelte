<script lang>
  import { fade } from "svelte/transition";
  import { bind, subscribe } from "svelte/internal";
  // import FormEditRoom from "$lib/components/formEditRoom.svelte";
  // import { formatPostcssSourceMap } from "vite";
  import Modal from "$lib/components/modal.svelte";
  import Loader from "$lib/components/loader.svelte";
  import { onMount, getContext } from "svelte";
  import fetchStore from "../../lib/components/fetchData.js";

  let showEditModal = false;
  let showAddModal = false;

  let curr_roomID=0;


  const handleAddModal = (e) => {
    showAddModal = !showAddModal;
  };
  const handle_EditModal_open = (e) => {
    curr_roomID = e;
    showEditModal = !showEditModal;
  };
  const handle_EditModal_close = () => {
    showEditModal = false;
  };
  let jsonRoomsData = {};
  let jsonDevicesData = {};

  let [rooms, devices, loading, error, saveRooms] = fetchStore();

  function changeData() {
    jsonRoomsData[0].name = "zmieniony";
    rooms.set(jsonRoomsData);
  }

  onMount(() => {
    rooms.subscribe((value) => {
      jsonRoomsData = value;
    });
    devices.subscribe((value) => {
      jsonDevicesData = value;
    });
  });
</script>

<!-- <button on:click={changeData}>zmień dane</button> -->

{#if !jsonRoomsData[0]}
  <div transition:fade><Loader /></div>
{:else}
  <div class="pt-20" transition:fade={{ delay: 500, duration: 500 }}>
    <div
      class="mx-auto max-w-11/12 justify-center px-6 md:flex md:space-x-6 xl:px-0"
    >
      <div class="md:w-2/3 rounded-lg">
        <table class="min-w-full  border-collapse block md:table rounded-lg">
          <thead class="block md:table-header-group">
            <tr
              class="border border-grey-500 md:border-none block md:table-row absolute -top-full md:top-auto -left-full md:left-auto  md:relative "
            >
              <th
                class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell"
                >Nazwa</th
              >
              <th
                class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell"
                >Device MAC</th
              >
              <!-- <th class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell">Email Address</th>
      <th class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell">Mobile</th> -->
              <th
                class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell"
              />
            </tr>
          </thead>

          {#each jsonRoomsData as room}
            <tbody class="block md:table-row-group">
              <tr
                class="bg-white border border-grey-500 md:border-none block md:table-row"
              >
                <td
                  class="p-2 md:border md:border-grey-500 text-left block md:table-cell"
                  ><span
                    class="inline-block w-1/3 md:hidden font-bold"
                  />{room.name}</td
                >
                <td
                  class="p-2 md:border md:border-grey-500 text-left block md:table-cell"
                  ><span
                    class="inline-block w-1/3 md:hidden font-bold"
                  />{room.mac}</td
                >
                <!-- <td class="p-2 md:border md:border-grey-500 text-left block md:table-cell"><span class="inline-block w-1/3 md:hidden font-bold"></span>mkoch@yahoo.com</td>
        <td class="p-2 md:border md:border-grey-500 text-left block md:table-cell"><span class="inline-block w-1/3 md:hidden font-bold"></span>582-400-3X36</td> -->
                <td
                  class="p-2 md:border md:border-grey-500 text-right block md:table-cell"
                >
                  <span class="inline-block w-1/3 md:hidden font-bold" />

                  <button
                    class="mr-1 rounded bg-white px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
                    type="button"
                    on:click={() => handle_EditModal_open(room.id)}
                    >Edycja</button
                  >
                  <button
                    class="mr-1 rounded bg-red-600 px-4 py-2 text-xs font-bold uppercase text-white shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
                    type="button">Usuń</button
                  >
                </td>
              </tr>
            </tbody>
          {/each}
        </table>
      </div>
    </div>
  </div>
{/if}

<!-- svelte-ignore missing-declaration -->
<!-- {#if jsonRoomsData!=undefined }
<div class="flex justify-between">
  <h1 class="text-2xl font-bold">Pokoje</h1>
  <button
    class="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded">
    Odśwież
  </button>
</div>


{#each jsonRoomsData as room} 
<p>Room roomID={room.id} {room.name} </p>
{/each}


 {/if} -->

<!-- <button on:click={() => handleToggleModal()}>Open modal</button> -->

<Modal
  title="edit room"
  open={showEditModal}
  close={() => handle_EditModal_close()}
>
  <svelte:fragment slot="body">
    <label
      class="block uppercase text-blueGray-600 text-xs font-bold mb-2"
      for="grid-password"
    >
      Nazwa
    </label>
    <input
      type="text"
      class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150"
      value="Lucky"
    />

    <br /><br />
    <label
      class="block uppercase text-blueGray-600 text-xs font-bold mb-2"
      for="grid-password"
    >
      Urządzenie
    </label>
    <select
      class="form-select appearance-none block w-full px-3 py-1.5 text-base font-normal text-gray-700 bg-white bg-clip-padding bg-no-repeat border border-solid border-gray-300 rounded transition ease-in-out
          m-0
          focus:text-gray-700 focus:bg-white focus:border-blue-600 focus:outline-none"
      aria-label="Default select example"
    >
      {#each jsonDevicesData as device}
            {#if jsonRoomsData[curr_roomID].mac == device.mac}
            <option selected value={device.mac}>{device.mac} ({device.name}) </option>
            {/if}
            {#if jsonRoomsData[curr_roomID].mac != device.mac}

            <option value={device.mac}>{device.mac} ({device.name}) </option>  
            {/if}
      {/each}  
      
  
    </select>

    <input
      type="text" disabled
      class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150"
      value={jsonDevicesData[0].name}
    />

  </svelte:fragment>

  <svelte:fragment slot="footer">
    <p />
    <button
      on:click={() => handle_EditModal_close()}
      class="mr-1 rounded bg-white px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
      type="button">Anuluj</button
    >
    <button
      on:click={() => handle_EditModal_close()}
      class="mr-1 rounded bg-red-600 px-4 py-2 text-xs font-bold uppercase text-white shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
      type="button">Zapisz</button
    >
  </svelte:fragment>
</Modal>


<style>


</style>


<!-- 

<Modal title="Edit your details" open={showAddModal} on:close={() => handleAddToggleModal()}>
<svelte:fragment slot="body">
<form>
<h6 class="text-blueGray-400 text-sm mt-3 mb-6 font-bold uppercase">
          User Information         </h6> 
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
</Modal> -->
