<script>
  import { createEventDispatcher } from "svelte";
  import { fade } from "svelte/transition";
  export let open = true;
  export let title = "Modal";
  const dispatch = createEventDispatcher();
</script>

{#if open}

  <div class="modal-overlay fixed h-full w-full bg-white opacity-90"
  in:fade={{  duration: 300 }}
  out:fade >
  </div>
  <div class="modal fixed top-0 left-0 z-50 flex h-full w-full items-center justify-center p-8 lg:p-0"
    in:fade={{ y: 200, duration: 100 }}
    out:fade >
    
    <div  class="flex flex-col items-center justify-center min-h-screen text-gray-700 p-10 "  >
      <div
      class="head flex justify-between bg-gray-100 py-5 px-8 text-2xl font-extrabold w-full max-w-screen-sm owHidden p-10 rounded-xl ring-8 ring-white ring-opacity-40">

    <h6 class="text-grayBlue-700  text-xl font-bold selectNONE">{title}</h6>
     
      <button class="p-2 bg-gray-200 hover:bg-gray-300 rounded-full ml-4"
        on:click={() => dispatch("close")}>
        
        <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 0 24 24" width="24px" fill="#000000">
        <path d="M0 0h24v24H0V0z" fill="none" /><path
            d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12 19 6.41z"/>
        </svg>
      
      </button>
    </div>
    

    <div transition:fade={{ delay: 200, duration: 300 }}  class="mod_content w-full max-w-screen-sm owHidden bg-white p-10 rounded-xl ring-8 ring-white ring-opacity-20"  >
        <div class="flex justify-between">
          <div class="flex flex-col">
            <slot name="body" />
          </div>    
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

<style>

  .head{
    border-radius: 10px 10px 0px 0px;
    box-shadow: none;
  }
  .head{
  position: relative;
    width: 96%;
    top: 13px;
    opacity: 0.7;
    padding-top: 21px;
}
.mod_content{

  z-index: 100;
}
  .modal-overlay {
    background-color: rgba(220,220,220,0.6);
    opacity: 0.99;
    -webkit-backdrop-filter: blur(10px);
    backdrop-filter: blur(10px);
    /* filter:blur(4px);
    -o-filter:blur(4px);
    -ms-filter:blur(4px);
    -moz-filter:blur(4px);
    -webkit-filter:blur(4px); */

      }

      .selectNONE{
          user-select: none;


      }
</style>
