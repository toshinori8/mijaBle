<script >
  import { fade, fly } from "svelte/transition";
  import Modal from "$lib/components/modal.svelte";
  import { onMount} from "svelte";
  import fetchStore from "../../../lib/stores/fetchData.js";
  import { createEventDispatcher } from 'svelte';
  
  import { page } from "$app/stores";

  const dispatchRoomMac = createEventDispatcher();

  let showEditModal = false;
  let showAddModal = false;
  let showConfirm = false;
  let currentConfirm = 0;
  let currentConfirmName = "";
  
  let newRoom = {
    id: 0,
    name: "",
    temp: 0,
    hum:0,
    setTemp: 0,
    mac: ""
  };

  let jsonRoomsData = [];
  let jsonDevicesData = [];

  let [rooms, devices, loading, error, saveRooms, saveDevices] = fetchStore();


  let curr_roomID=0;


  // Handle funcions for edit modal 
 

  function handle_AddModal_close(){
    showAddModal = !showAddModal;
  };

  function handle_AddModal_add(){

    let maxId = 0;
      for (let i = 0; i < jsonRoomsData.length; i++) {
        if (jsonRoomsData[i].id >= maxId) {
          maxId = parseFloat(jsonRoomsData[i].id)+1;
      }
    }

    newRoom.id = maxId;

    jsonRoomsData.push(newRoom);
    jsonRoomsData=jsonRoomsData;
    rooms.set(jsonRoomsData);
    
    saveRooms(jsonRoomsData).then(() => {
    
    console.log("saved");
    showAddModal = false;
  });

  }

  function handle_EditModal_save(){
    
    saveRooms(jsonRoomsData).then(() => {
    
    console.log("saved");
    showAddModal = false;
    });

    showEditModal = !showEditModal;
  };
 


  function handle_Confirm_close(e){
    currentConfirm = e;
    showConfirm = !showConfirm;
  };
 
 
  function handle_Confirm_del(e){
      
      const filteredRooms = jsonRoomsData.filter(room => room.id !== e)
      rooms.set(filteredRooms);
  
      saveRooms(jsonRoomsData).then(() => {
    
    console.log("saved");
      handle_Confirm_close(); 
      showEditModal = false;

});

  }
  

  const handle_EditModal_open = (e) => {
    curr_roomID = e;
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



{#if $rooms.length ==0}


<div    class="flex w-full flex-col items-center justify-center"  in:fade out:fly>

  <br>  <br>  <br>
<button class="button_add p-1 bg-gray-200 hover:bg-gray-300 rounded-full ml-4"
  on:click={() => handle_AddModal_close()}>
  <svg xmlns="http://www.w3.org/2000/svg" height="60px" width="60px" viewBox="0 0 24 24" fill="#000000" class="button_"><path d="M0 0h24v24H0V0z" fill="none" class="s-2bKfpBUBwHrG"></path><path d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12 19 6.41z" class="button|"></path></svg>
</button>

</div>

{:else}


<!-- if JsondevicesData and jsonRoomsData -->
{#if !$rooms[0] || !$devices[0]}

        

      <!-- <div><Loader /></div> -->


  {:else}

<div class=""
in:fade={{ y: 200, duration: 100 }}
out:fade >

<div    class="flex w-full flex-col items-center justify-center  text-gray-700 p-10 "  >
<div    class="head flex justify-between bg-gray-100 py-5 px-8 text-2xl font-extrabold w-full max-w-screen-sm owHidden p-10 rounded-xl ring-8 ring-white ring-opacity-40">

<div class="title flex place-content-start"> 
  
  <a href="/setup/rooms" class="text-grayBlue-700  text-xl font-bold selectNONE ml-4"
  aria-current={$page.url.pathname.startsWith('/setup/rooms') ? 'page' : undefined}>Pokoje</a>
  <a href="/setup/devices" class="text-grayBlue-700  text-xl font-bold selectNONE ml-4"
  aria-current={$page.url.pathname.startsWith('/setup/devices') ? 'page' : undefined}
  >Urządzenia</a>

</div>

<div class="flex justify-between place-content-end">

    <button class="button_add p-1 bg-gray-200 hover:bg-gray-300 rounded-full ml-4"
     
      on:click={() => handle_AddModal_close()}>
      <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 0 24 24" width="24px" fill="#000000" class="button_"><path d="M0 0h24v24H0V0z" fill="none" class="s-2bKfpBUBwHrG"></path><path d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12 19 6.41z" class="button|"></path></svg>
    </button>

  <!-- <button class="button_add p-1 bg-gray-200 hover:bg-gray-300 rounded-full ml-4">
   <img src="../images/reload.svg" width="28px" alt="reload"/>

  </button> -->

</div>

</div>


    <div transition:fade={{ delay: 200, duration: 300 }}  class="mod_content w-full max-w-screen-sm owHidden bg-white p-10 rounded-xl ring-8 ring-white ring-opacity-20"  >
      
      <div class="flex justify-between">
           
              <!-- /////// -->

              <table class="min-w-full  border-collapse block md:table rounded-lg">
                <thead class="block md:table-header-group">
                  <tr
                    class="border border-grey-500 md:border-none block md:table-row absolute -top-full md:top-auto -left-full md:left-auto  md:relative "
                  >
                    <th
                      class="bg-gray-200 noSelect p-2 text-gray-500 font-bold md:border md:border-grey-500 text-left block md:table-cell"
                      >Nazwa</th
                    >
                    <th
                      class="bg-gray-200 noSelect p-2 text-gray-500 font-bold md:border md:border-grey-500 text-right block md:table-cell"
                      >Przypisany czujnik</th
                    >
                    <!-- <th class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell">Email Address</th>
            <th class="bg-gray-600 p-2 text-white font-bold md:border md:border-grey-500 text-left block md:table-cell">Mobile</th> -->
                    <th
                      class="bg-gray-200 p-2 text-gray-500 font-bold md:border md:border-grey-500 text-left block md:table-cell"
                    />
                  </tr>
                </thead>
      
                {#each $rooms as room}
                  <tbody class="block md:table-row-group">
                    <tr class="bg-white border border-grey-500 md:border-none block md:table-row rounded-md">
                      <td class="p-2 md:border md:border-grey-500 sm:text-lg text-start block md:table-cell">
                        {room.name} 
                        </td> 
                      <td class="p-2 md:border md:border-grey-500 text-right block md:table-cell">
                        <span class="inline-block w-1/3 md:hidden font-bold text-slate-700 sm:text-start" />



                        
                         
                            {#each $devices as device}
                              {#if device.mac==room.mac}
                                {device.name}<br><span class="text-sm font-extralight sm:text-end text-slate-500">({device.mac})</span>
                              {/if}
                            {/each}
                         
                         
                        
                        
                        </td>
                      <!-- <td class="p-2 md:border md:border-grey-500 text-left block md:table-cell"><span class="inline-block w-1/3 md:hidden font-bold"></span>mkoch@yahoo.com</td>
              <td class="p-2 md:border md:border-grey-500 text-left block md:table-cell"><span class="inline-block w-1/3 md:hidden font-bold"></span>582-400-3X36</td> -->
                      <td class="table-cell p-2 md:border md:border-grey-100"
                      >
                        <span class="inline-block w-1/3 md:hidden font-bold text-center"/>
      

                     


                        <button class="button_edit text-center button_add p-1 bg-gray-200 hover:bg-gray-300 rounded-full ml-4"
                        on:click={() => handle_EditModal_open(room.id)}
                        >
                          <img src="../images/setup-icon.svg" width="20px" alt="edycja"/>
                       
                         </button>

                         <button class="button_del p-1 bg-gray-200 hover:bg-gray-300 rounded-full ml-4"
                         on:click={() => handle_Confirm_close(room.id)}
                         on:click={() => currentConfirm=room.id}
                         on:click={() => currentConfirmName=room.name}
                         >
                        
                         
                         <svg xmlns="http://www.w3.org/2000/svg" height="20px" viewBox="0 0 24 24" width="20px" fill="#000000" class="button_"><path d="M0 0h24v24H0V0z" fill="none" class="s-2bKfpBUBwHrG"></path><path d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12 19 6.41z" class="button|"></path></svg>
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

<Modal title="potwierdz usuniecie" open={showConfirm} on:close={()=>handle_Confirm_close()}>
  <svelte:fragment slot="body">
    <p class="noSelect">Czy na pewno chcesz usunąć pokój:</p>

      <span class="noSelect text-bold-xl bg-gray-300 rounded-md text-white p20 text-center mt-10">   { currentConfirmName? currentConfirmName : ""} ?</span>
      
  
    </svelte:fragment>
  <svelte:fragment slot="footer">



  <button
    on:click={() => handle_Confirm_del(currentConfirm)}
    class="mr-1 rounded noSelect bg-white px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
    type="button">Tak</button>
  <button
    on:click={() => handle_Confirm_close()}
    class="mr-1 rounded noSelect bg-white px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
    type="button">Nie</button>

  
  </svelte:fragment>
</Modal> 



<Modal
  title="Edycja pokoju {curr_roomID}"
  open={showEditModal}
  on:close={() => handle_EditModal_close()}
  >
  <svelte:fragment slot="body">
    <label
      class="block noSelect uppercase text-blueGray-600 text-xs font-bold mb-2"
      for="grid-password"
    >
      Nazwa
    </label>
    <input
      type="text"
      class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150"
      value= {jsonRoomsData[curr_roomID].name} on:change={(e) => {jsonRoomsData[curr_roomID].name = e.target.value; console.log(jsonRoomsData[curr_roomID].name)}}
    />
     

    <br /><br />
    <label
      class="block noSelect uppercase text-blueGray-600 text-xs font-bold mb-2"
      for="grid-password"
    >
      Urządzenie
    </label>
    <select
      class="form-select appearance-none block w-full px-3 py-1.5 text-base font-normal text-gray-700 bg-white bg-clip-padding bg-no-repeat border border-solid border-gray-300 rounded transition ease-in-out
          m-0
          focus:text-gray-700 focus:bg-white focus:border-blue-600 focus:outline-none"
      aria-label=""
     on:change={(e)=>{jsonRoomsData[curr_roomID].mac=e.target.value;}}>


            {#each jsonDevicesData as device}
            {#if jsonRoomsData[curr_roomID].mac == device.mac}
            <option selected value={device.mac}>{device.mac} ({device.name})</option>
            {/if}

            {#if jsonRoomsData[curr_roomID].mac != device.mac}

            <option value={device.mac}>{device.mac} ({device.name}) </option>  
            {/if}
           
      {/each}   
      <option value=------->-------</option> 
  
    </select>
</svelte:fragment>

  <svelte:fragment slot="footer">
    <p />
    
    <button
      on:click={() => handle_EditModal_close()}
      class="mr-1 rounded noSelect bg-white px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
      type="button">Anuluj</button
    >
    <button
      on:click={() => handle_EditModal_save()}
      class="mr-1 rounded noSelect bg-red-600 px-4 py-2 text-xs font-bold uppercase text-white shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
      type="button">Zapisz</button
    >
  </svelte:fragment>
</Modal>

<Modal
  title="Dodaj pokój"
  open={showAddModal}
  on:close={() => handle_AddModal_close()}
>
  <svelte:fragment slot="body">

  <label
  class="block uppercase noSelect text-blueGray-600 text-xs font-bold mb-2"
  for="grid-password"
>
  Nazwa
</label>
<input
  type="text"
  class="border-0 px-3 py-3 placeholder-blueGray-300 text-blueGray-600 bg-white rounded text-sm shadow focus:outline-none focus:ring w-full ease-linear transition-all duration-150"
  value=""  on:change={(e) => {newRoom.name = e.target.value; console.log(newRoom.name)}}
/>
 

<br /><br />
<label
  class="block noSelect uppercase text-blueGray-600 text-xs font-bold mb-2"
  for="grid-password"
>
  Urządzenie
</label>
<select
  class="form-select appearance-none block w-full px-3 py-1.5 text-base font-normal text-gray-700 bg-white bg-clip-padding bg-no-repeat border border-solid border-gray-300 rounded transition ease-in-out
      m-0
      focus:text-gray-700 focus:bg-white focus:border-blue-600 focus:outline-none"
  aria-label=""
  bind:value={newRoom.mac}
  on:change={(e)=>{newRoom.mac=e.target.value;}}>
        {#each jsonDevicesData as device}
        <option value={device.mac}>{device.mac} ({device.name}) </option>  
        {/each}   
        <option value=------->-------</option> 
  </select>


  </svelte:fragment>
  
  <svelte:fragment slot="footer">

  <button
  on:click={() => handle_AddModal_close()}
  class="mr-1 rounded bg-white noSelect px-4 py-2 text-xs font-bold uppercase text-black shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
  type="button">Anuluj</button
>
<button
  on:click={() => handle_AddModal_add()}
  class="mr-1 rounded bg-red-600 noSelect px-4 py-2 text-xs font-bold uppercase text-white shadow outline-none transition-all duration-150 ease-linear hover:shadow-md focus:outline-none active:bg-pink-600"
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
    background-color: white;
    /* background-color: rgba(253, 119, 22, 1.000); */
    transform: rotate(45deg);
    animation: rotate 1s;
    transition: all ease 0.5s;
  } 
  .button_edit:hover img{
    transform: scale(1.2);

  }

  .button_del{
    opacity: 0.5;
    transform: rotate(90deg);
    transition: all ease 0.5s;
  }
  /* keyframes  */
  @keyframes rotate {
    0% {
      transform: rotate(0deg);
    }
    100% {
      transform: rotate(90deg);
    }
  }
  .button_del:hover{
    background-color: rgba(253, 119, 22, 1.000);
    transform: rotate(90deg);
    animation: rotate 1s;
    transition: all ease 0.5s;
  }




  @media screen and (max-width: 768px) {
    .head .title {
      display: none;
    }
  }
  

</style>


