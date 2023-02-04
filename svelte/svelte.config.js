// import adapter from '@sveltejs/adapter-auto';
import adapter from '@sveltejs/adapter-static'

import { vitePreprocess } from '@sveltejs/kit/vite';


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
      pages: 'build',
      assets: 'build',
      fallback: "index.html",
      precompress: false,
      strict: true,
	  outDir: 'builda',
    }
    ),
  },
  preprocess: vitePreprocess()

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
