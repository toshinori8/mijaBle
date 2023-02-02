<script>
  import { onMount } from "svelte";

     export let level =100;
    
    // define random number 
    /**
     * @param {number} min
     * @param {number} max
     */
    function random(min, max) {
      return Math.floor(Math.random() * (max - min + 1) + min);
    } 
    let hash = random(20, 100);

   

     function levelColor(){
        let pos = 0;
        let color = ""
        if (level <= 1) {
          color= "fil_red";
          pos= 5;
        } else if (level <= 5) {
          color= "fil_gray";
          pos= 1;
        } else if (level <= 10) {
          color= "fil_orange";
          pos= 2;
        } else if (level <= 30) {
          color= "fil_orange";
          pos= 3;
        } else if (level <= 60) {
          color= "fil_green";
          pos= 3;
        } else if (level <= 80) {
          pos= 4;
          color= "fil_green";
        } else if (level <= 100) {
          pos= 5;
          color= "fil_green";
        }



        let meters= document.getElementsByClassName("meter " + hash);
        for (let i = 0; i < pos; i++) {
          
          meters[i].classList.add(color);
        } 
        
      }
     


  
  onMount(() => {
   

   

  });
</script>

<style>
  .battery_level
    {
    opacity:0.4 ;
    width: 30px;
    height: 10px;
    position: absolute;
    margin-top: 22px;
    margin-left: 27px;
   /* border: 0.1px solid black  ; */
    /* transform: scale(8.5); */
  }
  .meter{
    fill: none;
  }

  .battery_level .label{
    transition: all 0.5s ease;
    width: 120px;
    height: 120px;
    opacity: 1;
    font-size: 1.4em;
    position: relative;
    z-index: 900000;
    float: right;
  display: none;
  }

  .battery_level:hover .label{
    
 

  }
  .battery_level:hover .battery_level_svg{
    
    opacity: 0;
    display: none;
  }

</style>

<div class="battery_level">

  <span class="label">{level}%</span>

  
<!-- <p>{level}</p> -->
{#if level >= 0 }
    <!-- ts ignore -->
  
<svg   use:levelColor
  class="battery_level_svg"
  data-name="battery_level"
  xmlns="http://www.w3.org/2000/svg"
  viewBox="0 0 167.64 101.93"
  ><defs
    ><style>
      

      .fil_gray{
        fill: #838484 !important;
      }
      .fil_orange{
        fill: #EF7C09 !important;
      }
      .fil_yellow{
        fill: #F9DE00!important;
      }
      .fil_green{
        fill: #43A32F!important;
      }
      .fil_red{
        fill: #E60000!important;
        animation: blinker 0.7s linear infinite;
      }
      @keyframes blinker {
        50% {
          opacity: 0;
        }
      }
      .fil_none{
        fill: none !important; 
        opacity: 0.3 !important; 
      }
      .meter{
        opacity: 0.9; 
      }
      .border{

          opacity: 0.2;
      

      }


    </style></defs
  >



  
  <path
    
    class="fil_none"
    d="M581.25,374q-31,0-62,0c-9.24,0-13.56-4.2-13.57-13.34q0-37.74.06-75.49C505.77,276.17,510,272,519,272q62.25,0,124.49,0c8.71,0,12.9,4.17,12.9,12.79q0,38,0,76c0,9-4.15,13.11-13.11,13.12Q612.25,374,581.25,374Zm.5-91.84c-20.15,0-40.3.16-60.44-.11-4.59-.06-5.49,1.43-5.45,5.65q.3,35.46,0,70.93c0,4.07,1,5.37,5.23,5.35q60.18-.24,120.37,0c4.16,0,4.86-1.44,4.84-5.15q-.23-35.71,0-71.43c0-4.06-.92-5.38-5.18-5.33C621.38,282.26,601.56,282.13,581.75,282.11Z"
    transform="translate(-505.68 -272.03)"
  /><path
  
  class="fil_none"
    d="M659.52,322.87c0-4-.07-8,0-12,.09-4.69,2.39-6.32,6.78-4.61a10.13,10.13,0,0,1,6.84,9.52c.21,4.81.24,9.65,0,14.46a10,10,0,0,1-7.45,9.53c-4.07,1.27-6-.16-6.13-4.48C659.42,331.18,659.52,327,659.52,322.87Z"
    transform="translate(-505.68 -272.03)"
  /><path
  class="meter {hash}"
  d="M540.29,323.51c0,9.66-.11,19.32.06,29,.07,4.11-1.63,5.68-5.64,5.6-15.29-.31-13.81,2-13.85-13.36,0-16.82.12-33.63-.06-50.44,0-4.78,1.67-6.47,6.44-6.39,14.68.24,13-1.36,13.06,13.15,0,3.83,0,7.66,0,11.48v11Z"
  transform="translate(-505.68 -272.03)"
   
  /><path
  class="meter {hash}"
  d="M565.27,323.14c0,9.82-.09,19.65,0,29.47.05,3.81-1.47,5.5-5.28,5.45-15.8-.22-14.17,2-14.24-13.5-.08-16.65,0-33.3,0-49.95,0-5.95.71-6.64,6.55-6.63,14.35,0,12.92-1.5,12.94,12.68Q565.29,311.9,565.27,323.14Z"
  transform="translate(-505.68 -272.03)"
  /><path
  class="meter {hash}"
    d="M570.64,322.53c0-9.49.07-19,0-28.48-.05-4.42,1.66-6.22,6.24-6.15,15.21.23,13.37-1.4,13.41,13.3,0,16.82-.12,33.65.09,50.47,0,4.78-1.63,6.57-6.4,6.43-13.91-.4-13.24,2.36-13.28-13.59C570.63,337.18,570.64,329.86,570.64,322.53Z"
    transform="translate(-505.68 -272.03)"
  /><path
  class="meter {hash}"
   
  d="M615.25,323.56c0,9.64-.09,19.28,0,28.92.05,4-1.54,5.67-5.62,5.6-14.8-.28-14.16,2.11-14.1-13.74.06-16.29,0-32.58,0-48.86,0-6.92.57-7.46,7.24-7.5,13.72-.1,12.42-1.24,12.43,12.65Z"
  transform="translate(-505.68 -272.03)"
  /><path
  class="meter {hash}"
  d="M640.25,323.31c0,9.67-.1,19.33,0,29,.06,4-1.49,5.82-5.57,5.75-14.88-.26-14.28,2.24-14.24-13.66,0-16.5,0-33,0-49.49,0-6.18.82-7,6.86-7,13.8.14,12.94-1.77,12.88,12.86C640.22,308.31,640.24,315.81,640.25,323.31Z"
  transform="translate(-505.68 -272.03)"
  /></svg
>



{/if}

</div>