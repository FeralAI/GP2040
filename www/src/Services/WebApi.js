import axios from 'axios';

import controllers from '../Data/Controllers.json'

export const baseButtonMappings = {
	Up:    { pin: -1, error: null },
	Down:  { pin: -1, error: null },
	Left:  { pin: -1, error: null },
	Right: { pin: -1, error: null },
	B1:    { pin: -1, error: null },
	B2:    { pin: -1, error: null },
	B3:    { pin: -1, error: null },
	B4:    { pin: -1, error: null },
	L1:    { pin: -1, error: null },
	R1:    { pin: -1, error: null },
	L2:    { pin: -1, error: null },
	R2:    { pin: -1, error: null },
	S1:    { pin: -1, error: null },
	S2:    { pin: -1, error: null },
	L3:    { pin: -1, error: null },
	R3:    { pin: -1, error: null },
	A1:    { pin: -1, error: null },
	A2:    { pin: -1, error: null },
};

async function getPinMappings() {
	if (process.env.NODE_ENV === 'production') {
		return axios.get('api/getPinMappings')
			.then((response) => {
				let mappings = { ...baseButtonMappings };
				for (let prop of Object.keys(response.data))
					mappings[prop].pin = parseInt(response.data[prop]);

				return mappings;
			})
			.catch(console.error);
	}
	else {
		// Test code
		let mappings = { ...baseButtonMappings };
		for (let prop of Object.keys(controllers['pico'])) {
			if (mappings[prop])
				mappings[prop].pin = parseInt(controllers['pico'][prop]);
		}

		return mappings;
	}
}

async function setPinMappings(mappings) {
	let data = {};
	Object.keys(mappings).map((button, i) => data[button] = mappings[button].pin);

	if (process.env.NODE_ENV === 'production') {
		return axios.post(`api/setPinMappings`, data)
			.then((response) => {
				console.log(response.data);
				return true;
			})
			.catch((err) => {
				console.error(err);
				return false;
			});
	}
	else {
		console.log(data);
		return true;
	}
}

const WebApi = {
	getPinMappings,
	setPinMappings,
};

export default WebApi;
