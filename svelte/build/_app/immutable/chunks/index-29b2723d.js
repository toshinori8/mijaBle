function m(){}function I(t,e){for(const n in e)t[n]=e[n];return t}function J(t){return!!t&&(typeof t=="object"||typeof t=="function")&&typeof t.then=="function"}function z(t){return t()}function M(){return Object.create(null)}function x(t){t.forEach(z)}function B(t){return typeof t=="function"}function K(t,e){return t!=t?e==e:t!==e||t&&typeof t=="object"||typeof t=="function"}function Q(t){return Object.keys(t).length===0}function R(t,...e){if(t==null)return m;const n=t.subscribe(...e);return n.unsubscribe?()=>n.unsubscribe():n}function dt(t,e,n){t.$$.on_destroy.push(R(e,n))}function _t(t,e,n,r){if(t){const i=H(t,e,n,r);return t[0](i)}}function H(t,e,n,r){return t[1]&&r?I(n.ctx.slice(),t[1](r(e))):n.ctx}function ht(t,e,n,r){if(t[2]&&r){const i=t[2](r(n));if(e.dirty===void 0)return i;if(typeof i=="object"){const o=[],l=Math.max(e.dirty.length,i.length);for(let u=0;u<l;u+=1)o[u]=e.dirty[u]|i[u];return o}return e.dirty|i}return e.dirty}function mt(t,e,n,r,i,o){if(i){const l=H(e,n,r,o);t.p(l,i)}}function pt(t){if(t.ctx.length>32){const e=[],n=t.ctx.length/32;for(let r=0;r<n;r++)e[r]=-1;return e}return-1}let E=!1;function W(){E=!0}function U(){E=!1}function V(t,e,n,r){for(;t<e;){const i=t+(e-t>>1);n(i)<=r?t=i+1:e=i}return t}function X(t){if(t.hydrate_init)return;t.hydrate_init=!0;let e=t.childNodes;if(t.nodeName==="HEAD"){const s=[];for(let c=0;c<e.length;c++){const f=e[c];f.claim_order!==void 0&&s.push(f)}e=s}const n=new Int32Array(e.length+1),r=new Int32Array(e.length);n[0]=-1;let i=0;for(let s=0;s<e.length;s++){const c=e[s].claim_order,f=(i>0&&e[n[i]].claim_order<=c?i+1:V(1,i,_=>e[n[_]].claim_order,c))-1;r[s]=n[f]+1;const a=f+1;n[a]=s,i=Math.max(a,i)}const o=[],l=[];let u=e.length-1;for(let s=n[i]+1;s!=0;s=r[s-1]){for(o.push(e[s-1]);u>=s;u--)l.push(e[u]);u--}for(;u>=0;u--)l.push(e[u]);o.reverse(),l.sort((s,c)=>s.claim_order-c.claim_order);for(let s=0,c=0;s<l.length;s++){for(;c<o.length&&l[s].claim_order>=o[c].claim_order;)c++;const f=c<o.length?o[c]:null;t.insertBefore(l[s],f)}}function Y(t,e){if(E){for(X(t),(t.actual_end_child===void 0||t.actual_end_child!==null&&t.actual_end_child.parentNode!==t)&&(t.actual_end_child=t.firstChild);t.actual_end_child!==null&&t.actual_end_child.claim_order===void 0;)t.actual_end_child=t.actual_end_child.nextSibling;e!==t.actual_end_child?(e.claim_order!==void 0||e.parentNode!==t)&&t.insertBefore(e,t.actual_end_child):t.actual_end_child=e.nextSibling}else(e.parentNode!==t||e.nextSibling!==null)&&t.appendChild(e)}function bt(t,e,n){E&&!n?Y(t,e):(e.parentNode!==t||e.nextSibling!=n)&&t.insertBefore(e,n||null)}function Z(t){t.parentNode&&t.parentNode.removeChild(t)}function yt(t,e){for(let n=0;n<t.length;n+=1)t[n]&&t[n].d(e)}function tt(t){return document.createElement(t)}function et(t){return document.createElementNS("http://www.w3.org/2000/svg",t)}function A(t){return document.createTextNode(t)}function gt(){return A(" ")}function xt(){return A("")}function wt(t,e,n,r){return t.addEventListener(e,n,r),()=>t.removeEventListener(e,n,r)}function $t(t,e,n){n==null?t.removeAttribute(e):t.getAttribute(e)!==n&&t.setAttribute(e,n)}function nt(t){return Array.from(t.childNodes)}function rt(t){t.claim_info===void 0&&(t.claim_info={last_index:0,total_claimed:0})}function L(t,e,n,r,i=!1){rt(t);const o=(()=>{for(let l=t.claim_info.last_index;l<t.length;l++){const u=t[l];if(e(u)){const s=n(u);return s===void 0?t.splice(l,1):t[l]=s,i||(t.claim_info.last_index=l),u}}for(let l=t.claim_info.last_index-1;l>=0;l--){const u=t[l];if(e(u)){const s=n(u);return s===void 0?t.splice(l,1):t[l]=s,i?s===void 0&&t.claim_info.last_index--:t.claim_info.last_index=l,u}}return r()})();return o.claim_order=t.claim_info.total_claimed,t.claim_info.total_claimed+=1,o}function O(t,e,n,r){return L(t,i=>i.nodeName===e,i=>{const o=[];for(let l=0;l<i.attributes.length;l++){const u=i.attributes[l];n[u.name]||o.push(u.name)}o.forEach(l=>i.removeAttribute(l))},()=>r(e))}function Et(t,e,n){return O(t,e,n,tt)}function kt(t,e,n){return O(t,e,n,et)}function it(t,e){return L(t,n=>n.nodeType===3,n=>{const r=""+e;if(n.data.startsWith(r)){if(n.data.length!==r.length)return n.splitText(r.length)}else n.data=r},()=>A(e),!0)}function Nt(t){return it(t," ")}function vt(t,e){e=""+e,t.wholeText!==e&&(t.data=e)}function At(t,e,n,r){n===null?t.style.removeProperty(e):t.style.setProperty(e,n,r?"important":"")}function St(t,e){const n=[];let r=0;for(const i of e.childNodes)if(i.nodeType===8){const o=i.textContent.trim();o===`HEAD_${t}_END`?(r-=1,n.push(i)):o===`HEAD_${t}_START`&&(r+=1,n.push(i))}else r>0&&n.push(i);return n}function jt(t,e){return new t(e)}let g;function d(t){g=t}function S(){if(!g)throw new Error("Function called outside component initialization");return g}function Tt(t){S().$$.on_mount.push(t)}function Ct(t){S().$$.after_update.push(t)}const y=[],D=[],w=[],q=[],P=Promise.resolve();let N=!1;function F(){N||(N=!0,P.then(j))}function Mt(){return F(),P}function v(t){w.push(t)}const k=new Set;let p=0;function j(){if(p!==0)return;const t=g;do{try{for(;p<y.length;){const e=y[p];p++,d(e),ct(e.$$)}}catch(e){throw y.length=0,p=0,e}for(d(null),y.length=0,p=0;D.length;)D.pop()();for(let e=0;e<w.length;e+=1){const n=w[e];k.has(n)||(k.add(n),n())}w.length=0}while(y.length);for(;q.length;)q.pop()();N=!1,k.clear(),d(t)}function ct(t){if(t.fragment!==null){t.update(),x(t.before_update);const e=t.dirty;t.dirty=[-1],t.fragment&&t.fragment.p(t.ctx,e),t.after_update.forEach(v)}}const $=new Set;let h;function st(){h={r:0,c:[],p:h}}function lt(){h.r||x(h.c),h=h.p}function G(t,e){t&&t.i&&($.delete(t),t.i(e))}function ut(t,e,n,r){if(t&&t.o){if($.has(t))return;$.add(t),h.c.push(()=>{$.delete(t),r&&(n&&t.d(1),r())}),t.o(e)}else r&&r()}function Dt(t,e){const n=e.token={};function r(i,o,l,u){if(e.token!==n)return;e.resolved=u;let s=e.ctx;l!==void 0&&(s=s.slice(),s[l]=u);const c=i&&(e.current=i)(s);let f=!1;e.block&&(e.blocks?e.blocks.forEach((a,_)=>{_!==o&&a&&(st(),ut(a,1,1,()=>{e.blocks[_]===a&&(e.blocks[_]=null)}),lt())}):e.block.d(1),c.c(),G(c,1),c.m(e.mount(),e.anchor),f=!0),e.block=c,e.blocks&&(e.blocks[o]=c),f&&j()}if(J(t)){const i=S();if(t.then(o=>{d(i),r(e.then,1,e.value,o),d(null)},o=>{if(d(i),r(e.catch,2,e.error,o),d(null),!e.hasCatch)throw o}),e.current!==e.pending)return r(e.pending,0),!0}else{if(e.current!==e.then)return r(e.then,1,e.value,t),!0;e.resolved=t}}function qt(t,e,n){const r=e.slice(),{resolved:i}=t;t.current===t.then&&(r[t.value]=i),t.current===t.catch&&(r[t.error]=i),t.block.p(r,n)}const zt=typeof window<"u"?window:typeof globalThis<"u"?globalThis:global;function Bt(t){t&&t.c()}function Ht(t,e){t&&t.l(e)}function ot(t,e,n,r){const{fragment:i,after_update:o}=t.$$;i&&i.m(e,n),r||v(()=>{const l=t.$$.on_mount.map(z).filter(B);t.$$.on_destroy?t.$$.on_destroy.push(...l):x(l),t.$$.on_mount=[]}),o.forEach(v)}function at(t,e){const n=t.$$;n.fragment!==null&&(x(n.on_destroy),n.fragment&&n.fragment.d(e),n.on_destroy=n.fragment=null,n.ctx=[])}function ft(t,e){t.$$.dirty[0]===-1&&(y.push(t),F(),t.$$.dirty.fill(0)),t.$$.dirty[e/31|0]|=1<<e%31}function Lt(t,e,n,r,i,o,l,u=[-1]){const s=g;d(t);const c=t.$$={fragment:null,ctx:[],props:o,update:m,not_equal:i,bound:M(),on_mount:[],on_destroy:[],on_disconnect:[],before_update:[],after_update:[],context:new Map(e.context||(s?s.$$.context:[])),callbacks:M(),dirty:u,skip_bound:!1,root:e.target||s.$$.root};l&&l(c.root);let f=!1;if(c.ctx=n?n(t,e.props||{},(a,_,...T)=>{const C=T.length?T[0]:_;return c.ctx&&i(c.ctx[a],c.ctx[a]=C)&&(!c.skip_bound&&c.bound[a]&&c.bound[a](C),f&&ft(t,a)),_}):[],c.update(),f=!0,x(c.before_update),c.fragment=r?r(c.ctx):!1,e.target){if(e.hydrate){W();const a=nt(e.target);c.fragment&&c.fragment.l(a),a.forEach(Z)}else c.fragment&&c.fragment.c();e.intro&&G(t.$$.fragment),ot(t,e.target,e.anchor,e.customElement),U(),j()}d(s)}class Ot{$destroy(){at(this,1),this.$destroy=m}$on(e,n){if(!B(n))return m;const r=this.$$.callbacks[e]||(this.$$.callbacks[e]=[]);return r.push(n),()=>{const i=r.indexOf(n);i!==-1&&r.splice(i,1)}}$set(e){this.$$set&&!Q(e)&&(this.$$.skip_bound=!0,this.$$set(e),this.$$.skip_bound=!1)}}const b=[];function Pt(t,e=m){let n;const r=new Set;function i(u){if(K(t,u)&&(t=u,n)){const s=!b.length;for(const c of r)c[1](),b.push(c,t);if(s){for(let c=0;c<b.length;c+=2)b[c][0](b[c+1]);b.length=0}}}function o(u){i(u(t))}function l(u,s=m){const c=[u,s];return r.add(c),r.size===1&&(n=e(i)||m),u(t),()=>{r.delete(c),r.size===0&&(n(),n=null)}}return{set:i,update:o,subscribe:l}}export{Mt as A,Pt as B,et as C,kt as D,Y as E,m as F,dt as G,_t as H,mt as I,pt as J,ht as K,Dt as L,St as M,wt as N,qt as O,zt as P,yt as Q,Ot as S,gt as a,bt as b,Nt as c,lt as d,xt as e,G as f,st as g,Z as h,Lt as i,Ct as j,tt as k,Et as l,nt as m,$t as n,Tt as o,At as p,A as q,it as r,K as s,ut as t,vt as u,jt as v,Bt as w,Ht as x,ot as y,at as z};
