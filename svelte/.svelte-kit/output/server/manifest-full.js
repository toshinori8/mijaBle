export const manifest = {
	appDir: "_app",
	appPath: "_app",
	assets: new Set(["aclock.ttf","favicon.png","robots.txt"]),
	mimeTypes: {".ttf":"font/ttf",".png":"image/png",".txt":"text/plain"},
	_: {
		entry: {"file":"_app/immutable/start-5326cf19.js","imports":["_app/immutable/start-5326cf19.js","_app/immutable/chunks/index-29b2723d.js","_app/immutable/chunks/singletons-a136d1f6.js"],"stylesheets":[],"fonts":[]},
		nodes: [
			() => import('./nodes/0.js'),
			() => import('./nodes/1.js'),
			() => import('./nodes/2.js')
		],
		routes: [
			{
				id: "/",
				pattern: /^\/$/,
				params: [],
				page: { layouts: [0], errors: [1], leaf: 2 },
				endpoint: null
			}
		],
		matchers: async () => {
			
			return {  };
		}
	}
};
