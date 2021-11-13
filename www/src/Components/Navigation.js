import React, { useContext } from 'react';
import { Nav, NavDropdown, Navbar } from 'react-bootstrap';
import { NavLink } from "react-router-dom";
import { AppContext } from '../Contexts/AppContext';
import FormSelect from './FormSelect';
import { saveButtonLabels } from '../Services/Storage';
import BUTTONS from '../Data/Buttons.json';
import './Navigation.scss';

const Navigation = (props) => {
	const { buttonLabels, setButtonLabels } = useContext(AppContext);

	const updateButtonLabels = (e) => {
		saveButtonLabels(e.target.value);
		setButtonLabels(e.target.value);
	};

	return (
		<Navbar bg="dark" variant="dark" expand="md" fixed="top">
			<Navbar.Brand href="/">
				<img src="images/logo.png" className="title-logo" alt="logo" />{' '}GP2040
			</Navbar.Brand>
			<Navbar.Collapse id="basic-navbar-nav">
				<Nav as="ul" className="me-auto">
					<Nav.Item as="li">
						<Nav.Link as={NavLink} exact={true} className="nav-link" activeClassName="active" to="/">Home</Nav.Link>
					</Nav.Item>
					<Nav.Item as="li">
						<Nav.Link as={NavLink} exact={true} className="nav-link" activeClassName="active" to="/settings">Settings</Nav.Link>
					</Nav.Item>
					<NavDropdown as="li" title="Configuration">
						<NavDropdown.Item as={NavLink} to="/pin-mapping">Pin Mapping</NavDropdown.Item>
						<NavDropdown.Item as={NavLink} to="/led-config">LED Configuration</NavDropdown.Item>
					</NavDropdown>
					<NavDropdown as="li" title="Links">
						<NavDropdown.Item as={NavLink} to="https://feralai.github.io/GP2040/">Documentation</NavDropdown.Item>
						<NavDropdown.Item as={NavLink} to="https://github.com/FeralAI/GP2040/">Github</NavDropdown.Item>
					</NavDropdown>
					<NavDropdown as="li" title="DANGER ZONE" className="btn-danger danger-zone">
						<NavDropdown.Item as={NavLink} to="/reset-settings">Reset Settings</NavDropdown.Item>
					</NavDropdown>
				</Nav>
			</Navbar.Collapse>
			<div className="navbar-tools">
				<FormSelect
					name="buttonLabels"
					className="form-select-sm"
					value={buttonLabels}
					onChange={updateButtonLabels}
				>
					{Object.keys(BUTTONS).map((b, i) =>
						<option key={`button-label-option-${i}`} value={BUTTONS[b].value}>{BUTTONS[b].label}</option>
						)}
				</FormSelect>
				<Navbar.Toggle aria-controls="basic-navbar-nav" />
			</div>
		</Navbar>
	);
};

export default Navigation;
