import React, { useContext, useEffect, useState } from 'react';
import { Button, Form, Row } from 'react-bootstrap';
import { Formik, useFormikContext } from 'formik';
import * as yup from 'yup';
import { AppContext } from '../Contexts/AppContext';
import Section from '../Components/Section';
import DraggableListGroup from '../Components/DraggableListGroup';
import FormControl from '../Components/FormControl';
import FormSelect from '../Components/FormSelect';
import BUTTONS from '../Data/Buttons.json';
import WebApi from '../Services/WebApi';

const LED_FORMATS = [
	{ label: 'GRB', value: 0 },
	{ label: 'RGB', value: 1 },
	{ label: 'GRBW', value: 2 },
	{ label: 'RGBW', value: 3 },
];

const LED_LAYOUTS = [
	{ label: '8-Button Layout', value: 0 },
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

const FormContext = ({ buttonLabels }) => {
	const { values, setValues } = useFormikContext();

	useEffect(() => {
		async function fetchData() {
			setValues(await WebApi.getLedOptions());
		}
		fetchData();
	}, []);

	useEffect(() => {
		if (!!values.ledFormat)
			values.ledFormat = parseInt(values.ledFormat);
	}, [values, setValues]);

	return null;
};

export default function LEDConfigPage() {
	const { buttonLabels } = useContext(AppContext);
	const [ledButtonOptions, setLedButtonOptions] = useState(Object
		.keys(BUTTONS[buttonLabels])
		.filter(p => p !== 'label' && p !== 'value')
		.map(p => ({ id: p, label: BUTTONS[buttonLabels][p], value: null }))
	);
	const [saveMessage] = useState('');
	const defaultValue = {
		brightnessMax: 255,
		brightnessSteps: 5,
		ledFormat: 0,
		ledLayout: 0,
		ledsPerPixel: 2,
	};

	useEffect(() => {
		const newButtonOptions = [...ledButtonOptions];
		for (let option of newButtonOptions)
			option.label = BUTTONS[buttonLabels][option.id];

		setLedButtonOptions(newButtonOptions);
	}, [buttonLabels, setLedButtonOptions]);

	const ledOrderChanged = (value) => {
		console.log(value);
	};

	return (
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
					<Section title="LED Configuration">
						<Row>
							<FormSelect
								label="LED Format"
								name="ledFormat"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.ledFormat}
								error={errors.ledFormat}
								isInvalid={errors.ledFormat}
								onChange={handleChange}
								>
								{LED_FORMATS.map((o, i) => <option key={`ledFormat-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
							<FormSelect
								label="LED Layout"
								name="ledLayout"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.ledLayout}
								error={errors.ledLayout}
								isInvalid={errors.ledLayout}
								onChange={handleChange}
							>
								{LED_LAYOUTS.map((o, i) => <option key={`ledLayout-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
							<FormControl type="number"
								label="LEDs Per Pixel"
								name="ledsPerPixel"
								className="form-control-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.ledsPerPixel}
								error={errors.ledsPerPixel}
								isInvalid={errors.ledsPerPixel}
								onChange={handleChange}
								min={1}
							/>
						</Row>
						<Row>
							<FormControl type="number"
								label="Max Brightness"
								name="brightnessMax"
								className="form-control-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.brightnessMax}
								error={errors.brightnessMax}
								isInvalid={errors.brightnessMax}
								onChange={handleChange}
								min={0}
								max={255}
							/>
							<FormControl type="number"
								label="Brightness Steps"
								name="brightnessSteps"
								className="form-control-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.brightnessSteps}
								error={errors.brightnessSteps}
								isInvalid={errors.brightnessSteps}
								onChange={handleChange}
								min={1}
								max={10}
							/>
						</Row>
					</Section>
					<Section title="LED Button Order">
						<p className="card-text">
							Here you can define the which buttons have RGB LEDs, and in what order they run from the control board.
							This is required for certain LED animations and static theme support.
						</p>
						<p className="card-text">
							Drag and drop list items to assign and reorder the RGB LEDs.
						</p>
						<DraggableListGroup
							groupName="test"
							titles={['Available Buttons', 'Assigned Buttons']}
							dataSources={[ledButtonOptions, []]}
							onChange={ledOrderChanged}
						/>
					</Section>
					<Button type="submit">Save</Button>
					{saveMessage ? <span className="alert">{saveMessage}</span> : null}
					<FormContext
						buttonLabels={buttonLabels}
						ledButtonOptions={ledButtonOptions}
						setLedButtonOptions={setLedButtonOptions}
					/>
				</Form>
			)}
		</Formik>
	);
}
