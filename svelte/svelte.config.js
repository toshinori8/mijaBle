// import adapter from '@sveltejs/adapter-auto';
import adapter from '@sveltejs/adapter-static'

/** @type {import('@sveltejs/kit').Config} */
const config = {
  kit: {
    adapter: adapter({
      pages: 'build',
      assets: 'build',
      fallback: "index.html",
      precompress: false,
      strict: true,
	  outDir: 'builda',
    }),
  },
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