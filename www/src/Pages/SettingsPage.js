import React, { useEffect, useState } from 'react';
import { Button, Form } from 'react-bootstrap';
import { Formik, useFormikContext } from 'formik';
import * as yup from 'yup';
import Section from '../Components/Section';
import WebApi from '../Services/WebApi';

const INPUT_MODES = [
	{ label: 'XInput', value: 0 },
	{ label: 'Nintendo Switch', value: 1 },
	{ label: 'PS3/DirectInput', value: 2 },
];

const DPAD_MODES = [
	{ label: 'D-pad', value: 0 },
	{ label: 'Left Analog', value: 1 },
	{ label: 'Right Analog', value: 2 },
];

const SOCD_MODES = [
	{ label: 'Up Priority (Legacy)', value: 0 },
	{ label: 'Neutral (Legacy)', value: 1 },
	{ label: 'Last Win (Legacy)', value: 2 },
	{ label: 'L+R Neutral U+D Neutral', value: 3 },
	{ label: 'L+R Neutral U+D Down Priority', value: 4 },
	{ label: 'L+R Neutral U+D Up Priority', value: 5 },
	{ label: 'L+R Neutral U+D Last Input Priority', value: 6 },
	{ label: 'L+R Neutral U+D First Input Priority', value: 7 },
	{ label: 'L+R Left Priority U+D Neutral', value: 8 },
	{ label: 'L+R Left Priority U+D Down Priority', value: 9 },
	{ label: 'L+R Left Priority U+D Up Priority', value: 10 },
	{ label: 'L+R Left Priority U+D Last Input Priority', value: 11 },
	{ label: 'L+R Left Priority U+D First Input Priority', value: 12 },
	{ label: 'L+R Right Priority U+D Neutral', value: 13 },
	{ label: 'L+R Right Priority U+D Down Priority', value: 14 },
	{ label: 'L+R Right Priority U+D Up Priority', value: 15 },
	{ label: 'L+R Right Priority U+D Last Input Priority', value: 16 },
	{ label: 'L+R Right Priority U+D First Input Priority', value: 17 },
	{ label: 'L+R Last Input Priority U+D Neutral', value: 18 },
	{ label: 'L+R Last Input Priority U+D Down Priority', value: 19 },
	{ label: 'L+R Last Input Priority U+D Up Priority', value: 20 },
	{ label: 'L+R Last Input Priority U+D Last Input Priority', value: 21 },
	{ label: 'L+R Last Input Priority U+D First Input Priority', value: 22 },
	{ label: 'L+R First Input Priority U+D Neutral', value: 23 },
	{ label: 'L+R First Input Priority U+D Down Priority', value: 24 },
	{ label: 'L+R First Input Priority U+D Up Priority', value: 25 },
	{ label: 'L+R First Input Priority U+D Last Input Priority', value: 26 },
	{ label: 'L+R First Input Priority U+D First Input Priority', value: 27 },
];

const schema = yup.object().shape({
	dpadMode : yup.number().required().oneOf(DPAD_MODES.map(o => o.value)).label('D-Pad Mode'),
	inputMode: yup.number().required().oneOf(INPUT_MODES.map(o => o.value)).label('Input Mode'),
	socdMode : yup.number().required().oneOf(SOCD_MODES.map(o => o.value)).label('SOCD Mode'),
});

const FormContext = () => {
	const { values, setValues } = useFormikContext();

	useEffect(() => {
		async function fetchData() {
			setValues(await WebApi.getGamepadOptions());
		}
		fetchData();
	}, [setValues]);

	useEffect(() => {
		if (!!values.dpadMode)
			values.dpadMode = parseInt(values.dpadMode);
		if (!!values.inputMode)
			values.inputMode = parseInt(values.inputMode);
		if (!!values.socdMode)
			values.socdMode = parseInt(values.socdMode);
	}, [values, setValues]);

	return null;
};

export default function SettingsPage() {
	const [saveMessage, setSaveMessage] = useState('');

	const onSuccess = async (values) => {
		const success = WebApi.setGamepadOptions(values);
		setSaveMessage(success ? 'Saved!' : 'Unable to Save');
	};

	return (
		<Formik validationSchema={schema} onSubmit={onSuccess} initialValues={{}}>
			{({
				handleSubmit,
				handleChange,
				handleBlur,
				values,
				touched,
				errors,
			}) => (
				<Section title="Settings">
					<Form noValidate onSubmit={handleSubmit}>
						<Form.Group className="row mb-3">
							<Form.Label>Input Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="inputMode" className="form-select-sm" value={values.inputMode} onChange={handleChange} isInvalid={errors.inputMode}>
									{INPUT_MODES.map((o, i) => <option key={`button-inputMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.inputMode}</Form.Control.Feedback>
							</div>
						</Form.Group>
						<Form.Group className="row mb-3">
							<Form.Label>D-Pad Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="dpadMode" className="form-select-sm" value={values.dpadMode} onChange={handleChange} isInvalid={errors.dpadMode}>
									{DPAD_MODES.map((o, i) => <option key={`button-dpadMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.dpadMode}</Form.Control.Feedback>
							</div>
						</Form.Group>
						<Form.Group className="row mb-3">
							<Form.Label>SOCD Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="socdMode" className="form-select-sm" value={values.socdMode} onChange={handleChange} isInvalid={errors.socdMode}>
									{SOCD_MODES.map((o, i) => <option key={`button-socdMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.socdMode}</Form.Control.Feedback>
							</div>
						</Form.Group>
						<Button type="submit">Save</Button>
						{saveMessage ? <span className="alert">{saveMessage}</span> : null}
						<FormContext />
					</Form>
				</Section>
			)}
		</Formik>
	);
}
