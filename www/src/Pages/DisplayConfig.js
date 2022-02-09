import React, { useEffect, useState } from 'react';
import { Button, Form, Row, Col } from 'react-bootstrap';
import { Formik, useFormikContext } from 'formik';
import * as yup from 'yup';
import FormControl from '../Components/FormControl';
import FormSelect from '../Components/FormSelect';
import Section from '../Components/Section';
import WebApi from '../Services/WebApi';

const ON_OFF_OPTIONS = [
	{ label: 'Disabled', value: 0 },
	{ label: 'Enabled', value: 1 },
];

const I2C_DISPLAY_DRIVERS = [
	{ label: 'SSD130X (1306, 1309, etc.)', value: 3 },
	{ label: 'SH1106', value: 4 },
];

const I2C_PIN_MAPPINGS = [
	{ label: 'Disabled', value: -1 },
	{ label: '0/1', value: 0 },
	{ label: '2/3', value: 2 },
	{ label: '4/5', value: 4 },
	{ label: '6/7', value: 6 },
	{ label: '8/9', value: 8 },
	{ label: '10/11', value: 10 },
	{ label: '12/13', value: 12 },
	{ label: '14/15', value: 14 },
	{ label: '16/17', value: 16 },
	{ label: '18/19', value: 18 },
	{ label: '20/21', value: 20 },
	{ label: '26/27', value: 26 },
];

const I2C_SPEEDS = [
	{ label: '100 kbit/s', value: 100000 },
	{ label: '400 kbit/s', value: 400000 },
	{ label: '800 kbit/s', value: 800000 },
];

const defaultValues = {
	i2cPinMapping: 26,
	i2cSpeed: 400000,
	i2cAddress: '3C',
	displayDriver: 3,
	flipDisplay: 0,
	invertDisplay: 0,
};

let usedPins = [];

const schema = yup.object().shape({
	// eslint-disable-next-line no-template-curly-in-string
	i2cPinMapping: yup.number().required().min(-1).max(26).test('', '${originalValue} is already assigned!', (value) => usedPins.indexOf(value) === -1).label('I2C Pins'),
	i2cSpeed: yup.number().required().label('I2C Speed'),
	i2cAddress: yup.string().required().label('I2C Address'),
	displayDriver: yup.number().min(3).max(4).required().label('Display Driver'),
	flipDisplay: yup.number().label('Flip Display'),
	invertDisplay: yup.number().label('Invert Display'),
});

function fixValues(values) {
	if (!!values.enabled)
		values.enabled = parseInt(values.enabled);
	if (!!values.i2cPinMapping)
		values.i2cPinMapping = parseInt(values.i2cPinMapping);
	if (!!values.flipDisplay)
		values.flipDisplay = parseInt(values.flipDisplay);
	if (!!values.invertDisplay)
		values.invertDisplay = parseInt(values.invertDisplay);
}

const FormContext = () => {
	const { values, setValues } = useFormikContext();

	useEffect(() => {
		async function fetchData() {
			const data = await WebApi.getDisplayOptions();
			console.log(data);
			// fixValues(data);
			usedPins = data.usedPins;
			setValues(data);
		}
		fetchData();
	}, [setValues]);

	// useEffect(() => {
	// 	fixValues(values);
	// }, [values, setValues]);

	return null;
};

export default function DisplayConfigPage() {
	const [saveMessage, setSaveMessage] = useState('');

	const onSubmit = async (values) => {
		const success = WebApi.setDisplayOptions(values);
		setSaveMessage(success ? 'Saved!' : 'Unable to Save');
	};

	return (
		<Formik validationSchema={schema} onSubmit={onSubmit} initialValues={defaultValues}>
			{({
				handleSubmit,
				handleChange,
				handleBlur,
				values,
				touched,
				errors,
			}) => (
				<Section title="Display Configuration">
					<p>
						A monochrome display can be used to show controller status and button activity. Ensure your display module
						has the following attributes:
					</p>
					<ul>
						<li>Monochrome display with 128x64 resolution</li>
						<li>Uses I2C with SSD1306, SH1106 or other compatible display driver IC</li>
						<li>Supports 3.3v operation</li>
					</ul>
					<Form noValidate onSubmit={handleSubmit}>
						<h5 className="mt-4">I2C Options</h5>
						<Row>
							<FormSelect
								label="I2C GPIO (SDA/SCL)"
								name="i2cPinMapping"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.i2cPinMapping}
								error={errors.i2cPinMapping}
								isInvalid={errors.i2cPinMapping}
								onChange={handleChange}
							>
								{I2C_PIN_MAPPINGS.map((o, i) => <option key={`i2cPinMapping-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
							<FormSelect
								label="I2C Speed"
								name="i2cSpeed"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.i2cSpeed}
								error={errors.i2cSpeed}
								isInvalid={errors.i2cSpeed}
								onChange={handleChange}
							>
								{I2C_SPEEDS.map((o, i) => <option key={`i2cSpeed-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
							<FormControl type="text"
								label="I2C Address"
								name="i2cAddress"
								className="form-control-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.i2cAddress}
								error={errors.i2cAddress}
								isInvalid={errors.i2cAddress}
								onChange={handleChange}
								maxLength={4}
							/>
						</Row>
						<h5 className="mt-3">Display Options</h5>
						<Row className="mb-3">
							<FormSelect
								label="Driver IC"
								name="displayDriver"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.displayDriver}
								error={errors.displayDriver}
								isInvalid={errors.displayDriver}
								onChange={handleChange}
							>
								{I2C_DISPLAY_DRIVERS.map((o, i) => <option key={`displayDriver-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
							<FormSelect
								label="Flip Display"
								name="flipDisplay"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.flipDisplay}
								error={errors.flipDisplay}
								isInvalid={errors.flipDisplay}
								onChange={handleChange}
							>
								{ON_OFF_OPTIONS.map((o, i) => <option key={`flipDisplay-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
							<FormSelect
								label="Invert Display"
								name="invertDisplay"
								className="form-select-sm"
								groupClassName="col-sm-4 mb-3"
								value={values.invertDisplay}
								error={errors.invertDisplay}
								isInvalid={errors.invertDisplay}
								onChange={handleChange}
							>
								{ON_OFF_OPTIONS.map((o, i) => <option key={`invertDisplay-option-${i}`} value={o.value}>{o.label}</option>)}
							</FormSelect>
						</Row>
						<div className="mt-3">
							<Button type="submit">Save</Button>
							{saveMessage ? <span className="alert">{saveMessage}</span> : null}
						</div>
						<FormContext />
					</Form>
				</Section>
			)}
		</Formik>
	);
}
