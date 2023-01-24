export const manifest = {
	appDir: "_app",
	appPath: "_app",
	assets: new Set(["aclock.ttf","devices.json","favicon.png","robots.txt","rooms.json"]),
	mimeTypes: {".ttf":"font/ttf",".json":"application/json",".png":"image/png",".txt":"text/plain"},
	_: {
		entry: {"file":"_app/immutable/start-6064c76e.js","imports":["_app/immutable/start-6064c76e.js","_app/immutable/chunks/index-29b2723d.js","_app/immutable/chunks/singletons-a136d1f6.js"],"stylesheets":[],"fonts":[]},
		nodes: [
			() => import('./nodes/0.js'),
			() => import('./nodes/1.js')
		],
		routes: [
			
		],
		matchers: async () => {
			
			return {  };
		}
	}
};
