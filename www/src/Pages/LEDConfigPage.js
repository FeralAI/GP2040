import React, { useEffect, useState } from 'react';
import { Button, Form } from 'react-bootstrap';
import { Formik } from 'formik';
import * as yup from 'yup';
import WebApi from '../Services/WebApi';

const LED_FORMATS = [
	{ label: 'GRB', value: 0 },
	{ label: 'RGB', value: 1 },
	{ label: 'GRBW', value: 2 },
	{ label: 'RGBW', value: 3 },
];

const LED_LAYOUTS = [
	{ label: '8-button Arcade Stick', value: 0 },
	{ label: 'Hit Box Layout', value: 1 },
	{ label: 'WASD Layout', value: 2 },
];

const schema = yup.object().shape({
	brightnessMax  : yup.number().required().positive().integer().min(0).max(255).label('Max Brightness'),
	brightnessSteps: yup.number().required().positive().integer().min(1).max(10).label('Brightness Steps'),
	ledFormat      : yup.number().required().positive().integer().min(0).max(3).label('LED Format'),
	ledLayout      : yup.number().required().positive().integer().min(0).max(2).label('LED Layout'),
	ledsPerPixel   : yup.number().required().positive().integer().min(1).label('LEDs Per Pixel'),
	dpadUp         : yup.number().positive().integer().label('Up Index'),
	dpadDown       : yup.number().positive().integer().label('Down Index'),
	dpadLeft       : yup.number().positive().integer().label('Left Index'),
	dpadRight      : yup.number().positive().integer().label('Right Index'),
	buttonB1       : yup.number().positive().integer().label('B1 Index'),
	buttonB2       : yup.number().positive().integer().label('B2 Index'),
	buttonB3       : yup.number().positive().integer().label('B3 Index'),
	buttonB4       : yup.number().positive().integer().label('B4 Index'),
	buttonL1       : yup.number().positive().integer().label('L1 Index'),
	buttonR1       : yup.number().positive().integer().label('R1 Index'),
	buttonL2       : yup.number().positive().integer().label('L2 Index'),
	buttonR2       : yup.number().positive().integer().label('R2 Index'),
	buttonS1       : yup.number().positive().integer().label('S1 Index'),
	buttonS2       : yup.number().positive().integer().label('S2 Index'),
	buttonL3       : yup.number().positive().integer().label('L3 Index'),
	buttonR3       : yup.number().positive().integer().label('R3 Index'),
	buttonA1       : yup.number().positive().integer().label('A1 Index'),
	buttonA2       : yup.number().positive().integer().label('A2 Index'),
});

const defaultValue = {
	brightnessMax: 255,
	brightnessSteps: 5,
	ledFormat: 0,
	ledLayout: 0,
	ledsPerPixel: 2,
	dpadUp: null,
	dpadDown: null,
	dpadLeft: null,
	dpadRight: null,
	buttonB1: null,
	buttonB2: null,
	buttonB3: null,
	buttonB4: null,
	buttonL1: null,
	buttonR1: null,
	buttonL2: null,
	buttonR2: null,
	buttonS1: null,
	buttonS2: null,
	buttonL3: null,
	buttonR3: null,
	buttonA1: null,
	buttonA2: null,
};

export default function LEDConfigPage() {
	const [saveMessage, setSaveMessage] = useState('');
	const [ledOptions, setLedOptions] = useState({});

	useEffect(() => {
		const options = WebApi.getLedOptions();
		setLedOptions(options);
	}, [setLedOptions]);

	return (
		<div className="card">
			<div className="card-header">
				<strong>LED Configuration</strong>
			</div>
			<div className="card-body">
				<Formik validationSchema={schema} onSubmit={console.log} initialValues={defaultValue}>
					{({
						handleSubmit,
						handleChange,
						handleBlur,
						values,
						touched,
						errors,
					}) => (
						<Form noValidate onSubmit={handleSubmit}>
							<Form.Group className="row mb-3">
								<Form.Label>Max Brightness (0-255)</Form.Label>
								<div className="col-sm-4">
									<Form.Control
										type="number"
										name="brightnessMax"
										value={values.brightnessMax}
										onChange={handleChange}
										isInvalid={errors.brightnessMax}
										min={0}
										max={255}
									/>
									<Form.Control.Feedback type="invalid">{errors.brightnessMax}</Form.Control.Feedback>
								</div>
							</Form.Group>
							<Form.Group className="row mb-3">
								<Form.Label>Brightness Steps (1-10)</Form.Label>
								<div className="col-sm-4">
									<Form.Control
										type="number"
										name="brightnessSteps"
										value={values.brightnessSteps}
										onChange={handleChange}
										isInvalid={errors.brightnessSteps}
										min={1}
										max={10}
									/>
									<Form.Control.Feedback type="invalid">{errors.brightnessSteps}</Form.Control.Feedback>
								</div>
							</Form.Group>
							<Form.Group className="row mb-3">
								<Form.Label>LED Format</Form.Label>
								<div className="col-sm-4">
									<Form.Select name="ledFormat" className="form-select-sm" value={values.ledFormat} onChange={handleChange} isInvalid={errors.ledFormat}>
										{LED_FORMATS.map((o, i) => <option key={`ledFormat-option-${i}`} value={o.value}>{o.label}</option>)}
									</Form.Select>
									<Form.Control.Feedback type="invalid">{errors.ledFormat}</Form.Control.Feedback>
								</div>
							</Form.Group>
							<Form.Group className="row mb-3">
								<Form.Label>LED Layout</Form.Label>
								<div className="col-sm-4">
									<Form.Select name="ledLayout" className="form-select-sm" value={values.ledLayout} onChange={handleChange} isInvalid={errors.ledLayout}>
										{LED_LAYOUTS.map((o, i) => <option key={`ledLayout-option-${i}`} value={o.value}>{o.label}</option>)}
									</Form.Select>
									<Form.Control.Feedback type="invalid">{errors.ledLayout}</Form.Control.Feedback>
								</div>
							</Form.Group>
							<Form.Group className="row mb-3">
								<Form.Label>LEDs Per Pixel</Form.Label>
								<div className="col-sm-4">
									<Form.Control
										type="number"
										name="ledsPerPixel"
										value={values.ledsPerPixel}
										onChange={handleChange}
										isInvalid={errors.ledsPerPixel}
										min={1}
									/>
									<Form.Control.Feedback type="invalid">{errors.ledsPerPixel}</Form.Control.Feedback>
								</div>
							</Form.Group>
							<div>
								<strong>LED Order</strong>
								<p className="card-text">
									Define the order of the LEDs on the chain. The numbering is the index and starts at 0 for the first button on your LED chain.
								</p>
							</div>
							<Button type="submit">Save</Button>
							{saveMessage ? <span className="alert">{saveMessage}</span> : null}
						</Form>
					)}
				</Formik>

			</div>
		</div>
	);
}
