import React, { useEffect, useState } from 'react';
import { Button, Form, Row, Col } from 'react-bootstrap';
import axios from 'axios';

import './PinMappings.scss';

const baseButtonMappings = [
	{ pin: -1, button: 'Up', xinput: 'Up', switch: 'Up', ps3: 'Up', dinput: 'Up', arcade: 'Up', error: null },
	{ pin: -1, button: 'Down', xinput: 'Down', switch: 'Down', ps3: 'Down', dinput: 'Down', arcade: 'Down', error: null },
	{ pin: -1, button: 'Left', xinput: 'Left', switch: 'Left', ps3: 'Left', dinput: 'Left', arcade: 'Left', error: null },
	{ pin: -1, button: 'Right', xinput: 'Right', switch: 'Right', ps3: 'Right', dinput: 'Right', arcade: 'Right', error: null },
	{ pin: -1, button: 'B1', xinput: 'A', switch: 'B', ps3: 'Cross', dinput: '2', arcade: 'K1', error: null },
	{ pin: -1, button: 'B2', xinput: 'B', switch: 'A', ps3: 'Circle', dinput: '3', arcade: 'K2', error: null },
	{ pin: -1, button: 'B3', xinput: 'X', switch: 'Y', ps3: 'Square', dinput: '1', arcade: 'P1', error: null },
	{ pin: -1, button: 'B4', xinput: 'Y', switch: 'X', ps3: 'Triangle', dinput: '4', arcade: 'P2', error: null },
	{ pin: -1, button: 'L1', xinput: 'LB', switch: 'L', ps3: 'L1', dinput: '5', arcade: 'L1', error: null },
	{ pin: -1, button: 'R1', xinput: 'RB', switch: 'R', ps3: 'R1', dinput: '6', arcade: 'R1', error: null },
	{ pin: -1, button: 'L2', xinput: 'LT', switch: 'ZL', ps3: 'L2', dinput: '7', arcade: 'L2', error: null },
	{ pin: -1, button: 'R2', xinput: 'RT', switch: 'ZR', ps3: 'R2', dinput: '8', arcade: 'R2', error: null },
	{ pin: -1, button: 'S1', xinput: 'Back', switch: 'Minus', ps3: 'Select', dinput: '9', arcade: 'Coin', error: null },
	{ pin: -1, button: 'S2', xinput: 'Start', switch: 'Plus', ps3: 'Start', dinput: '10', arcade: 'Start', error: null },
	{ pin: -1, button: 'L3', xinput: 'LS', switch: 'LS', ps3: 'L3', dinput: '11', arcade: 'L3', error: null },
	{ pin: -1, button: 'R3', xinput: 'RS', switch: 'RS', ps3: 'R3', dinput: '12', arcade: 'R3', error: null },
	{ pin: -1, button: 'A1', xinput: 'Guide', switch: 'Home', ps3: 'N/A', dinput: '13', arcade: 'Home', error: null },
	{ pin: -1, button: 'A2', xinput: 'N/A', switch: 'Capture', ps3: 'N/A', dinput: '14', arcade: 'N/A', error: null },
];

const buttonLabelOptions = [
	{ label: 'GP2040', value: 'button' },
	{ label: 'Arcade', value: 'arcade' },
	{ label: 'XInput', value: 'xinput' },
	{ label: 'Nintendo Switch', value: 'switch' },
	{ label: 'PS3', value: 'ps3' },
	{ label: 'DirectInput', value: 'dinput' },
]

const pinMappingParams = {
	name: 'pico',
	min: 0,
	max: 28,
	invalid: [23, 24, 25],
};

export default function PinMappingPage() {
	const [buttonMappings, setButtonMappings] = useState(baseButtonMappings);
	const [selectedButtonLabels, setSelectedButtonLabels] = useState(buttonLabelOptions[0]);
	const [validated, setValidated] = useState(false);

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

	const buttonLabelsChanged = (e) => {
		setSelectedButtonLabels(buttonLabelOptions.filter(o => o.value === e.target.value)[0]);
	};

	const handlePinChange = (e, i) => {
		validatePin(i);
		if (!buttonMappings[i].error) {
			buttonMappings[i].pin = parseInt(e.target.value);
			setButtonMappings(buttonMappings);
		}
	};

	const handleSubmit = (e) => {
		e.preventDefault();
		e.stopPropagation();

		for (let i = 0; i < buttonMappings.length; i++)
			validatePin(i);

		const form = e.currentTarget;
		if (form.checkValidity() === false) {
			console.error('Invalid!');
		}

		setValidated(true);
	};

	const validatePin = (i) => {
		let changed = false;

		if (buttonMappings[i].pin === -1)
			return;

		for (let buttonMapping of buttonMappings) {
			if (buttonMappings[i].button === buttonMapping.button)
				continue;

			if (buttonMappings[i].pin === buttonMapping.pin) {
				buttonMappings[i].error = `Pin ${buttonMappings[i].pin} is already assigned`;
				changed = true;
			}
		}

		if (!changed && pinMappingParams.invalid.filter(p => p === buttonMappings[i].pin).length > 0) {
			buttonMappings[i].error = `Pin ${buttonMappings[i].pin} is invalid for this board`;
			changed = true;
		}

		if (changed)
			setButtonMappings(buttonMappings);
	};

	return (
		<div>
			<Form noValidate validated={validated} onSubmit={handleSubmit}>
				<header>
					<Row className="mb-3">
						<Col>
							<h1>Pin Mapping</h1>
						</Col>
						<Form.Group as={Col} className="select-button-labels-container">
							<Form.Select className="select-button-labels form-select-sm float-end" onChange={buttonLabelsChanged}>
								{buttonLabelOptions.map((o, i) => <option key={`button-label-option-${i}`} value={o.value}>{o.label}</option>)}
							</Form.Select>
						</Form.Group>
					</Row>
				</header>
				<table className="table table-striped pin-mapping-table">
					<thead>
						<tr>
							<th>Pin</th>
							<th>{selectedButtonLabels.label}</th>
						</tr>
					</thead>
					<tbody>
						{buttonMappings?.map((mapping, i) =>
							<tr key={`button-map-${i}`}>
								<td>
									<Form.Control
										type="number"
										min={pinMappingParams.min}
										max={pinMappingParams.max}
										className="pin-input form-control-sm"
										isInvalid={!!mapping.error}
										onChange={(e) => handlePinChange(e, i)}
									></Form.Control>
									<Form.Control.Feedback type="invalid">{mapping.error}</Form.Control.Feedback>
								</td>
								<td>{mapping[selectedButtonLabels.value]}</td>
							</tr>
						)}
					</tbody>
				</table>

				<Button type="submit">Save</Button>
			</Form>
		</div>
	);
}
