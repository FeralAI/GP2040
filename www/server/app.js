/**
 * GP2040 Configurator Development Server
 */

const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser')

const controllers = require('../src/Data/Controllers.json');

const port = process.env.PORT || 8080;
const baseButtonMappings = {
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

const app = express();
app.use(cors());
app.use(bodyParser.json());

app.get('/api/getGamepadOptions', (req, res) => {
	console.log('/api/getGamepadOptions');
	return res.send({
		dpadMode: 0,
		inputMode: 0,
		socdMode: 0,
	});
});

app.get('/api/getPinMappings', (req, res) => {
	console.log('/api/getPinMappings');
	let mappings = { ...baseButtonMappings };
	for (let prop of Object.keys(controllers['pico'])) {
		if (mappings[prop])
			mappings[prop] = parseInt(controllers['pico'][prop]);
	}

	return res.send(mappings);
});

app.post('/api/setGamepadOptions', (req, res) => {
	console.log('/api/setGamepadOptions');
	return res.send(req.body);
});

app.post('/api/setPinMappings', (req, res) => {
	console.log('/api/setPinMappings');
	return res.send(req.body);
});

app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
});
