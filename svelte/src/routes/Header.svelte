<script>
// @ts-nocheck

  import { page } from "$app/stores";
  import Update_icon from "../lib/components/update_icon.svelte";
  import Sensor_icon from "../lib/components/sensor_icon.svelte";
  
</script>

<header>
  <div class="corner">
    <!-- <a href="https://kit.svelte.dev">
			<img src={logo} alt="SvelteKit" />
		</a> -->
  </div>

  <nav>
    <svg viewBox="0 0 2 3" aria-hidden="true">
      <path d="M0,0 L1,2 C1.5,3 1.5,3 2,3 L2,0 Z" />
    </svg>
    <ul>
      <li aria-current={$page.url.pathname === "/" ? "page" : undefined}>
        <a href="/">
          
          <Sensor_icon />

        </a>
      </li>
      <li aria-current={$page.url.pathname.startsWith('/update') ? 'page' : undefined}>
        <a href="http://cleargrasstermostat.local/update">
          
          <Update_icon />

        </a>
      </li>
      <li aria-current={$page.url.pathname.startsWith('/setup') ? 'page' : undefined}>
        <a href="/setup/rooms">
          <img src="/images/setup-icon.svg" width="28px" alt="setup"/>
        </a>
      </li>
      <!-- <li aria-current={$page.url.pathname.startsWith('/sverdle') ? 'page' : undefined}>
				<a href="/sverdle">Sverdle</a>
			</li> -->
    </ul>
    <svg viewBox="0 0 2 3" aria-hidden="true">
      <path d="M0,0 L0,3 C0.5,3 0.5,3 1,2 L2,0 Z" />
    </svg>
  </nav>

  <div class="corner">
    <!-- <a href="https://github.com/sveltejs/kit">
			<img src={github} alt="GitHub" />
		</a> -->

    <svg
      id="loading_dot"
      class="loading_dot_on hidden"
      fill="rgba(252, 62, 0, 0.8)"
      width="800px"
      height="800px"
      viewBox="0 0 64 64"
      version="1.1"
      xmlns="http://www.w3.org/2000/svg"
      xmlns:xlink="http://www.w3.org/1999/xlink"
      xml:space="preserve"
      
      xmlns:serif="http://www.serif.com/"
      style="fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;"
    >
      <g transform="matrix(1,0,0,1,-576,-320)">
        <rect
          id="Icons"
          x="0"
          y="0"
          width="1280"
          height="800"
          style="fill:none;"
        />
        <g id="Icons1" serif:id="Icons">
          <g
            id="circle-filled"
            transform="matrix(1.70002,0,0,1.70002,-316.778,-246.387)"
          >
            <circle cx="543.992" cy="352" r="14.13" />
          </g>
        </g>
      </g>
    </svg>
  </div>
</header>

<style>
  header {
    display: flex;
    justify-content: space-between;
  }

  .corner {
    width: 3em;
    height: 3em;
  }

  /* .corner a {
    display: flex;
    align-items: center;
    justify-content: center;
    width: 100%;
    height: 100%;
  } */

  /* .corner img {
    width: 2em;
    height: 2em;
    object-fit: contain;
  } */

  nav {
    display: flex;
    justify-content: center;
    --background: rgba(255, 255, 255, 0.7);
  }

  svg {
    width: 2em;
    height: 3em;
    display: block;
  }
  svg.loading_dot_on {
    animation: loading_dot_on 0.5s infinite;
  }

  svg.loading_dot_on.hidden {
	display: none;
  }
  @keyframes loading_dot_on {
    0% {
      fill: rgba(253, 119, 22, 1);
    }
    50% {
      fill: rgba(253, 119, 22, 0);
    }
    100% {
      fill: rgba(253, 119, 22, 1);
    }
  }

  path {
    fill: var(--background);
  }

  ul {
    position: relative;
    padding: 0;
    margin: 0;
    height: 3em;
    display: flex;
    justify-content: center;
    align-items: center;
    list-style: none;
    background: var(--background);
    background-size: contain;
  }

  li {
    position: relative;
    height: 100%;
  }

  li[aria-current="page"]::before {
    --size: 6px;
    content: "";
    width: 0;
    height: 0;
    position: absolute;
    top: 0;
    left: calc(50% - var(--size));
    border: var(--size) solid transparent;
    border-top: var(--size) solid var(--color-theme-1);
  }

  nav a {
    display: flex;
    height: 100%;
    align-items: center;
    padding: 0 0.5rem;
    color: var(--color-text);
    font-weight: 700;
    font-size: 0.8rem;
    text-transform: uppercase;
    letter-spacing: 0.1em;
    text-decoration: none;
    transition: color 0.2s linear;
  }

  a:hover {
    color: var(--color-theme-1);
  }
</style>
