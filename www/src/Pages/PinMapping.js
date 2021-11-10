import React, { useEffect, useState } from 'react';
import { NavLink } from "react-router-dom";
import { Button, Form } from 'react-bootstrap';

import WebApi, { baseButtonMappings } from '../Services/WebApi';
import boards from '../Data/Boards.json'
import buttons from '../Data/Buttons.json'

import './PinMappings.scss';

const buttonLabelOptions = [
	{ label: 'GP2040', value: 'gp2040' },
	{ label: 'Arcade', value: 'arcade' },
	{ label: 'XInput', value: 'xinput' },
	{ label: 'Nintendo Switch', value: 'switch' },
	{ label: 'PS3', value: 'ps3' },
	{ label: 'DirectInput', value: 'dinput' },
];

const requiredButtons = ['B1', 'B2', 'B3', 'S2'];

export default function PinMappingPage() {
	const [validated, setValidated] = useState(false);
	const [saveMessage, setSaveMessage] = useState('');
	const [buttonMappings, setButtonMappings] = useState(baseButtonMappings);
	const [selectedController] = useState(process.env.REACT_APP_GP2040_CONTROLLER);
	const [selectedBoard] = useState(process.env.REACT_APP_GP2040_BOARD);
	const [selectedButtonLabels, setSelectedButtonLabels] = useState(buttonLabelOptions[0]);

	useEffect(() => {
		async function fetchData() {
			setButtonMappings(await WebApi.getPinMappings());
		}

		fetchData();
	}, [setButtonMappings, selectedController]);

	const buttonLabelsChanged = (e) => {
		setSelectedButtonLabels(buttonLabelOptions.filter(o => o.value === e.target.value)[0]);
	};

	const handlePinChange = (e, prop) => {
		const newMappings = {...buttonMappings};
		if (e.target.value)
			newMappings[prop].pin = parseInt(e.target.value);
		else
			newMappings[prop].pin = '';

		validateMappings(newMappings);
	};

	const handleSubmit = async (e) => {
		e.preventDefault();
		e.stopPropagation();

		let mappings = {...buttonMappings};
		validateMappings(mappings);

		if (Object.keys(mappings).filter(p => !!mappings[p].error).length) {
			setSaveMessage('Validation errors, see above');
			return;
		}

		const success = await WebApi.setPinMappings(mappings);
		setSaveMessage(success ? 'Saved!' : 'Unable to Save');
	};

	const validateMappings = (mappings) => {
		const props = Object.keys(mappings);

		for (let prop of props) {
			mappings[prop].error = null;

			for (let otherProp of props) {
				if (prop === otherProp)
					continue;

				if (mappings[prop].pin === '' && !requiredButtons.filter(b => b === mappings[otherProp].button).length)
					mappings[prop].error = `${mappings[prop].button} is required`;
				else if (mappings[prop].pin === mappings[otherProp].pin)
					mappings[prop].error = `Pin ${mappings[prop].pin} is already assigned`;
				else if (boards[selectedBoard].invalidPins.filter(p => p === mappings[prop].pin).length > 0)
					mappings[prop].error = `Pin ${mappings[prop].pin} is invalid for this board`;
			}

		}

		setButtonMappings(mappings);
		setValidated(true);
	};

	return (
		<div className="card">
			<div className="card-header">
				<strong>Pin Mapping</strong>
			</div>
			<div className="card-body">
				<Form noValidate validated={validated} onSubmit={handleSubmit}>
					<p>Use the form below to reconfigure your button-to-pin mapping.</p>
					<div className="alert alert-warning">
						Mapping buttons to pins that aren't connected or available can leave the device in non-functional state. To clear the
						the invalid configuration go to the <NavLink exact={true} to="/reset-settings">Reset Settings</NavLink> page.
					</div>
					<Form.Group className="select-button-labels-container">
						<Form.Label>Labels</Form.Label>
						<Form.Select className="select-button-labels form-select-sm" onChange={buttonLabelsChanged}>
							{buttonLabelOptions.map((o, i) => <option key={`button-label-option-${i}`} value={o.value}>{o.label}</option>)}
						</Form.Select>
					</Form.Group>
					<table className="table table-sm pin-mapping-table">
						<thead className="table">
							<tr>
								<th className="table-header-button-label">{selectedButtonLabels.label}</th>
								<th>Pin</th>
							</tr>
						</thead>
						<tbody>
							{Object.keys(buttons)?.map((button, i) =>
								<tr key={`button-map-${i}`} className={validated && !!buttonMappings[button].error ? "table-danger" : ""}>
									<td>{buttons[button][selectedButtonLabels.value]}</td>
									<td>
										<Form.Control
											type="number"
											className="pin-input form-control-sm"
											value={buttonMappings[button].pin}
											min={boards[selectedBoard].minPin}
											max={boards[selectedBoard].maxPin}
											isInvalid={!!buttonMappings[button].error}
											required={requiredButtons.filter(b => b === button).length}
											onChange={(e) => handlePinChange(e, button)}
										></Form.Control>
										{boards[selectedBoard]?.min}
										<Form.Control.Feedback type="invalid">{buttonMappings[button].error}</Form.Control.Feedback>
									</td>
								</tr>
							)}
						</tbody>
					</table>

					<Button type="submit">Save</Button>
					{saveMessage ? <span className="alert">{saveMessage}</span> : null}
				</Form>
			</div>
		</div>
	);
}
