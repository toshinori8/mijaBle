<script>
    /** @type {import('./$types').PageData} */
    export let data;
    import { getContext, onMount } from "svelte";
    import { setContext } from "svelte/types/runtime/internal/lifecycle";
    import childf from './childf.svelte';

    let datta;
    datta = getContext("jsonRooms");
    setContext("jsonRooms", datta);

    

</script>


<div><p>diad</p> </div>
