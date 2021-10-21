import React, { useEffect, useState } from 'react';
import axios from 'axios';

import './PinMappings.scss';

const baseButtonMappings = [
	{ pin: -1, button: 'Up', xinput: '', switch: '', ps3: '', dinput: '', error: 'This is an error' },
	{ pin: -1, button: 'Down', xinput: '', switch: '', ps3: '', dinput: '', error: null },
	{ pin: -1, button: 'Left', xinput: '', switch: '', ps3: '', dinput: '', error: null },
	{ pin: -1, button: 'Right', xinput: '', switch: '', ps3: '', dinput: '', error: null },
	{ pin: -1, button: 'B1', xinput: 'A', switch: 'B', ps3: 'Cross', dinput: '2', error: null },
	{ pin: -1, button: 'B2', xinput: 'B', switch: 'A', ps3: 'Circle', dinput: '3', error: null },
	{ pin: -1, button: 'B3', xinput: 'X', switch: 'Y', ps3: 'Square', dinput: '1', error: null },
	{ pin: -1, button: 'B4', xinput: 'Y', switch: 'X', ps3: 'Triangle', dinput: '4', error: null },
	{ pin: -1, button: 'L1', xinput: 'LB', switch: 'L', ps3: 'L1', dinput: '5', error: null },
	{ pin: -1, button: 'R1', xinput: 'RB', switch: 'R', ps3: 'R1', dinput: '6', error: null },
	{ pin: -1, button: 'L2', xinput: 'LT', switch: 'ZL', ps3: 'L2', dinput: '7', error: null },
	{ pin: -1, button: 'R2', xinput: 'RT', switch: 'ZR', ps3: 'R2', dinput: '8', error: null },
	{ pin: -1, button: 'S1', xinput: 'Back', switch: 'Minus', ps3: 'Select', dinput: '9', error: null },
	{ pin: -1, button: 'S2', xinput: 'Start', switch: 'Plus', ps3: 'Start', dinput: '10', error: null },
	{ pin: -1, button: 'L3', xinput: 'LS', switch: 'LS', ps3: 'L3', dinput: '11', error: null },
	{ pin: -1, button: 'R3', xinput: 'RS', switch: 'RS', ps3: 'R3', dinput: '12', error: null },
	{ pin: -1, button: 'A1', xinput: 'Guide', switch: 'Home', ps3: '', dinput: '13', error: null },
	{ pin: -1, button: 'A2', xinput: '', switch: 'Capture', ps3: '', dinput: '14', error: null },
];

const invalidPins = [23, 24, 25];

export default function PinMappingPage() {
	const [buttonMappings, setButtonMappings] = useState(baseButtonMappings);

	useEffect(() => {
		axios.get('cgi/action?method=getPinMapping')
			.then((response) => {
				setButtonMappings(baseButtonMappings);
				console.log(response);
			})
			.catch((error) => {
				console.error(error);
			});
	}, []);

	function savePinMapping(e) {
		e.preventDefault();

		// let mappings = [...buttonMappings];
		// mappings[i].pin = e.target.value;

		setButtonMappings(buttonMappings);
	}

	function validatePinMappings(e, i) {
		console.log('validating');

		// Validate value
		if (invalidPins.filter(ip => ip == e.target.value).length > 0) {

		}

		let usedPins = [];
		for (let mapping of buttonMappings) {
			let pin = -1;
			for (let usedPin of usedPins) {
				if (usedPin === mapping.pin)
					pin = mapping.pin;
			}

			if (pin > 0) {
				mapping.error = `Pin ${mapping.pin} is already used.`;
				console.error(mapping.error);
			} else {
				usedPins.push(mapping.pin);
			}
		}

		console.log(usedPins);
	}

	return (
		<div>
			<header>
				<h1>Pin Mapping</h1>
			</header>
			<form noValidate>
				<table className="table table-dark table-striped">
					<thead>
						<tr>
							<th>Pin</th>
							<th>GP2040</th>
							<th>XInput</th>
							<th>Switch</th>
							<th>PS3</th>
							<th>DirectInput</th>
						</tr>
					</thead>
					<tbody>
						{buttonMappings?.map((mapping, i) =>
							<tr key={`button-map-${i}`}>
								<td>
									<input
										type="number"
										min="0" max="28"
										className="form-control form-control-sm pin-input"
										title="Valid pins are 0-22, 26-28"
										onChange={(e) => validatePinMappings(e, i)}
									></input>
									{mapping.error ? <span class="invalid-feedback">{mapping.error}</span> : ''}
								</td>
								<td>{mapping.button}</td>
								<td>{mapping.xinput}</td>
								<td>{mapping.switch}</td>
								<td>{mapping.ps3}</td>
								<td>{mapping.dinput}</td>
							</tr>
						)}
					</tbody>
				</table>

				<input type="submit" value="Save" class="btn btn-primary" onClick={(e) => savePinMapping(e)}></input>
			</form>
		</div>
	);
}
