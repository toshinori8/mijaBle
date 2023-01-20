<script>
  // import Counter from './Counter.svelte';
  // import welcome from '$lib/images/svelte-welcome.webp';
  // import welcome_fallback from '$lib/images/svelte-welcome.png';
  import { onMount } from "svelte";
  import { setContext } from "svelte";
  


  import Room from "./room.svelte";

  let jsonData;
    onMount(async () => {
        jsonData = await fetch('http://cleargrasstermostat.local/JSONdevices').then(res => res.json());
    });
    // @ts-ignore
    const jsonContext = createContext({ jsonData, updateJsonData });
    // @ts-ignore
    function updateJsonData(newJsonData){
        jsonData = newJsonData;
    }
    
   

   


</script>

<svelte:head>
  <title>CGG1 admin</title>
  <meta name="description" content="CGG1 admin panel" />
</svelte:head>

<section>
  {#each rooms as room}
    <Room roomData={room}  />
  {/each}
</section>

<style>
</style>
