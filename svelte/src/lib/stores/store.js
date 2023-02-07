import { writable } from "svelte/store";
// import { setContext } from "svelte";
    
    // export const roomsStore = writable({});

    // let rooms = writable({});
    // $: roomsStore.set(rooms);
    
  
    
  let roomsStore = writable(rooms);
  $: roomsStore.set(rooms);
  setContext("rooms", roomsStore);
    
    // function updateRoomStore(dataIn) {
    //     
    // roomsStore.set(dataIn);
    // dataIn = dataIn;
    // }

   export { rooms , displayRooms};

