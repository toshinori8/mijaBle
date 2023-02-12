<script >
	import { bind } from 'svelte/internal';
  import { fade, fly } from "svelte/transition";
  import Modal from "$lib/components/modal.svelte";
  // import Loader from "$lib/components/loader.svelte";
  import { onMount, setContext } from "svelte";
  import fetchStore from "../../../lib/stores/fetchData.js";

  let showEditModal = false;
  let currentConfirm = 0;
  let currentConfirmName = "";
  let deviceEdit = {};
  let deviceIndex;


  let jsonRoomsData = [];
  let jsonDevicesData = [];

  let [rooms, devices, loading, error, saveRooms, saveDevices] = fetchStore();

  let curr_roomID=0;

  // Handle funcions for edit modal 
 
  
  function handle_EditModal_save(mac){
    // find device by Index
    $devices[deviceIndex].name= deviceEdit.name;
    $devices[deviceIndex].mac= deviceEdit.mac;
    saveDevices($devices).then(() => {
    
      console.log("saved");
      showEditModal = false;
    });

  

    
  };
 
  function handle_Confirm_close(e){
    currentConfirm = e;
    showConfirm = !showConfirm;
  };
 
  const handle_EditModal_open = (e) => {
    deviceEdit = $devices.find((device) => device.mac == e);
    deviceIndex = $devices.findIndex((device) => device.mac == e);
    console.log("device Index is: " + deviceIndex);
    showEditModal = !showEditModal;
  };

  const handle_EditModal_close = () => {
    showEditModal = false;
  };

  onMount(() => {
    rooms.subscribe((value) => {
      jsonRoomsData = value;
      });
    devices.subscribe((value) => {
      jsonDevicesData = value;      
      });

  });

</script>

<!-- {deviceEdit.name} -->

