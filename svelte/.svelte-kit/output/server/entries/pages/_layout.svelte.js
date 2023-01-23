import { c as create_ssr_component, b as subscribe, d as add_attribute, v as validate_component } from "../../chunks/index.js";
import { p as page } from "../../chunks/stores.js";
/* empty css                                                  */const css$1 = {
  code: 'header.svelte-z86p5u.svelte-z86p5u{display:flex;justify-content:space-between}.corner.svelte-z86p5u.svelte-z86p5u{width:3em;height:3em}nav.svelte-z86p5u.svelte-z86p5u{display:flex;justify-content:center;--background:rgba(255, 255, 255, 0.7)}svg.svelte-z86p5u.svelte-z86p5u{width:2em;height:3em;display:block}svg.loading_dot_on.svelte-z86p5u.svelte-z86p5u{animation:svelte-z86p5u-loading_dot_on 0.5s infinite}svg.loading_dot_on.hidden.svelte-z86p5u.svelte-z86p5u{display:none}@keyframes svelte-z86p5u-loading_dot_on{0%{fill:rgba(253, 119, 22, 1)}50%{fill:rgba(253, 119, 22, 0)}100%{fill:rgba(253, 119, 22, 1)}}path.svelte-z86p5u.svelte-z86p5u{fill:var(--background)}ul.svelte-z86p5u.svelte-z86p5u{position:relative;padding:0;margin:0;height:3em;display:flex;justify-content:center;align-items:center;list-style:none;background:var(--background);background-size:contain}li.svelte-z86p5u.svelte-z86p5u{position:relative;height:100%}li[aria-current="page"].svelte-z86p5u.svelte-z86p5u::before{--size:6px;content:"";width:0;height:0;position:absolute;top:0;left:calc(50% - var(--size));border:var(--size) solid transparent;border-top:var(--size) solid var(--color-theme-1)}nav.svelte-z86p5u a.svelte-z86p5u{display:flex;height:100%;align-items:center;padding:0 0.5rem;color:var(--color-text);font-weight:700;font-size:0.8rem;text-transform:uppercase;letter-spacing:0.1em;text-decoration:none;transition:color 0.2s linear}a.svelte-z86p5u.svelte-z86p5u:hover{color:var(--color-theme-1)}',
  map: null
};
const Header = create_ssr_component(($$result, $$props, $$bindings, slots) => {
  let $page, $$unsubscribe_page;
  $$unsubscribe_page = subscribe(page, (value) => $page = value);
  $$result.css.add(css$1);
  $$unsubscribe_page();
  return `<header class="${"svelte-z86p5u"}"><div class="${"corner svelte-z86p5u"}"></div>

  <nav class="${"svelte-z86p5u"}"><svg viewBox="${"0 0 2 3"}" aria-hidden="${"true"}" class="${"svelte-z86p5u"}"><path d="${"M0,0 L1,2 C1.5,3 1.5,3 2,3 L2,0 Z"}" class="${"svelte-z86p5u"}"></path></svg>
    <ul class="${"svelte-z86p5u"}"><li${add_attribute("aria-current", $page.url.pathname === "/" ? "page" : void 0, 0)} class="${"svelte-z86p5u"}"><a href="${"/"}" class="${"svelte-z86p5u"}">Pokoje</a></li>
      <li${add_attribute("aria-current", $page.url.pathname === "/config" ? "page" : void 0, 0)} class="${"svelte-z86p5u"}"></li>
      </ul>
    <svg viewBox="${"0 0 2 3"}" aria-hidden="${"true"}" class="${"svelte-z86p5u"}"><path d="${"M0,0 L0,3 C0.5,3 0.5,3 1,2 L2,0 Z"}" class="${"svelte-z86p5u"}"></path></svg></nav>

  <div class="${"corner svelte-z86p5u"}">

    <svg id="${"loading_dot"}" class="${"loading_dot_on hidden svelte-z86p5u"}" fill="${"rgba(252, 62, 0, 1.000)"}" width="${"800px"}" height="${"800px"}" viewBox="${"0 0 64 64"}" version="${"1.1"}" xmlns="${"http://www.w3.org/2000/svg"}" xmlns:xlink="${"http://www.w3.org/1999/xlink"}" xml:space="${"preserve"}" xmlns:serif="${"http://www.serif.com/"}" style="${"fill-rule:evenodd;clip-rule:evenodd;stroke-linejoin:round;stroke-miterlimit:2;"}"><g transform="${"matrix(1,0,0,1,-576,-320)"}"><rect id="${"Icons"}" x="${"0"}" y="${"0"}" width="${"1280"}" height="${"800"}" style="${"fill:none;"}"></rect><g id="${"Icons1"}" serif:id="${"Icons"}"><g id="${"circle-filled"}" transform="${"matrix(1.70002,0,0,1.70002,-316.778,-246.387)"}"><circle cx="${"543.992"}" cy="${"352"}" r="${"14.13"}"></circle></g></g></g></svg></div>
</header>`;
});
const css = {
  code: ".app.svelte-1secgvi{display:flex;flex-direction:column;min-height:100vh}main.svelte-1secgvi{flex:1;display:flex;flex-direction:column;padding:1rem;width:100%;max-width:64rem;margin:0 auto;box-sizing:border-box}footer.svelte-1secgvi{display:flex;flex-direction:column;justify-content:center;align-items:center;padding:12px}@media(min-width: 480px){footer.svelte-1secgvi{padding:12px 0}}",
  map: null
};
const Layout = create_ssr_component(($$result, $$props, $$bindings, slots) => {
  $$result.css.add(css);
  return `<div class="${"app svelte-1secgvi"}">${validate_component(Header, "Header").$$render($$result, {}, {}, {})}

	<main class="${"svelte-1secgvi"}">${slots.default ? slots.default({}) : ``}</main>

	<footer class="${"svelte-1secgvi"}"></footer>
</div>`;
});
export {
  Layout as default
};
