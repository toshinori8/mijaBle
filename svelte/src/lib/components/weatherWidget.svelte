<script lang="ts">
  // @ts-ignore
    import Carousel from 'svelte-carousel';
    // @ts-ignore

    import { onMount } from "svelte";
    import { owapikey, lat, lon } from "../components/owapikey";
    import IconHumidity from "../components/iconHumidity.svelte";
    import { crossfade, fade,  } from "svelte/transition";
 

    import dayjs from 'dayjs';
    import localePL from "dayjs/locale/pl";
    // import calendar from "dayjs/plugin/calendar";
    // console.log(localePL);
    dayjs.locale('pl') // use locale globally
    // dayjs().locale('pl').format() // use locale in a specific instance
 

    let data: {
        daily: any; current: { temp: any; feels_like: number; humidity: number; weather: { icon: any; }[]; }; hourly: any; 
} | null;
   
    
function random(min: number, max: number) {
    return Math.floor(Math.random() * (max - min + 1) + min);
  }
  
    

    let convertTimestamp = (timestamp: any, type: string) =>
    {
  
      let date = new Date(timestamp * 1000);
      let hours = date.getHours();
      let minutes = date.getMinutes();
      let seconds = date.getSeconds();
      let day = date.getDate();
      let month = date.getMonth() + 1;
      let year = date.getFullYear();
  
      if (type == "date")
      {
          return `${day}.${month}.${year}`;
      }
      else if (type == "time"){
          return `${hours}:${minutes}:${seconds}`;
      }
      else if (type == "day"){
          return day;
      }
      else if (type == "month"){
          return month;
      }
      else if (type == "year"){
          return year;
      }
      else if (type == "hours"){
          return hours;
      }
      else if (type == "minutes"){
          return minutes;
      }
      else if (type == "seconds"){
          return seconds;
      }else return false;
    }

    
    // @ts-ignore
    let srcOfImage = "";
  
    // get forecast
    function getForecast() {
      fetch(
      // new API 
      "https://api.openweathermap.org/data/3.0/onecall?lat="+lat+"&lon="+lon+"&appid="+owapikey+"&units=metric"
      
          )
        .then((response) => response.json())
        .then((data_) => {
         
          // @ts-ignore
          if (data_.current.temp= -0){data_.current.temp=0}
          data = data_;
          srcOfImage="http://openweathermap.org/img/wn/"+data_.current.weather[0].icon+"@2x.png";
        });
    }
  
    let x = 0;
    let list: HTMLDivElement;

    
  

    /// async function

    let setDragable = async () => {

     // @ts-ignore
     const jqs = window.$;
    


      // @ts-ignore
      jqs(".dragable-container").draggable({
        cursor: "move",
        containment: "parent-window",
        stop: function () {
          // @ts-ignore
          if (jqs(".child-window").position().left < 1) jqs(".child-window").css("left", "720px");
        },
      });
          };

 





    onMount(async () => {
      getForecast();


      // get forecast 60 minutes
      setInterval(() => getForecast(), 1000 * 60 * 60);
  
  
      setDragable();
   

    

    });
  

    function jqs(arg0: string) {
        throw new Error("Function not implemented.");
    }
