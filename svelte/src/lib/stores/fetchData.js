import { writable } from 'svelte/store'
import { setContext } from 'svelte'
export default function () {
  let loading = writable(false)
  let error = writable(false)
  let rooms = writable({})
  let devices = writable({})

  setContext('rooms', rooms)
  setContext('devices', devices)

  const urlRooms = 'http://cleargrasstermostat.local/data/JSONrooms'
  const urlDev = 'http://cleargrasstermostat.local/data/JSONdevices'

  async function fetchUrl(url) {
    loading.set(true)
    error.set(false)
    try {
      const response = await fetch(url)
      return await response.json()
    } catch (e) {
      error.set(e)
    }
    loading.set(false)
  }

  // set rooms
  fetchUrl(urlRooms).then((dataRooms) => {
    if (!dataRooms) {
      return
    }
    fetchUrl(urlDev).then((dataDev) => {
      if (!dataDev) {
        return
      }
      dataDev.forEach((dev) => {
        dataRooms.forEach((room) => {
          if (room.mac == dev.mac) {
            room.temp = dev.temp
            room.hum = dev.hum
            room.batt = dev.batt
            room.lastseen = dev.lastseen
          }
        })
      })
      rooms.set(dataRooms)
      devices.set(dataDev)
    })
  })

  async function saveRooms(roomsJson) {
    console.log('saveRooms to remote server')

    try {
      const response = await fetch(
        'http://cleargrasstermostat.local/data/saveRooms',
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify(roomsJson),
        },
      )
        
      if (!response.ok) {
        throw new Error(response.statusText)
      }
      // console.log(response);
    } catch (e) {
      // console.log(e);
    }

    return true;
  }


  async function saveDevices(devicesJson) {
    console.log('saveDevices to remote server')

    try {
      const response = await fetch(
        'http://cleargrasstermostat.local/data/saveDevices',
        {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify(devicesJson),
        },
      )
        
      if (!response.ok) {
        throw new Error(response.statusText)
      }
      // console.log(response);
    } catch (e) {
      // console.log(e);
    }

    return true;


 }

  return [rooms, devices, loading, error, saveRooms, saveDevices]
}
