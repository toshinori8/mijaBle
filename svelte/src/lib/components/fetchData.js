import { writable } from 'svelte/store'

export default function () {
	let loading = writable(false)
	let error = writable(false)
	let rooms = writable({})
	let devices = writable({})

    const urlRooms = 'http://cleargrasstermostat.local/data/JSONrooms'
    const urlDev = 'http://cleargrasstermostat.local/data/JSONdevices'

    async function fetchUrl(url) {
        loading.set(true)
        error.set(false)
        try {
            const response = await fetch(url)
            return await response.json()
        }
        catch (e) {
            error.set(e)
        }
        loading.set(false)
    }

	// set rooms 
    fetchUrl(urlRooms).then(data => {
        rooms.set(data);
        fetchUrl(urlDev).then(data => {
            devices.set(data);
        })
    })
    
	return [rooms, devices, loading, error]

}