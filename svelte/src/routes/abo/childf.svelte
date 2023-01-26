<script>
  // @ts-ignore

  const jq = window.$;

  export const xxx = (/** @type {any} */ room) => {
    console.log(room);
  };

 /**
     * @type {(arg0: any) => (arg0: any) => void}
     */
 

  import { getContext, onMount } from "svelte";

  let contetnt = getContext("jsonRooms");



  onMount(() => {
  



  });
</script>


<!-- <p>{} </p> -->
{#each contetnt as ittem}
{ittem.name}
{/each}