{#if $rooms.length ==0}


<div    class="flex w-full flex-col items-center justify-center"  in:fade out:fly>

  <br>  <br>  <br>

  Brak urządzeń, poczekaj az się załadują

</div>

{:else}


<!-- if JsondevicesData and jsonRoomsData -->
{#if !$devices[0]}

     <!-- <div><Loader /></div> -->

  {:else}

<div class=""
in:fade={{ y: 200, duration: 100 }}
out:fade >

<div    class="flex w-full flex-col items-center justify-center  text-gray-700 p-10 "  >
<div    class="head flex justify-between bg-gray-100 py-5 px-8 text-2xl font-extrabold w-full max-w-screen-sm owHidden p-10 rounded-xl ring-8 ring-white ring-opacity-40">

<div class="title flex place-content-start"> 
  <a href="/setup" class="text-grayBlue-700  text-xl font-bold selectNONE ml-4">Pokoje</a>
  <a href="/setup/devices" class="text-grayBlue-700  text-xl font-bold selectNONE ml-4">Urządzenia</a>
</div>

<div class="flex justify-between place-content-end">




</div>

</div>


    <div transition:fade={{ delay: 200, duration: 300 }}  class="mod_content w-full max-w-screen-sm owHidden bg-white p-10 rounded-xl ring-8 ring-white ring-opacity-20"  >
      
      <div class="flex justify-between">
           
              <!-- /////// -->

              <table class="min-w-full  border-collapse block mdczujnikprz:table rounded-lg">
                <thead class="block md:table-header-group">
                  <tr
                    class="border border-grey-500 md:border-none block md:table-row absolute -top-full md:top-auto -left-full md:left-auto  md:relative " >
                    <th class="bg-gray-200 noSelect p-2 text-gray-500 font-bold md:border md:border-grey-500 text-left block md:table-cell">Nazwa</th>
                    <th class="bg-gray-200 noSelect p-2 text-gray-500 font-bold md:border md:border-grey-500 text-right block md:table-cell">Przypisany do pokóju</th>
                    <!-- <th class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell">Email Address</th>
            <th class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell">Mobile</th> -->
                    <th class="bg-gray-200 p-2 noSelect text-gray-500 font-bold md:border md:border-grey-500 text-left block md:table-cell" />
                  </tr>
                </thead>
      
                {#each $devices as device}
                  <tbody class="block md:table-row-group">
                    <tr class="bg-white border border-grey-500 md:border-none block md:table-row rounded-md">
                      <td class="p-2 md:border md:border-grey-500 sm:text-lg text-start block md:table-cell" style="min-width:250px;">
                        {device.name} 
                        </td> 
                      <td class="p-2 md:border md:border-grey-500 text-right block md:table-cell">
                        <span class="inline-block w-1/3 md:hidden font-bold text-slate-700 sm:text-start" />



                        
                         
                            {#each $rooms as room}
                              {#if device.mac==room.mac}
                                {room.name}
                              {/if}
                            {/each}
                         
                         
                        
                        
                        </td>
                      <!-- <td class="p-2 md:border md:border-grey-500 text-left block md:table-cell"><span class="inline-block w-1/3 md:hidden font-bold"></span>mkoch@yahoo.com</td>
              <td class="p-2 md:border md:border-grey-500 text-left block md:table-cell"><span class="inline-block w-1/3 md:hidden font-bold"></span>582-400-3X36</td> -->
                      <td class="table-cell p-2 md:border md:border-grey-100"
                      >
                        <span class="inline-block w-1/3 md:hidden font-bold text-center"/>
      

                     


                        <button class="button_edit text-center button_add p-1 bg-gray-200 hover:bg-gray-300 rounded-full ml-4"
                        on:click={() => handle_EditModal_open(device.mac)}
                        >
                          <img src="../images/setup-icon.svg" width="20px" alt="edycja"/>
                       
                         </button>

                      </td>
                    </tr>
                  </tbody>
                {/each}
              </table>

              <!-- //////// -->
              
    </div>
    <br><br> 
      <div class="flex justify-center">
              <div class="flex justify-end p10"> 
                <slot name="footer" />
              </div>   
      </div>
    </div>
  </div>
</div>

{/if}
 
{/if}





<Modal 
  title="Edycja"
  open={showEditModal}
  on:close={() => handle_EditModal_close()}
  > 

  <!-- {console.log("MAC : "+deviceEdit.mac)} -->

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
      bind:value= {deviceEdit.name} on:change={(e) => {deviceEdit.name = e.target.value; console.log(deviceEdit.name)}}
    />
     

    <br /><br />
    <label
      class="block uppercase text-blueGray-600 text-xs font-bold mb-2"
      for="grid-password"
    >
      Urządzenie
    </label>
    <select
      bind:value={deviceEdit.mac}
      class="form-select appearance-none block w-full px-3 py-1.5 text-base font-normal text-gray-700 bg-white bg-clip-padding bg-no-repeat border border-solid border-gray-300 rounded transition ease-in-out
          m-0
          focus:text-gray-700 focus:bg-white focus:border-blue-600 focus:outline-none"
      aria-label=""
     on:change={(e)=>{deviceEdit.mac=e.target.value;}}>


            {#each jsonRoomsData as room}
            {#if deviceEdit.mac == room.mac}
            <option selected value={room.mac}>{room.name} <span class="text-sm">({room.mac})</span></option>
            {/if}

            {#if deviceEdit.mac != room.mac}

            <option value={room.mac}>{room.name} <span class="text-sm">({room.mac})</span> </option>  
            {/if}
           
      {/each}   
      <option value=------->-------</option> 
  
    </select>
</svelte:fragment>

  <svelte:fragment slot="footer">
    <p />
    
    <button
      on:click={() => handle_EditModal_close()}
      class="mr-1 rounded bg-white px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
      type="button">Anuluj</button
    >
    <button
      on:click={() => handle_EditModal_save()}
      class="mr-1 rounded bg-red-600 px-4 py-2 text-xs font-bold uppercase text-white shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
      type="button">Zapisz</button
    >
  </svelte:fragment>
</Modal>




<style>
  .noSelect{
    user-select: none;
  }
  button {
    user-select: none;
  }
.head{
  border-radius: 10px 10px 0px 0px;
    box-shadow: none;
    width: 39%;
    padding-top: 10px;
    padding-bottom: 10px;
    user-select: none;
    box-shadow: 6px 1px 24px -2px rgb(0 0 0 / 10%);
    place-content: space-between;
  }
  .button_add{
    opacity: 0.5;
    transform: rotate(45deg);
    transition: all ease 0.5s;
  }

  .button_edit{
    opacity: 0.5;
    transform: rotate(45deg);
    transition: all ease 0.5s;
  }

  .button_edit:hover{
    background-color: rgba(253, 119, 22, 1.000);
    transform: rotate(45deg);
    animation: rotate 1s;
    transition: all ease 0.5s;
  } 
  .button_edit:hover img{
    transform: scale(1.2);

  }

  /* .button_del{
    opacity: 0.5;
    transform: rotate(90deg);
    transition: all ease 0.5s;
  } */
  /* keyframes  */
  @keyframes rotate {
    0% {
      transform: rotate(0deg);
    }
    100% {
      transform: rotate(90deg);
    }
  }
  /* .button_del:hover{
    background-color: rgba(253, 119, 22, 1.000);
    transform: rotate(90deg);
    animation: rotate 1s;
    transition: all ease 0.5s;
  } */




  @media screen and (max-width: 768px) {
    .head .title {
      display: none;
    }
  }
  

</style>


