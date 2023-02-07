<script>
    import { createEventDispatcher } from 'svelte'
    import { fade, fly } from 'svelte/transition';
    export let open = true
    export let title = 'Modal'
    const dispatch = createEventDispatcher()
    close = () => {
        dispatch('close')
    }
</script>


{#if open}

<div class="modal fixed top-0 left-0 z-50 flex h-full w-full items-center justify-center p-8 lg:p-0" in:fade="{{ y: 200, duration: 1200 }}" out:fade >
  <div class="modal-overlay fixed h-full w-full bg-gray-900 opacity-50" in:fade="{{ y: 200, duration: 1600 }}" out:fade>
   

    
  </div>
  <div class="z-50 mx-auto w-full overflow-y-auto rounded-lg bg-white shadow-xl lg:h-max lg:w-1/2"  in:fly="{{ y: 200, duration: 600 }}" out:fade>
    <div class="head flex justify-between bg-gray-100 py-5 px-8 text-2xl font-extrabold">
       <h6 class="text-blueGray-700 text-xl font-bold">Edit room </h6>
       <button class="p-2 bg-gray-200 hover:bg-gray-300 rounded-full ml-4" on:click={() => dispatch('close')}>
        <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 0 24 24" width="24px" fill="#000000"><path d="M0 0h24v24H0V0z" fill="none"/><path d="M19 6.41L17.59 5 12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12 19 6.41z"/></svg>
      </button>
    </div>
    <div class="content p-8">


      
<slot name="body"></slot>






    <hr class="border-b-1 border-blueGray-300 mt-6" />  
    </div>
    <div class="head flex justify-end bg-white py-5 px-8 text-2xl font-extrabold">
        <!-- <p></p>
         <button on:click={() => dispatch('close')} class="bg-white text-black active:bg-pink-600 font-bold uppercase text-xs px-4 py-2 rounded shadow hover:shadow-md outline-none focus:outline-none mr-1 ease-linear transition-all duration-150"  type="button">
          Anuluj
        </button>
         <button class="accent-color text-white active:bg-pink-600 font-bold uppercase text-xs px-4 py-2 rounded shadow hover:shadow-md outline-none focus:outline-none mr-1 ease-linear transition-all duration-150"  type="button">
          Zapisz
        </button> -->
        <slot name="footer"></slot>
    
      </div>
  </div>
</div>

{/if}


<style>
   
   .accent-color{
    background-color: rgba(253, 119, 22, 1.000);

   }


    </style>