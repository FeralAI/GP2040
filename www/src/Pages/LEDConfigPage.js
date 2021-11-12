import React, { useEffect, useState } from 'react';
import { Button, Form } from 'react-bootstrap';
import { Formik, useFormikContext } from 'formik';
import * as yup from 'yup';
import Section from '../Components/Section';
import DraggableListGroup from '../Components/DraggableListGroup';
import buttons from '../Data/Buttons.json';
import WebApi from '../Services/WebApi';

const dataSource = Object.keys(buttons).map(p => ({ label: p, value: null }));

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
});

const defaultValue = {
	brightnessMax: 255,
	brightnessSteps: 5,
	ledFormat: 0,
	ledLayout: 0,
	ledsPerPixel: 2,
	ledButtonMap: [],
};

const FormContext = () => {
	const { values, setValues } = useFormikContext();

	useEffect(() => {
		async function fetchData() {
			setValues(await WebApi.getLedOptions());
		}
		fetchData();
	}, [setValues]);

	useEffect(() => {
		if (!!values.ledFormat)
			values.ledFormat = parseInt(values.ledFormat);
	}, [values, setValues]);

	return null;
};

export default function LEDConfigPage() {
	const [saveMessage, setSaveMessage] = useState('');

	return (
		<Section title="LED Configuration">
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
								<Form.Select
									name="ledFormat"
									className="form-select-sm"
									value={values.ledFormat}
									onChange={handleChange}
									isInvalid={errors.ledFormat}
								>
									{LED_FORMATS.map((o, i) => <option key={`ledFormat-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.ledFormat}</Form.Control.Feedback>
							</div>
						</Form.Group>
						<Form.Group className="row mb-3">
							<Form.Label>LED Layout</Form.Label>
							<div className="col-sm-4">
								<Form.Select
									name="ledLayout"
									className="form-select-sm"
									value={values.ledLayout}
									onChange={handleChange}
									isInvalid={errors.ledLayout}
								>
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
							<strong>LED Button Order</strong>
							<p className="card-text">
								Here you can define the order of your LED buttons. This is required for certain LED animations and static theme support.
							</p>
							<DraggableListGroup groupName="test" titles={['Available Buttons', 'Assigned Buttons']} dataSources={[dataSource, []]} />
						</div>
						<Button type="submit">Save</Button>
						{saveMessage ? <span className="alert">{saveMessage}</span> : null}
						<FormContext />
					</Form>
				)}
			</Formik>
		</Section>
	);
}