</script>





  {#if data != null}
    <div class="weather_widget weather_section flex flex-col items-center justify-center min-h-screen text-gray-700 p-10 " >
      <!-- Component Start -->
      <!-- svelte-ignore missing-declaration -->
      <div transition:fade={{ delay: 200, duration: 300 }}
        class="w-full max-w-screen-sm owHidden bg-white p-10 rounded-xl ring-8 ring-white ring-opacity-40"
      >
        <div class="flex justify-between">
          <div class="flex flex-col">
            <span class="text-6xl font-bold lcd"
              >{data.current.temp}<span class="text-3xl">°C</span></span
            >
            <span class="font-light mt-1 text-gray-500">Odczuwalna : {data.current.feels_like.toFixed(1)}</span>
            <!-- <span class="font-semibold mt-1 text-gray-500">{data.name}</span> -->
          </div>
          <div class="flex flex-col">
            <span class="text-6xl font-bold lcd"
              >{data.current.humidity.toFixed(0)}<span class="text-3xl"
                ><span class="upperTop">o</span> /o</span></span>
           
            <!-- <span class="font-semibold mt-1 text-gray-500">{data.name}</span> -->
          </div>
          <div class="h-24 w-24 ">
              <!-- {data.current.weather[0].icon} -->
            
            
            <img  src="{srcOfImage}" alt=""/>

            
          </div>
        </div>


        <br>
        <br>
       
<hr>
        <Carousel
        particlesToShow={8}
        particlesToScroll={8}
        dots={true}
  arrows={false}
  infinite={false}
      >
      {#each data.hourly as nexthour}
         
      
      <div class="flex time_items flex-col items-center noSelect" style="margin-right:30px; padding-bottom: 20px;">
        <span class="font-semibold mt-1 text-sm lcd noSelect time_dis">{convertTimestamp(nexthour.dt,"hours")}:{convertTimestamp(nexthour.dt,"minutes")} </span>
        <!-- <span class="text-xs font-semibold text-gray-400 lcd noSelect">PM</span> -->
        
        <img alt="" src="http://openweathermap.org/img/wn/{nexthour.weather[0].icon}@2x.png" width="120px" class="noSelect" style="pointer-events: none;"/>	
        <span class="font-semibold text-lg lcd noSelect">{nexthour.temp.toFixed(1)}°C</span>
        <span class="text-sm lcd noSelect">{nexthour.feels_like}</span>
        
    </div>



        {/each}
      </Carousel>
<hr>


<!-- 
          <div class="dragable-container"  style="overflow-x: auto;">

            <div class="flex justify-between mt-12 parent-w">
                 
              
                  {#each data.hourly as nexthour}
                    
                      
                          <div class="flex time_items flex-col items-center" style="margin-right:30px; padding-bottom: 20px;">
                              <span class="font-semibold text-lg lcd">{nexthour.temp.toFixed(1)}°C</span>
                              <img alt="" src="http://openweathermap.org/img/wn/{nexthour.weather[0].icon}@2x.png" width="120px" />	
                              <span class="font-semibold mt-1 text-sm lcd">{convertTimestamp(nexthour.dt,"hours")}:{convertTimestamp(nexthour.dt,"minutes")}</span>
                              <span class="text-xs font-semibold text-gray-400 lcd">PM</span>
                          </div>
                      
                  {/each }
           
              
          </div> -->

          <!-- </div> -->
      </div>
  


      <!-- svelte-ignore missing-declaration -->

      <div transition:fade={{ delay: 500, duration: 500 }} class="flex flex-col space-y-6 w-full max-w-screen-sm bg-white p-10 mt-10 rounded-xl ring-8 ring-white ring-opacity-40">
      
        <div class="scrollHandle">

        {#each data.daily as day}
        
        


        <div class="flex justify-between items-center">
          <span class="font-semibold text-lg w-1/4 firstLetterUp">  
          
            {dayjs.unix(day.dt).format('dddd')}<br>
            {dayjs.unix(day.dt).locale('pl').format('D')} {dayjs.unix(day.dt).format('MMM')}
          </span>
            <!-- <br> Fri, 22 Jan</span> -->
          <div class="flex items-center justify-end w-1/4 pr-10">
            
            <span class="font-semibold humidityForecast">
                  {day.humidity}%

            </span>
          
              <IconHumidity></IconHumidity>

          </div>
          <img alt="" src="http://openweathermap.org/img/wn/{day.weather[0].icon}@2x.png" width="" />	

            <!-- {day.weather[0].icon} -->

          <span class="font-semibold text-lg w-1/4 text-right temp_primary">{day.temp.day}°</span>
          <span class="under">

          <span class="fontTemp w-1/2 text-right temp_secondary"><span class="labelTemp">min </span>{day.temp.min}°</span>
          <span class="fontTemp w-1/2 text-right temp_secondary"><span class="labelTemp">max </span>{day.temp.max}°</span>
        </span>
          
        
        </div>



        {/each}
      
        </div>


    
       
      </div>
      <!-- Component End  -->
    </div>
  {/if}
  
  <style>
  
    .time_dis{

      background-color: rgba(230, 230, 230, 1.000);
    width: 47px;
    height: 21px;
    text-align: center;
    color: gray;
    border-radius: 40px;

    }
    .noSelect{

        user-select: none;
        

    }

    .weather_widget{
      padding-top: 220px;

    }
    .weather_widget .lcd {
      font-family: "lcd", Fallback, sans-serif !important;
    }
    .upperTop {
      position: absolute;
      margin-top: 8px;
      margin-left: 7px;
    }
    .firstLetterUp::first-letter{
     /* uppercase */
     text-transform: capitalize;
    }
    .labelTemp{

      font-size: 10px !important;
      /* position: absolute; */
      margin-right: 4px;

    }
    .temp_primary{
      font-size: 1.4em;
    
    }
      .temp_secondary{
     
    display: flex;
    /* width: 27px; */
    font-size: 0.7em !important;
       
    margin-left: 2px;
       
     
    }
    .under{

      border-left: 1px solid gray;
    margin-left: 5px;
    padding-left: 5px;
          
    }

    .owHidden{
      overflow: hidden;
    }





  </style>