// import adapter from '@sveltejs/adapter-static';
// import preprocess from 'svelte-preprocess';

// /** @type {import('@sveltejs/kit').Config} */
// const config = {
//   preprocess: preprocess(),

//   kit: {
//     adapter: adapter(
//       { fallback: '200.html' }
//     ),
//     prerender: { enabled: false },
//     ssr: false,
//     target: '#svelte',
//     vite: {
//       build: {
//         rollupOptions: {
//           output: {
//             manualChunks: undefined
//           }
//         }
//       }
//     }
//   }
// };

// import adapter from '@sveltejs/adapter-auto';
import adapter from '@sveltejs/adapter-static'

import { vitePreprocess } from '@sveltejs/kit/vite'

/** @type {import('@sveltejs/kit').Config} */
const config = {
  kit: {
    // vite: {
    //   optimizeDeps: {
    //     include: ['lodash.get', 'lodash.isequal', 'lodash.clonedeep']
    //   }
    //   // plugins: []
    // },
    adapter: adapter({
      pages: '../data',
      assets: '../data',
      fallback: 'index.html',
      precompress: false,
      strict: true,
      outDir: '../data',
      prerender: { enabled: true },
      ssr: false,
      vite: {
        build: {
          rollupOptions: {
            output: {
              manualChunks: undefined,
              cssCodeSplit: false,
              outDir: '../data',
            },
          },
        },
      },
    }),
  },
  preprocess: vitePreprocess(),
}

export default config

// export default {
//   kit: {
//     adapter: adapter({
//       // default options are shown. On some platforms
//       // these options are set automatically — see below
//       pages: 'build',
//       assets: 'build',
//       fallback: null,
//       precompress: false,
//       strict: true
//     })
//   }
// };
