import React, { useEffect, useState } from 'react';
import { Button, Form } from 'react-bootstrap';
import WebApi from '../Services/WebApi';

const InputModes = [
	{ label: 'XInput', value: 0 },
	{ label: 'Nintendo Switch', value: 1 },
	{ label: 'PS3/DirectInput', value: 2 },
];

const DpadModes = [
	{ label: 'D-pad', value: 0 },
	{ label: 'Left Analog', value: 1 },
	{ label: 'Right Analog', value: 2 },
];

const SOCDModes = [
	{ label: 'Up Priority', value: 0 },
	{ label: 'Neutral', value: 1 },
	{ label: 'Last Win', value: 2 },
];

export default function SettingsPage() {
	const [validated, setValidated] = useState(false);
	const [saveMessage, setSaveMessage] = useState('');
	const [gamepadOptions, setGamepadOptions] = useState({});

	useEffect(() => {
		async function fetchData() {
			setGamepadOptions(await WebApi.getGamepadOptions());
		}

		fetchData();
	}, [setGamepadOptions]);

	const dpadModeChanged = (e) => {
		const options = { ...gamepadOptions };
		options.dpadMode = parseInt(e.target.value);
		setGamepadOptions(options);
	};

	const inputModeChanged = (e) => {
		const options = { ...gamepadOptions };
		options.inputMode = parseInt(e.target.value);
		setGamepadOptions(options);
	};

	const socdModeChanged = (e) => {
		const options = { ...gamepadOptions };
		options.socdMode = parseInt(e.target.value);
		setGamepadOptions(options);
	};

	const handleSubmit = async (e) => {
		e.preventDefault();
		e.stopPropagation();

		const form = e.currentTarget;
		const isValid = form.checkValidity();
		setValidated(true);

		if (isValid)
		{
			const success = WebApi.setGamepadOptions(gamepadOptions);
			setSaveMessage(success ? 'Saved!' : 'Unable to Save');
		}
	};

	return (
		<div>
			<Form noValidate validated={validated} onSubmit={handleSubmit}>
				<header>
					<h1>Settings</h1>
				</header>
				<Form.Group className="row mb-3">
					<Form.Label>Input Mode</Form.Label>
					<div className="col-sm-4">
						<Form.Select className="form-select-sm" onChange={inputModeChanged}>
							{InputModes.map((o, i) => <option key={`button-inputMode-option-${i}`} value={o.value}>{o.label}</option>)}
						</Form.Select>
					</div>
				</Form.Group>
				<Form.Group className="row mb-3">
					<Form.Label>D-Pad Mode</Form.Label>
					<div className="col-sm-4">
						<Form.Select className="form-select-sm" onChange={dpadModeChanged}>
							{DpadModes.map((o, i) => <option key={`button-inputMode-option-${i}`} value={o.value}>{o.label}</option>)}
						</Form.Select>
					</div>
				</Form.Group>
				<Form.Group className="row mb-3">
					<Form.Label>SOCD Mode</Form.Label>
					<div className="col-sm-4">
						<Form.Select className="form-select-sm" onChange={socdModeChanged}>
							{SOCDModes.map((o, i) => <option key={`button-inputMode-option-${i}`} value={o.value}>{o.label}</option>)}
						</Form.Select>
					</div>
				</Form.Group>
				<Button type="submit">Save</Button>
				{saveMessage ? <span className="alert">{saveMessage}</span> : null}
			</Form>
		</div>
	);
}
